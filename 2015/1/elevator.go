package main

import (
	"bufio"
	"fmt"
	"os"
)

func main() {
	// variables used
	var result int = 0
	var char_pos int = 0
	var pos_checked bool = false

	// Open the file
	file, err := os.Open("input1.txt")
	if err != nil {
		fmt.Println("Error opening file:", err)
		os.Exit(1)
	}
	defer file.Close() // Automatically closes when function exits

	// Create a scanner to read line by line
	scanner := bufio.NewScanner(file)

	// Read line by line
	for scanner.Scan() {
		line := scanner.Text() // Get the current line as a string

		// find values the riddle asks for with a for loop
		for i := 0; i < len(line); i++ {
			if line[i] == '(' {
				result++
			}
			if line[i] == ')' {
				result--
			}
			if result < 0 && !pos_checked {
				char_pos = i + 1
				pos_checked = true
			}
		}
	}

	// Print the results
	fmt.Printf("The floor in the end is: %d\n", result)
	fmt.Printf("The position of first entering the cellar is: %d\n", char_pos)

	// Check for errors during scanning
	if err := scanner.Err(); err != nil {
		fmt.Println("Error reading file:", err)
		os.Exit(1)
	}
}
