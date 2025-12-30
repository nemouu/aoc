package main

import (
	"bufio"
	"fmt"
	"os"
)

func isValidTriangle(inOne int, inTwo int, inThree int) bool {
	if ((inOne + inTwo) > inThree) && ((inOne + inThree) > inTwo) && ((inThree + inTwo) > inOne) {
		return true
	} else {
		return false
	}
}

func main() {
	// declare variables for part 1
	var firstSideOne int
	var secondSideOne int
	var thridSideOne int
	var resultOne int = 0

	// declare variables for part 2
	var lineCounter int = 1
	var sides [9]int
	var resultTwo int = 0

	// read and parse the input
	file, err := os.Open("input3.txt")
	if err != nil {
		fmt.Println("Error opening file:", err)
		os.Exit(1)
	}
	defer file.Close()

	scanner := bufio.NewScanner(file)

	for scanner.Scan() {
		line := scanner.Text()

		// read in numbers for each line
		fmt.Sscanf(line, "%d %d %d", &firstSideOne, &secondSideOne, &thridSideOne)

		// part 1
		if isValidTriangle(firstSideOne, secondSideOne, thridSideOne) {
			resultOne++
		}

		// part 2
		switch lineCounter % 3 {
		case 1:
			sides[0] = firstSideOne
			sides[1] = secondSideOne
			sides[2] = thridSideOne
		case 2:
			sides[3] = firstSideOne
			sides[4] = secondSideOne
			sides[5] = thridSideOne
		case 0:
			sides[6] = firstSideOne
			sides[7] = secondSideOne
			sides[8] = thridSideOne

			for i := range 3 {
				if isValidTriangle(sides[i], sides[i+3], sides[i+6]) {
					resultTwo++
				}
			}
		}
		lineCounter++
	}

	// print the result
	fmt.Printf("There are %d valid triangles in the input data for part 1.\n", resultOne)
	fmt.Printf("There are %d valid triangles in the input data for part 2.\n", resultTwo)

	// Check for errors during scanning
	if err := scanner.Err(); err != nil {
		fmt.Println("Error reading file:", err)
		os.Exit(1)
	}
}
