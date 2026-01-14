package main

import (
	"bufio"
	"fmt"
	"os"
)

func main() {
	// declare variables

	// read and parse the input
	file, err := os.Open("input1.txt")
	if err != nil {
		fmt.Println("Error opening file:", err)
		os.Exit(1)
	}
	defer file.Close()

	scanner := bufio.NewScanner(file)

	for scanner.Scan() {
		line := scanner.Text()

		// check the whole number for part 1
		result := 0
		for i := 0; i < len(line)-1; i++ {
			if line[i] == line[i+1] {
				result = result + int(line[i]-'0')
			}
		}

		// check the end of the number seperatly
		if line[0] == line[len(line)-1] {
			result = result + int(line[0]-'0')
		}

		// print result for part 1
		fmt.Printf("The result for part 1 is: %d\n", result)

		// check the whole number for part 2
		result2 := 0
		for i := 0; i < len(line)/2; i++ {
			if line[i] == line[i+(len(line)/2)] {
				result2 = result2 + 2*int(line[i]-'0')
			}
		}

		// print result for part 2
		fmt.Printf("The result for part 2 is: %d\n\n", result2)
	}

	// check for errors during scanning
	if err := scanner.Err(); err != nil {
		fmt.Println("Error reading file:", err)
		os.Exit(1)
	}
}
