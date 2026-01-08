package main

import (
	"bufio"
	"fmt"
	"os"
	"slices"
	"strings"
)

func containsABBA(inStr string) bool {
	for i := 0; i < len(inStr)-3; i++ {
		if inStr[i] == inStr[i+3] && inStr[i+1] == inStr[i+2] && inStr[i] != inStr[i+1] {
			return true
		}
	}
	return false
}

func main() {
	// declare variables
	var result int

	// read and parse the input
	file, err := os.Open("inputtest2.txt")
	if err != nil {
		fmt.Println("Error opening file:", err)
		os.Exit(1)
	}
	defer file.Close()

	scanner := bufio.NewScanner(file)

	for scanner.Scan() {
		line := scanner.Text()

		var outside []string // parts outside brackets
		var inside []string  // parts inside brackets

		currentPos := 0

		for {
			start := strings.Index(line[currentPos:], "[")
			if start == -1 {
				// no more brackets, add the rest
				if currentPos < len(line) {
					outside = append(outside, line[currentPos:])
				}
				break
			}

			// adjust start to absolute position
			start += currentPos

			// add the part before the bracket
			if start > currentPos {
				outside = append(outside, line[currentPos:start])
			}

			// find the closing bracket
			end := strings.Index(line[start:], "]")
			if end == -1 {
				break // malformed string
			}
			end += start

			// add the part inside brackets
			inside = append(inside, line[start+1:end])

			// move past this bracket pair
			currentPos = end + 1
		}

		// check if the parts outside of the brackets contain at least one ABBA and the inside of the brackets contains none
		if slices.ContainsFunc(outside, containsABBA) && !slices.ContainsFunc(inside, containsABBA) {
			result++
		}

		// part 2
		// loop over outside strings and search for ABAs
		// everytime one is found create a string BAB (string builder)
		// loop over all strings in the inside and see if the current BAB is in there somewhere
		// if it is increase the result and stop
		for _, outWord := range outside {
			for i := 0; i < len(outWord)-2; i++ {
				if (outWord[i] == outWord[i+2]) && (outWord[i+1] != outWord[i]) {
					var currBAB strings.Builder
					currBAB.WriteByte(outWord[i+1])
					currBAB.WriteByte(outWord[i])
					currBAB.WriteByte(outWord[i+1])
					for _, inWord := range inside {
						if strings.Contains(inWord, currBAB.String()) {
							fmt.Printf("juhuuuuu, das wort ist: %s und es ist in %s\n", currBAB.String(), inWord)
						}
					}
				}
			}
		}
	}

	// print results
	fmt.Printf("There are %d IP addresses that support TLS.\n", result)

	// check for errors during scanning
	if err := scanner.Err(); err != nil {
		fmt.Println("Error reading file:", err)
		os.Exit(1)
	}
}
