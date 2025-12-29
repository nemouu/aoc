package main

import (
	"bufio"
	"fmt"
	"os"
)

func main() {
	// declare variables part 1
	var xCo int = 1
	var yCo int = 1
	var codeIndex int = 0
	var codeOne [5]int

	// matrix of numbers for part 1
	matrixOne := [][]int{
		{1, 2, 3},
		{4, 5, 6},
		{7, 8, 9},
	}

	// declare variables part 2
	var xCoTwo int = 1
	var yCoTwo int = 3
	var codeTwo [5]byte

	// matrix of characters for part 2
	matrixTwo := [][]byte{
		{'0', '0', '1', '0', '0'},
		{'0', '2', '3', '4', '0'},
		{'5', '6', '7', '8', '9'},
		{'0', 'A', 'B', 'C', '0'},
		{'0', '0', 'D', '0', '0'},
	}

	// read and parse the input
	file, err := os.Open("input2.txt")
	if err != nil {
		fmt.Println("Error opening file:", err)
		os.Exit(1)
	}
	defer file.Close()

	scanner := bufio.NewScanner(file)

	for scanner.Scan() {
		line := scanner.Text()

		// for part 1
		for i := 0; i < len(line); i++ {
			if line[i] == 'U' && yCo > 0 {
				yCo--
			} else if line[i] == 'D' && yCo < 2 {
				yCo++
			} else if line[i] == 'R' && xCo < 2 {
				xCo++
			} else if line[i] == 'L' && xCo > 0 {
				xCo--
			}
		}

		// Test printing
		// fmt.Printf("Coordinates for line %d are X: %d and Y: %d\n", codeIndex, xCo, yCo)

		// for part 2
		for i := 0; i < len(line); i++ {
			if line[i] == 'U' && yCoTwo > 0 && matrixTwo[xCoTwo][yCoTwo-1] != '0' {
				yCoTwo--
			} else if line[i] == 'D' && yCoTwo < 4 && matrixTwo[xCoTwo][yCoTwo+1] != '0' {
				yCoTwo++
			} else if line[i] == 'R' && xCoTwo < 4 && matrixTwo[xCoTwo+1][yCoTwo] != '0' {
				xCoTwo++
			} else if line[i] == 'L' && xCoTwo > 0 && matrixTwo[xCoTwo-1][yCoTwo] != '0' {
				xCoTwo--
			}
		}

		// Test printing
		// fmt.Printf("Coordinates for line %d are X: %d and Y: %d\n", codeIndex, xCoTwo, yCoTwo)

		codeOne[codeIndex] = matrixOne[yCo][xCo]
		codeTwo[codeIndex] = matrixTwo[yCoTwo][xCoTwo]
		codeIndex++
	}

	// print the results
	for i := range len(codeOne) {
		fmt.Printf("%d", codeOne[i])
	}
	fmt.Println()
	for i := range len(codeTwo) {
		fmt.Printf("%c", codeTwo[i])
	}
	fmt.Println()

	// Check for errors during scanning
	if err := scanner.Err(); err != nil {
		fmt.Println("Error reading file:", err)
		os.Exit(1)
	}
}
