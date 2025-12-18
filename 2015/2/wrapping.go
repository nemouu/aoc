package main

import (
	"bufio"
	"fmt"
	"os"
)

// determine the minimum of three input integers, account for integers being equal (<=)
func min_of_three(a int, b int, c int) int {
	if a <= b && a <= c {
		return a
	} else if b <= c && b <= a { // although b < a is redundant here
		return b
	} else {
		return c
	}
}

// determine the two smallest numbers of three input integers, account for integers being equal (<=)
func two_smallest_of_three(a int, b int, c int) (int, int) {
	if a <= c && b <= c {
		return a, b
	} else if a <= b && c <= b {
		return a, c
	} else {
		return c, b
	}
}

func main() {
	// variables used
	var test_input string = "1x1x10"
	var length int
	var width int
	var height int
	var side_a int
	var side_b int
	var side_c int
	var needed_paper int = 0
	var needed_ribbon int = 0
	var smallest int
	var sec_smallest int

	// Open the file
	file, err := os.Open("input2.txt")
	if err != nil {
		fmt.Println("Error opening file:", err)
		os.Exit(1)
	}
	defer file.Close() // Automatically closes when function exits

	// Create a scanner to read line by line
	scanner := bufio.NewScanner(file)

	// BEGIN TEST: Test with a test input first
	fmt.Sscanf(test_input, "%dx%dx%d", &length, &width, &height)
	fmt.Printf("%d * %d * %d\n", length, width, height)

	side_a = length * width
	side_b = length * height
	side_c = height * width

	needed_paper += 2*side_a + 2*side_b + 2*side_c + min_of_three(side_a, side_b, side_c)

	fmt.Printf("The elves should order %d square feet of wrapping paper (Test input).\n", needed_paper)

	smallest, sec_smallest = two_smallest_of_three(length, width, height)

	fmt.Printf("The smallest: %d and the second smallest: %d.\n\n", smallest, sec_smallest)

	needed_ribbon += 2*smallest + 2*sec_smallest + length*width*height

	fmt.Printf("The elves should order %d feet of ribbon (Test input).\n\n", needed_ribbon)
	// END TEST

	// reset for real calculation
	needed_paper = 0
	needed_ribbon = 0

	// Read line by line now
	for scanner.Scan() {
		line := scanner.Text() // Get the current line as a string
		fmt.Sscanf(line, "%dx%dx%d", &length, &width, &height)

		// calculate the sides for the current data - like this we can compare them easily
		side_a = length * width
		side_b = length * height
		side_c = height * width

		// do the necessary calculations and comparison and add it to the result
		needed_paper += 2*side_a + 2*side_b + 2*side_c + min_of_three(side_a, side_b, side_c)

		// get the two smallest numbers out of the input to do the ribbon calculation
		smallest, sec_smallest = two_smallest_of_three(length, width, height)
		needed_ribbon += 2*smallest + 2*sec_smallest + length*width*height // add up to the ribbon length
	}

	// print the result
	fmt.Printf("The elves should order %d square feet of wrapping paper.\n", needed_paper)
	fmt.Printf("The elves should order %d feet of ribbon.\n\n", needed_ribbon)

	// Check for errors during scanning
	if err := scanner.Err(); err != nil {
		fmt.Println("Error reading file:", err)
		os.Exit(1)
	}
}
