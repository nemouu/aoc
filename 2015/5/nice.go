package main

import (
	"bufio"
	"fmt"
	"os"
)

// checks if the input string contains at least 3 vowels
func containsThreeVowels(inStr string) bool {
	var vCount int = 0
	for i := 0; i < len(inStr); i++ {
		curr_char := inStr[i]
		if curr_char == 'a' || curr_char == 'e' || curr_char == 'i' || curr_char == 'o' || curr_char == 'u' {
			vCount++
		}
		if vCount > 2 {
			return true
		}
	}
	return false
}

// Checks if the input string contains at least one double letter like for example "aa"
func containsDoubleLetter(inStr string) bool {
	for i := 0; i < len(inStr)-1; i++ {
		curr_char := inStr[i]
		next_char := inStr[i+1]
		if curr_char == next_char {
			return true
		}
	}
	return false
}

// checks if the first input string contains the second one. the second one has to have 2 characters
func containsInputStringLengthTwo(inStr string, conStr string) bool {
	for i := 0; i < len(inStr)-1; i++ {
		curr_char := inStr[i]
		next_char := inStr[i+1]
		curr_str := string(curr_char) + string(next_char)
		if curr_str == conStr {
			return true
		}
	}
	return false
}

// chccks if the input string contains a pair of characters twice (without overlapping)
func containsPairTwice(inStr string) bool {
	for i := 0; i < len(inStr)-2; i++ {
		curr_char := inStr[i]
		next_char := inStr[i+1]
		curr_str := string(curr_char) + string(next_char)
		curr_search := inStr[i+2:]
		if containsInputStringLengthTwo(curr_search, curr_str) {
			return true
		}
	}
	return false
}

// checks if the input string contains a substring with two identical letters that have a different letter in between
func containsOneWithOneBetween(inStr string) bool {
	for i := 0; i < len(inStr)-2; i++ {
		curr_char := inStr[i]
		next_char := inStr[i+1]
		third_char := inStr[i+2]
		if curr_char == third_char && curr_char != next_char {
			return true
		}

	}
	return false
}

// a simple function to summarize the needed results for the first part of the riddle
func niceOrNaughty(inStr string) bool {
	return !containsInputStringLengthTwo(inStr, "ab") && !containsInputStringLengthTwo(inStr, "cd") && !containsInputStringLengthTwo(inStr, "pq") && !containsInputStringLengthTwo(inStr, "xy") && containsDoubleLetter(inStr) && containsThreeVowels(inStr)
}

// a simple function to summarize the needed results for the second part of the riddle
func niceOrNaughtyRev(inStr string) bool {
	return containsPairTwice(inStr) && containsOneWithOneBetween(inStr)
}

func main() {
	// variables used
	var result int = 0
	var result_rev int = 0

	// Open the file
	file, err := os.Open("input5.txt")
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
		if niceOrNaughty(line) {
			result++
		}
		if niceOrNaughtyRev(line) {
			result_rev++
		}
	}

	// Print the results
	fmt.Printf("The number of nice strings is: %d\n", result)
	fmt.Printf("The number of nice strings revisited is: %d\n", result_rev)

	// Check for errors during scanning
	if err := scanner.Err(); err != nil {
		fmt.Println("Error reading file:", err)
		os.Exit(1)
	}
}
