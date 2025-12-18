package main

import (
	"bufio"
	"fmt"
	"os"
)

// function that finds the command mode
func getOpCmd(inStr string) string {
	var firstCmd string
	var secCmd string
	fmt.Sscanf(inStr, "%s %s", &firstCmd, &secCmd)
	if firstCmd == "toggle" {
		return firstCmd
	} else {
		return secCmd
	}
}

// function that finds the coordinates based on the command mode
func getCoord(inStr string, mode string) (int, int, int, int) {
	var start_x, start_y, end_x, end_y int

	switch mode {
	case "on":
		fmt.Sscanf(inStr, "turn on %d,%d through %d,%d", &start_x, &start_y, &end_x, &end_y)
	case "off":
		fmt.Sscanf(inStr, "turn off %d,%d through %d,%d", &start_x, &start_y, &end_x, &end_y)
	default:
		fmt.Sscanf(inStr, "toggle %d,%d through %d,%d", &start_x, &start_y, &end_x, &end_y)
	}
	return start_x, start_y, end_x, end_y
}

// Function that turns on/off or toggles the light based on the given input
func operateLights(inGrid *[1000][1000]int, in_start_x, in_start_y, in_end_x, in_end_y int, mode string) {
	switch mode {
	case "on":
		for i := in_start_x; i <= in_end_x; i++ {
			for j := in_start_y; j <= in_end_y; j++ {
				inGrid[i][j] = 1
			}
		}
	case "off":
		for i := in_start_x; i <= in_end_x; i++ {
			for j := in_start_y; j <= in_end_y; j++ {
				inGrid[i][j] = 0
			}
		}
	default:
		for i := in_start_x; i <= in_end_x; i++ {
			for j := in_start_y; j <= in_end_y; j++ {
				inGrid[i][j] = (inGrid[i][j] + 1) % 2
			}
		}
	}
}

// Function that operates the brightness of the lights based on the given input
func operateLightsBrightness(inGrid *[1000][1000]int, in_start_x, in_start_y, in_end_x, in_end_y int, mode string) {
	switch mode {
	case "on":
		for i := in_start_x; i <= in_end_x; i++ {
			for j := in_start_y; j <= in_end_y; j++ {
				inGrid[i][j]++
			}
		}
	case "off":
		for i := in_start_x; i <= in_end_x; i++ {
			for j := in_start_y; j <= in_end_y; j++ {
				if inGrid[i][j] > 0 {
					inGrid[i][j]--
				}
			}
		}
	default:
		for i := in_start_x; i <= in_end_x; i++ {
			for j := in_start_y; j <= in_end_y; j++ {
				inGrid[i][j] += 2
			}
		}
	}
}

func main() {
	// variables used
	var result int = 0
	var result_brightness int = 0
	var light_grid [1000][1000]int
	var brightness_grid [1000][1000]int
	var op_str string
	var start_x, start_y, end_x, end_y int

	// Open the file
	file, err := os.Open("input6.txt")
	if err != nil {
		fmt.Println("Error opening file:", err)
		os.Exit(1)
	}
	defer file.Close() // Automatically closes when function exits

	// Create a scanner to read line by line
	scanner := bufio.NewScanner(file)

	for scanner.Scan() {
		line := scanner.Text() // Get the current line as a string

		// scan for the current command
		op_str = getOpCmd(line)

		// scan for the current coordinates
		start_x, start_y, end_x, end_y = getCoord(line, op_str)

		// operate the lights based on current command and coordinates (both switching and brightness from part 1 and 2)
		operateLights(&light_grid, start_x, start_y, end_x, end_y, op_str)
		operateLightsBrightness(&brightness_grid, start_x, start_y, end_x, end_y, op_str)
	}

	// Count the lights in the grid that were turned on
	for _, row := range light_grid {
		for _, value := range row {
			result += value
		}
	}

	// Count the brightness of the lights in the brightness grid
	for _, row := range brightness_grid {
		for _, value := range row {
			result_brightness += value
		}
	}

	// Print the results
	fmt.Printf("The number of lit lights is: %d\n", result)
	fmt.Printf("The total brightness of the lights is: %d\n", result_brightness)

	// Check for errors during scanning
	if err := scanner.Err(); err != nil {
		fmt.Println("Error reading file:", err)
		os.Exit(1)
	}
}
