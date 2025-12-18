package main

import (
	"crypto/md5"
	"encoding/hex"
	"fmt"
)

// function that calculates the md5 hash from an input string
func GetMD5Hash(text string) string {
	hash := md5.Sum([]byte(text))
	return hex.EncodeToString(hash[:])
}

// function that checks whether an input string starts with 5 zeros
func BeginsWith5Zeros(input string) bool {
	if input[:5] == "00000" {
		return true
	} else {
		return false
	}
}

// function that checks whether an input string starts with 5 zeros
func BeginsWith6Zeros(input string) bool {
	if input[:6] == "000000" {
		return true
	} else {
		return false
	}
}

func main() {
	// variables used
	var result int = 0
	var curr_try int = 1
	var curr_hash_input string
	var input string = "iwrupvqb"

	// check for 5 leading zeros -> Part 1
	for {
		// assemble the current iterations input from the string input and the current number
		// (start with 1 and then make it bigger to get the minimum)
		curr_hash_input = fmt.Sprintf("%s%d", input, curr_try)

		// Check if the MD5 hash starts with 5 zeros and stop if it does
		if BeginsWith5Zeros(GetMD5Hash(curr_hash_input)) {
			result = curr_try
			break
		}

		// continue searching otherwise
		curr_try++
	}

	// Print the result
	fmt.Printf("The lowest number that creates a MD5 hash with 5 leading zeros is: %d\n", result)

	// Reset variables
	result = 0
	curr_try = 1

	// check for 6 leading zeros -> Part 2
	for {
		// assemble the current iterations input from the string input and the current number
		// (start with 1 and then make it bigger to get the minimum)
		curr_hash_input = fmt.Sprintf("%s%d", input, curr_try)

		// Check if the MD5 hash starts with 5 zeros and stop if it does
		if BeginsWith6Zeros(GetMD5Hash(curr_hash_input)) {
			result = curr_try
			break
		}

		// continue searching otherwise
		curr_try++
	}

	// Print the result
	fmt.Printf("The lowest number that creates a MD5 hash with 6 leading zeros is: %d\n", result)
}
