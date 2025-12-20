package main

import (
	"bufio"
	"fmt"
	"os"
)

func main() {
	// variables used
	var codeChars int = 0
	var strChars int = 0
	var encodedChars int = 0

	// Open the file
	file, err := os.Open("input8.txt")
	if err != nil {
		fmt.Println("Error opening file:", err)
		os.Exit(1)
	}
	defer file.Close() // Automatically closes when function exits

	// Create a scanner to read line by line
	scanner := bufio.NewScanner(file)

	for scanner.Scan() {
		line := scanner.Text() // Get the current line as a string

		// get the count of the characters in the code
		codeChars += len(line)

		// get the count of the characters in memory for string values
		// in this version we do not substract since we already exclude the "" in the iterator
		for i := 1; i < len(line)-1; i++ {

			// if the character '\\' or '\"' is in the string, count it as one character by advancing the iterator ahead of time
			if line[i] == '\\' && (line[i+1] == '\\' || line[i+1] == '"') {
				i++
			} else if line[i] == '\\' && line[i+1] == 'x' { // if the character '\\' is in the string and followed by an 'x' we know it will be a hex number so count it as one character
				i = i + 3
			}
			strChars++
		}

		// count the encoded string lengths '"' -> '\n' and '\' -> '\\'
		for char := range line {
			if line[char] == '"' || line[char] == '\\' {
				encodedChars++
			}
			encodedChars++
		}

		// add the counts for the beginning and end of the strings
		encodedChars += 2
	}

	// Print the results
	fmt.Printf("\nThe total number of characters of string code (%d) minus the total number of characters in memory "+
		"for string values (%d) is: %d\n", codeChars, strChars, codeChars-strChars)
	fmt.Printf("\nThe total number of characters to represent the newly encoded strings (%d) minus the number of "+
		"characters of code in each original string literal (%d) is: %d\n\n", encodedChars, codeChars, encodedChars-codeChars)

	// Check for errors during scanning
	if err := scanner.Err(); err != nil {
		fmt.Println("Error reading file:", err)
		os.Exit(1)
	}
}
