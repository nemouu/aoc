package main

import (
	"bufio"
	"fmt"
	"os"
	"strings"
)

type Coord struct {
	x, y int
}

func walkToEnd(instructions []string) Coord {
	pos := Coord{0, 0}
	direction := 0 // 0=North, 1=East, 2=South, 3=West

	for _, instruction := range instructions {
		// Parse the instruction
		turn := instruction[0]
		var distance int
		fmt.Sscanf(instruction[1:], "%d", &distance)

		// Update direction based on turn
		if turn == 'R' {
			direction = (direction + 1) % 4
		} else {
			direction = (direction - 1 + 4) % 4
		}

		// Move in the current direction
		pos = moveInDirection(pos, direction, distance)
	}

	return pos
}

func findFirstRevisit(instructions []string) (Coord, bool) {
	pos := Coord{0, 0}
	direction := 0
	visited := make(map[Coord]bool)
	visited[pos] = true // mark starting position as visited

	for _, instruction := range instructions {
		// parse the instruction
		turn := instruction[0]
		var distance int
		fmt.Sscanf(instruction[1:], "%d", &distance)

		// update direction
		if turn == 'R' {
			direction = (direction + 1) % 4
		} else {
			direction = (direction - 1 + 4) % 4
		}

		// move one block at a time, checking for revisits
		for step := 0; step < distance; step++ {
			pos = moveInDirection(pos, direction, 1)

			if visited[pos] {
				// found the first location we visit twice!
				return pos, true
			}
			visited[pos] = true
		}
	}

	return Coord{0, 0}, false
}

func moveInDirection(pos Coord, direction int, distance int) Coord {
	// calculate the change in position based on direction
	// north increases y, East increases x, South decreases y, West decreases x
	switch direction {
	case 0: // North
		pos.y += distance
	case 1: // East
		pos.x += distance
	case 2: // South
		pos.y -= distance
	case 3: // West
		pos.x -= distance
	}
	return pos
}

func abs(x int) int {
	if x < 0 {
		return -x
	}
	return x
}

func main() {
	// read and parse the input
	file, err := os.Open("input1.txt")
	if err != nil {
		fmt.Println("Error opening file:", err)
		os.Exit(1)
	}
	defer file.Close()

	scanner := bufio.NewScanner(file)
	var instructions []string

	for scanner.Scan() {
		line := scanner.Text()
		// split on comma and space, trim whitespace
		parts := strings.Split(line, ", ")
		instructions = append(instructions, parts...)
	}

	// part 1: Find final distance
	finalPos := walkToEnd(instructions)
	distance1 := abs(finalPos.x) + abs(finalPos.y)
	fmt.Printf("Part 1: %d blocks away\n", distance1)

	// part 2: Find first revisited location
	revisitedPos, found := findFirstRevisit(instructions)
	if found {
		distance2 := abs(revisitedPos.x) + abs(revisitedPos.y)
		fmt.Printf("Part 2: %d blocks away\n", distance2)
	}
}
