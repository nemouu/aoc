package main

import (
	"bufio"
	"fmt"
	"os"
	"strconv"
	"strings"
)

func checkValue(manualVal string) bool {
	_, err := strconv.Atoi(manualVal)
	return err == nil // returns true if it's a number, false if not
}

func getSignal(fromStr string, inInstructions map[string]string, inCache map[string]uint16) uint16 {
	if checkValue(fromStr) {
		value, err := strconv.Atoi(fromStr)
		if err == nil {
			return uint16(value)
		} else {
			fmt.Println("ERROR")
			return 0
		}
	} else {
		// not a number - lets look for a cache entry
		instruction, exists := inCache[fromStr]
		if exists {
			return instruction
		} else {
			// not a number and not in cache - lets look up instruction
			var instruct string = inInstructions[fromStr]
			var arg1 string
			var op string
			var arg2 string
			var res uint16

			fmt.Sscanf(instruct, "%s %s %s", &arg1, &op, &arg2)

			if op == "" {
				res = getSignal(arg1, inInstructions, inCache)
			} else if arg1 == "NOT" {
				res = ^getSignal(op, inInstructions, inCache)
			} else {
				switch op {
				case "AND":
					res = getSignal(arg1, inInstructions, inCache) & getSignal(arg2, inInstructions, inCache)
				case "OR":
					res = getSignal(arg1, inInstructions, inCache) | getSignal(arg2, inInstructions, inCache)
				case "LSHIFT":
					res = getSignal(arg1, inInstructions, inCache) << getSignal(arg2, inInstructions, inCache)
				case "RSHIFT":
					res = getSignal(arg1, inInstructions, inCache) >> getSignal(arg2, inInstructions, inCache)
				default:
					fmt.Println("ERROR")
				}
			}
			inCache[fromStr] = res
			return res
		}
	}
}

func main() {
	// variables used
	var result1 uint16 = 0
	var result2 uint16 = 0
	var parts []string

	// maps for instructions and cache
	instructions := make(map[string]string)
	cache1 := make(map[string]uint16)
	cache2 := make(map[string]uint16)

	// Open the file
	file, err := os.Open("input7.txt")
	if err != nil {
		fmt.Println("Error opening file:", err)
		os.Exit(1)
	}
	defer file.Close() // Automatically closes when function exits

	// Create a scanner to read line by line
	scanner := bufio.NewScanner(file)

	for scanner.Scan() {
		line := scanner.Text()              // Get the current line as a string
		parts = strings.Split(line, " -> ") // split the strings at " -> " to get key value pairs
		instructions[parts[1]] = parts[0]   // save those pairs in the map
	}

	// test print of read in values
	// for key, value := range instructions {
	// 	fmt.Println(key, ":", value)
	// }

	result1 = getSignal("a", instructions, cache1)

	// converts the uint16 result to an int, then converts that integer to a string representation - changes the instruction for "b"
	instructions["b"] = strconv.Itoa(int(result1))

	result2 = getSignal("a", instructions, cache2)

	// Print the results
	fmt.Printf("The signal for the wire a is: %d\n", result1)
	fmt.Printf("The signal for the wire a after shorting wire b is: %d\n", result2)

	// Check for errors during scanning
	if err := scanner.Err(); err != nil {
		fmt.Println("Error reading file:", err)
		os.Exit(1)
	}
}
