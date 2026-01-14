package main

import (
	"fmt"
	"os"
	"strings"
)

// try and get a fully decompressed substring
func decrompStr(inStr string, result int) int {
	for i := 0; i < len(inStr); i++ {
		if inStr[i] == '(' {
			// get two int variables to read in the numbers
			inOne := 0
			inTwo := 0

			// first we count how long the current marker is since
			// we have to add this later
			markerLen := 0
			for _, c := range inStr[i:] {
				if c == ')' {
					break
				} else {
					markerLen++
				}
			}

			// scan for the operands of the current marker
			fmt.Sscanf(inStr[i:], "(%dx%d)", &inOne, &inTwo)

			// we add the length determined by the marker to the result
			result = result + (decrompStr(inStr[i+markerLen+1:i+markerLen+1+inOne], 0) * inTwo)

			// move position as determined by the marker
			i = i + markerLen + inOne
		} else { // normal character, count as length 1
			result++
		}
	}
	return result
}

func main() {
	// declare variables
	var inOne int
	var inTwo int
	var result int

	// read and parse the input (different here because we only have to read one line)
	content, err := os.ReadFile("input9.txt")
	if err != nil {
		fmt.Println("Error reading file:", err)
		os.Exit(1)
	}
	// Convert bytes to string and remove any surrounding whitespace
	line := strings.TrimSpace(string(content))

	// we use this iterative method for part 1
	for i := 0; i < len(line); i++ {
		if line[i] == '(' {
			// first we count how long the current marker is since
			// we have to add this later
			markerLen := 0
			for _, c := range line[i:] {
				if c == ')' {
					break
				} else {
					markerLen++
				}
			}

			// scan for the operands of the current marker
			fmt.Sscanf(line[i:], "(%dx%d)", &inOne, &inTwo)

			// we add the length determined by the marker to the result
			result = result + (inOne * inTwo)

			// move position as determined by the marker
			i = i + markerLen + inOne
		} else { // normal character, count as length 1
			result++
		}
	}

	// for part 2 we use the above recursive function
	result2 := decrompStr(line, 0)

	// print the result
	fmt.Println()
	fmt.Printf("The decrompressed length of the file is %d (part 1)\n", result)
	fmt.Printf("The full decrompressed length of the file is %d (part 2)\n\n", result2)
}
