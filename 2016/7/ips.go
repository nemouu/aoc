package main

import (
	"bufio"
	"fmt"
	"os"
	"slices"
	"strings"
)

func containsABBA(inStr string) bool {
	for i := 0; i < len(inStr)-3; i++ {
		if inStr[i] == inStr[i+3] && inStr[i+1] == inStr[i+2] && inStr[i] != inStr[i+1] {
			return true
		}
	}
	return false
}

func parseIP(line string) (outside, inside []string) {
	currentPos := 0

	for {
		start := strings.Index(line[currentPos:], "[")
		if start == -1 {
			if currentPos < len(line) {
				outside = append(outside, line[currentPos:])
			}
			break
		}

		start += currentPos
		if start > currentPos {
			outside = append(outside, line[currentPos:start])
		}

		end := strings.Index(line[start:], "]")
		if end == -1 {
			break
		}
		end += start

		inside = append(inside, line[start+1:end])
		currentPos = end + 1
	}
	return
}

func main() {
	// declare variables
	var resultTLS int
	var resultSSL int

	// read and parse the input
	file, err := os.Open("input7.txt")
	if err != nil {
		fmt.Println("Error opening file:", err)
		os.Exit(1)
	}
	defer file.Close()

	scanner := bufio.NewScanner(file)

	for scanner.Scan() {
		line := scanner.Text()

		// Parse the IP address into its components
		outside, inside := parseIP(line)

		// part 1: Check for TLS support - check if it has at least one ABBA outside brackets
		// 			and no ABBAs inside brackets
		if slices.ContainsFunc(outside, containsABBA) && !slices.ContainsFunc(inside, containsABBA) {
			resultTLS++
		}

		// part 2: Check for SSL support - check if it has at least one ABA outside brackets
		// 			and at least one corresponding BAB inside brackets
		foundSSL := false
		for _, outWord := range outside {
			if foundSSL {
				break
			}
			for i := 0; i < len(outWord)-2; i++ {
				if outWord[i] == outWord[i+2] && outWord[i+1] != outWord[i] {
					// Build the corresponding BAB
					bab := string([]byte{outWord[i+1], outWord[i], outWord[i+1]})

					// Check if any inside section contains this BAB
					for _, inWord := range inside {
						if strings.Contains(inWord, bab) {
							resultSSL++
							foundSSL = true
							break
						}
					}
					if foundSSL {
						break
					}
				}
			}
		}
	}

	// print results
	fmt.Printf("There are %d IP addresses that support TLS.\n", resultTLS)
	fmt.Printf("And there are %d IP addresses that support SSL.\n", resultSSL)

	// check for errors during scanning
	if err := scanner.Err(); err != nil {
		fmt.Println("Error reading file:", err)
		os.Exit(1)
	}
}
