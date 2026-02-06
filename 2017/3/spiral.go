package main

import (
	"fmt"
)

func ringNumber(inNum int) (int, int) {
	currRing := 1
	currSquare := 1
	for i := 3; i*i <= inNum; i += 2 {
		currRing++
		currSquare = i * i
	}
	return currRing, currSquare
}

func ringSideLen(inNum int) int {
	return (inNum * 2) + 1
}

func ringMidPoints(inRing int, inStartSquare int) (int, int, int, int) {
	one := inRing + inStartSquare
	two := one + inRing*2
	three := two + inRing*2
	four := three + inRing*2
	return one, two, three, four
}

func absValue(inNum int) int {
	if inNum < 0 {
		return inNum * (-1)
	} else {
		return inNum
	}
}

func main() {
	// declare variables
	var input int = 347991
	//var tryNum int = 1024

	// for part 1 we try to find a mathematical solution
	ring, square := ringNumber(input)
	one, two, three, four := ringMidPoints(ring, square)
	midPs := [4]int{one, two, three, four}
	minDis := ringSideLen(input)

	for i := range len(midPs) {
		if absValue(midPs[i]-input) < minDis {
			minDis = absValue(midPs[i] - input)
		}
	}

	// print result for part 1
	fmt.Printf("For part 1: The distance to the center is %d.\n", minDis+ring)

	// for part 2 we try to simulate the spiral
	x, y := 0, 0  // current position
	dirIndex := 0 // which direction are we looking at (0=right, 1=up, 2=left, 3=down)
	directions := [][2]int{{1, 0}, {0, -1}, {-1, 0}, {0, 1}}
	spiral := make(map[[2]int]int)
	spiral[[2]int{0, 0}] = 1

	for {
		// figure out what's to our left
		leftDir := (dirIndex + 1) % 4
		leftX := x + directions[leftDir][0]
		leftY := y + directions[leftDir][1]

		// is left empty? If so, turn left
		if spiral[[2]int{leftX, leftY}] == 0 {
			dirIndex = leftDir
		}

		// now move one step in our current direction
		x += directions[dirIndex][0]
		y += directions[dirIndex][1]

		// calculate sum of all eight neighbors
		sum := 0
		for dx := -1; dx <= 1; dx++ {
			for dy := -1; dy <= 1; dy++ {
				if dx == 0 && dy == 0 {
					continue // skip the current position itself
				}
				sum += spiral[[2]int{x + dx, y + dy}]
			}
		}

		// store the sum at current position
		spiral[[2]int{x, y}] = sum

		// check if we're done
		if sum > input {
			fmt.Println("For part 2: The answer is:", sum)
			break
		}
	}
}
