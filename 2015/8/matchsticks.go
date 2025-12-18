package main

import (
	"bufio"
	"fmt"
	"os"
)

func main() {
	// variables used
	var result int = 0

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
		fmt.Println(line)
	}

	// Print the results
	fmt.Printf("The signal for the wire a is: %d\n", result)

	// Check for errors during scanning
	if err := scanner.Err(); err != nil {
		fmt.Println("Error reading file:", err)
		os.Exit(1)
	}
}
