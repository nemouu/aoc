package main

import (
	"bufio"
	"fmt"
	"os"
)

func main() {
	// declare variables
	/*var testStr1 string = "A(1x5)BC"
	var testStr2 string = "(6x1)(1x3)A"
	var testStr3 string = "A(2x2)BCD(2x2)EFG"
	var testStr4 string = "X(8x2)(3x3)ABCY"*/

	// read and parse the input
	file, err := os.Open("input9.txt")
	if err != nil {
		fmt.Println("Error opening file:", err)
		os.Exit(1)
	}
	defer file.Close()

	scanner := bufio.NewScanner(file)

	for scanner.Scan() {
		line := scanner.Text()
		fmt.Println(line)
	}

	// check for errors during scanning
	if err := scanner.Err(); err != nil {
		fmt.Println("Error reading file:", err)
		os.Exit(1)
	}
}
