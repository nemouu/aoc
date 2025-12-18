package main

import (
	"bufio"
	"fmt"
	"os"
)

type Coord struct {
	x_co int
	y_co int
}

func main() {
	// variables used
	// var test_input string = "^v^v^v^v^v"
	var santa_xpos int = 0
	var santa_ypos int = 0
	var ranta_xpos int = 0
	var ranta_ypos int = 0

	// Initialize map and add the starting point
	visited_houses := make(map[Coord]int)
	visited_houses[Coord{santa_xpos, santa_ypos}] = 1

	visited_houses_robo := make(map[Coord]int)
	visited_houses_robo[Coord{santa_xpos, santa_ypos}] = 1

	// Open the file
	file, err := os.Open("input3.txt")
	if err != nil {
		fmt.Println("Error opening file:", err)
		os.Exit(1)
	}
	defer file.Close() // Automatically closes when function exits

	// Create a scanner to read line by line
	scanner := bufio.NewScanner(file)

	// Read line by line now
	for scanner.Scan() {
		line := scanner.Text() // Get the current line as a string
		for i := 0; i < len(line); i++ {
			santa_char := line[i]

			// up and down will change the y position for santa
			if santa_char == '^' {
				santa_ypos++
			}
			if santa_char == 'v' {
				santa_ypos--
			}

			// right and left will change the x position for santa
			if santa_char == '>' {
				santa_xpos++
			}
			if santa_char == '<' {
				santa_xpos--
			}

			// if the current position is in map the amount of presents will be incremented by 1, if it has not been visited
			// the following expression returns the int 0 (default value for ints) and we add the position like this to the map
			// by incrementing that zero by 1
			visited_houses[Coord{santa_xpos, santa_ypos}]++
		}
	}

	// get the number of visited houses
	fmt.Printf("%d Houses have received at least one present.\n", len(visited_houses))

	// Check for error in the first scan
	if err := scanner.Err(); err != nil {
		fmt.Println("Error in first pass:", err)
		os.Exit(1)
	}

	// Rewind to beginning
	file.Seek(0, 0)
	scanner = bufio.NewScanner(file) // Create new scanner

	// reset the starting positions
	santa_xpos = 0
	santa_ypos = 0
	ranta_xpos = 0
	ranta_ypos = 0

	// Read line by line for the second time to do the second riddle for this day
	for scanner.Scan() {
		line := scanner.Text() // Get the current line as a string
		for i := 0; i < len(line)-1; i = i + 2 {
			santa_char := line[i]
			ranta_char := line[i+1]

			// up and down will change the y position for santa
			if santa_char == '^' {
				santa_ypos++
			}
			if santa_char == 'v' {
				santa_ypos--
			}

			// right and left will change the x position for santa
			if santa_char == '>' {
				santa_xpos++
			}
			if santa_char == '<' {
				santa_xpos--
			}

			// up and down will change the y position for robo santa
			if ranta_char == '^' {
				ranta_ypos++
			}
			if ranta_char == 'v' {
				ranta_ypos--
			}

			// right and left will change the x position for robo santa
			if ranta_char == '>' {
				ranta_xpos++
			}
			if ranta_char == '<' {
				ranta_xpos--
			}

			// if the current position is in map the amount of presents will be incremented by 1, if it has not been visited
			// the following expression returns the int 0 (default value for ints) and we add the position like this to the map
			// by incrementing that zero by 1
			visited_houses_robo[Coord{santa_xpos, santa_ypos}]++
			visited_houses_robo[Coord{ranta_xpos, ranta_ypos}]++
		}
	}

	// get the number of visited houses
	fmt.Printf("%d Houses have received at least one present with the help of robo santa.", len(visited_houses_robo))

	// Check for errors in the second scan
	if err := scanner.Err(); err != nil {
		fmt.Println("Error in second pass:", err)
		os.Exit(1)
	}
}
