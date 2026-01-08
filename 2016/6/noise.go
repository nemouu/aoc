package main

import (
	"bufio"
	"fmt"
	"os"
	"strings"
)

func main() {
	// declare variables
	var lines []string // this time we store the data in memory to iterate several times
	var maxMsg strings.Builder
	var minMsg strings.Builder

	// read and parse the input
	file, err := os.Open("input6.txt")
	if err != nil {
		fmt.Println("Error opening file:", err)
		os.Exit(1)
	}
	defer file.Close()

	scanner := bufio.NewScanner(file)

	for scanner.Scan() {
		lines = append(lines, scanner.Text())
	}

	// Now you can iterate over lines multiple times - first for part 1 and 2
	for i := 0; i < len(lines[0]); i++ {

		// We use a map to save the letter count
		letterCount := make(map[byte]int)
		for _, line := range lines {
			letterCount[line[i]]++
		}

		// create a new "empty maximum" on every iteration to get the maximum count of each column
		maxCount := 0
		var maxLetter byte
		minCount := len(lines)
		var minLetter byte
		for letter, count := range letterCount {
			if count > maxCount {
				maxCount = count
				maxLetter = letter
			}
			if count < minCount {
				minCount = count
				minLetter = letter
			}
		}
		maxMsg.WriteByte(maxLetter)
		minMsg.WriteByte(minLetter)
	}

	// we print out the word letter by letter - then we can copy it from the output
	fmt.Printf("The message in for part 1 is: %s\n", maxMsg.String())
	fmt.Printf("The message in for part 2 is: %s\n", minMsg.String())

	// Check for errors during scanning
	if err := scanner.Err(); err != nil {
		fmt.Println("Error reading file:", err)
		os.Exit(1)
	}
}
