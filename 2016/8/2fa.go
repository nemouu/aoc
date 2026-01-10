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
	var screen [6][50]bool
	var inOne int
	var inTwo int
	var result int

	// read and parse the input
	file, err := os.Open("input8.txt")
	if err != nil {
		fmt.Println("Error opening file:", err)
		os.Exit(1)
	}
	defer file.Close()

	scanner := bufio.NewScanner(file)

	for scanner.Scan() {
		line := scanner.Text()

		// deal with first command
		if strings.HasPrefix(line, "rect") {

			// read in values
			fmt.Sscanf(line, "rect %dx%d", &inOne, &inTwo)

			// looping over the space given by the values
			for i := 0; i < inTwo; i++ {
				for j := 0; j < inOne; j++ {
					screen[i][j] = true
				}
			}

			// deal with the column rotation
		} else if strings.HasPrefix(line, "rotate") {
			instructions := strings.Split(line, " ")
			if strings.HasPrefix(instructions[1], "column") {

				// read in values
				fmt.Sscanf(line, "rotate column x=%d by %d", &inOne, &inTwo)

				// turn column into a slice
				newCol := make([]bool, len(screen))
				for i := range len(screen) {
					newCol[i] = screen[i][inOne]
				}

				// get a rotated version of that slice
				newCol = rotateRight(newCol, inTwo)

				// write it back into the screen
				for i := range len(screen) {
					screen[i][inOne] = newCol[i]
				}

				// deal with the row rotation
			} else if strings.HasPrefix(instructions[1], "row") {

				// read in values
				fmt.Sscanf(line, "rotate row y=%d by %d", &inOne, &inTwo)

				// get a new row that is rotated by the right amount
				newRow := rotateRight(screen[inOne][:], inTwo)

				// write it back to the screen
				copy(screen[inOne][:], newRow)

			}
		}
	}

	// print screen and count result
	fmt.Printf("\nAfter swiping the keycard the screen looks like this:\n\n")
	for i := range len(screen) {
		for j := range len(screen[0]) {
			if screen[i][j] {
				fmt.Print("#")
				result++
			} else {
				fmt.Print(".")
			}
		}
		fmt.Println()
	}
	fmt.Printf("\nand %d pixels are lit.\n\n", result)

	// check for errors during scanning
	if err := scanner.Err(); err != nil {
		fmt.Println("Error reading file:", err)
		os.Exit(1)
	}
}
