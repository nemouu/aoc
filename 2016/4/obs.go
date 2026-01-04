package main

import (
	"bufio"
	"fmt"
	"os"
	"sort"
	"strconv"
	"strings"
)

func main() {
	// declare variables
	var name string
	var num string
	var checksum string
	var lastIndexHyphen int
	var firstIndexBracket int
	var result int

	// read and parse the input
	file, err := os.Open("input4.txt")
	if err != nil {
		fmt.Println("Error opening file:", err)
		os.Exit(1)
	}
	defer file.Close()

	scanner := bufio.NewScanner(file)

	for scanner.Scan() {
		line := scanner.Text()

		// get index of last hyphen and first bracket
		lastIndexHyphen = strings.LastIndex(line, "-")
		firstIndexBracket = strings.Index(line, "[")

		if lastIndexHyphen != -1 {
			// Capture everything up to and including the last hyphen (current name)
			name = line[:lastIndexHyphen]

			// Capture everything after the hyphen (current number)
			num = line[lastIndexHyphen+1 : firstIndexBracket]

			// Capture everything after the first bracket (current checksum)
			checksum = line[firstIndexBracket+1 : len(line)-1]

		}

		// We use a map to save the letter count
		letterCount := make(map[rune]int)
		for _, char := range name {
			if char != '-' {
				letterCount[char]++
			}
		}

		// create a slice of letters to sort
		var letters []rune
		for letter := range letterCount {
			letters = append(letters, letter)
		}

		// Sort by frequency (descending), then alphabetically (ascending)
		sort.Slice(letters, func(i, j int) bool {
			// If frequencies are different, sort by frequency (higher first)
			if letterCount[letters[i]] != letterCount[letters[j]] {
				return letterCount[letters[i]] > letterCount[letters[j]]
			}
			// If frequencies are the same, sort alphabetically
			return letters[i] < letters[j]
		})

		// build the expected checksum from the top 5 letters
		expectedChecksum := ""
		for i := 0; i < 5 && i < len(letters); i++ {
			expectedChecksum += string(letters[i])
		}

		// Check if the expected checksum matches the actual checksum and check for part 2
		if expectedChecksum == checksum {
			nr, err := strconv.Atoi(num)
			if err != nil {
				fmt.Println("Error during conversion!")
			} else {
				result += nr
			}

			// Now for part 2 we want to look through the valid rooms to find the ID of the northpole object storage
			shiftNum := nr % 26 // since the rotation wraps around we can optimize
			var decrypted strings.Builder

			for _, char := range name {
				if char == '-' {
					decrypted.WriteRune(' ') // Add a space for hyphens
				} else {
					position := int(char - 'a')
					newPosition := (position + shiftNum) % 26
					newChar := rune(newPosition) + 'a'
					decrypted.WriteRune(newChar) // Add the rotated character
				}
			}

			// At the end, get the final string and print the result for part 2
			resultStr := decrypted.String()
			if strings.Contains(resultStr, "north") {
				fmt.Printf("'%s' has the number: %d\n", resultStr, nr)
			}
		}
	}

	// print the result for part 1
	fmt.Printf("The result is: %d", result)

	// Check for errors during scanning
	if err := scanner.Err(); err != nil {
		fmt.Println("Error reading file:", err)
		os.Exit(1)
	}
}
