package main

import (
	"bufio"
	"fmt"
	"os"
	"strings"
)

func rotateRight(inSlice []bool, inAmount int) []bool {
	b := inAmount
	n := len(inSlice)
	rotated := append(inSlice[n-b:], inSlice[:n-b]...)
	return rotated
}

func main() {
	// declare variables
	var screen [3][7]bool
	var inOne int
	var inTwo int

	// read and parse the input
	file, err := os.Open("inputtest.txt")
	if err != nil {
		fmt.Println("Error opening file:", err)
		os.Exit(1)
	}
	defer file.Close()

	scanner := bufio.NewScanner(file)

	for scanner.Scan() {
		line := scanner.Text()
		if strings.HasPrefix(line, "rect") {

			// read in values
			fmt.Sscanf(line, "rect %dx%d", &inOne, &inTwo)

			// test printing
			fmt.Printf("Line '%s' has: rect %d xxx %d\n", line, inOne, inTwo)

			// looping over the space given by the values
			for i := 0; i < inTwo; i++ {
				for j := 0; j < inOne; j++ {
					screen[i][j] = true
				}
			}

		} else if strings.HasPrefix(line, "rotate") {
			instructions := strings.Split(line, " ")
			if strings.HasPrefix(instructions[1], "column") {

				// read in values
				fmt.Sscanf(line, "rotate column x=%d by %d", &inOne, &inTwo)

				// test printing
				fmt.Printf("Line '%s' has: rotate column %d byby %d\n", line, inOne, inTwo)

				// get a new row that is rotated by the right amount
				newRow := rotateRight(screen[inOne][:], inTwo)

				// write it back to the screen
				copy(screen[inOne][:], newRow)

			} else if strings.HasPrefix(instructions[1], "row") {

				// read in values
				fmt.Sscanf(line, "rotate row y=%d by %d", &inOne, &inTwo)

				// test printing
				fmt.Printf("Line '%s' has: rotate row %d byby %d\n", line, inOne, inTwo)
			}
		}
	}

	// print screen
	fmt.Println()
	for i := range len(screen) {
		for j := range len(screen[0]) {
			if screen[i][j] {
				fmt.Print("#")
			} else {
				fmt.Print(".")
			}
		}
		fmt.Println()
	}

	// check for errors during scanning
	if err := scanner.Err(); err != nil {
		fmt.Println("Error reading file:", err)
		os.Exit(1)
	}
}
