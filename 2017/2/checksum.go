package main

import (
	"bufio"
	"fmt"
	"os"
	"strconv"
	"strings"
)

func main() {
	// declare variables
	var resultOne int
	var resultTwo int

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

		// get rid of the white space in the line
		numLine := strings.Fields(line)

		// initialize minimum and maximum
		min, _ := strconv.Atoi(numLine[0])
		max, _ := strconv.Atoi(numLine[0])

		// iterate over the line and find minimum and maximum
		for i := 0; i < len(numLine); i++ {
			curr, _ := strconv.Atoi(numLine[i])

			// get the two numbers for part 2
			for j := i + 1; j < len(numLine); j++ {
				currTwo, _ := strconv.Atoi(numLine[j])
				if curr%currTwo == 0 {
					resultTwo += curr / currTwo
				} else if currTwo%curr == 0 {
					resultTwo += currTwo / curr
				}
			}

			// get min and max for part 1
			if curr > max {
				max = curr
			}
			if curr < min {
				min = curr
			}
		}
		resultOne += max - min
	}

	// print results
	fmt.Printf("The checksum for part 1 is: %d\n", resultOne)
	fmt.Printf("and the checksum for part 2 is: %d\n", resultTwo)

	// check for errors during scanning
	if err := scanner.Err(); err != nil {
		fmt.Println("Error reading file:", err)
		os.Exit(1)
	}
}
