package main

import (
	"crypto/md5"
	"encoding/hex"
	"fmt"
	"strings"
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

func main() {
	// declare variables part 1
	var curr_try int = 1
	var curr_hash_input string
	var input string = "reyedfim"
	//var inputTest string = "abc"
	var pw strings.Builder

	// check for 5 leading zeros 8 times -> Part 1
	for i := 1; i < 9; i++ {
		for {
			// assemble the current iterations input from the string input and the current number
			// (start with 1 and then make it bigger to get the minimum)
			curr_hash_input = fmt.Sprintf("%s%d", input, curr_try)

			// Check if the MD5 hash starts with 5 zeros and stop if it does
			currHash := GetMD5Hash(curr_hash_input)
			if BeginsWith5Zeros(currHash) {
				fmt.Println(currHash)
				pw.WriteRune(rune(currHash[5])) // write the current 6th letter of the hash to the password(pw)
				curr_try++
				break
			}

			// move to the next hash input
			curr_try++
		}
	}

	fmt.Printf("\nThe password for part 1 is: ")
	fmt.Println(pw.String())
	fmt.Println()

	// declare/reset variables fpr part 2
	decrypOut := make([]byte, 8) // use a slice for part 2
	curr_try = 1                 // reset the try counter
	counter := 1                 // counter for the number of full positions in the slice

	// check for 5 leading zeros 8 times -> Part 2
	for {
		if counter > 8 {
			break
		}
		for {
			// assemble the current iterations input from the string input and the current number
			// (start with 1 and then make it bigger to get the minimum)
			curr_hash_input = fmt.Sprintf("%s%d", input, curr_try)

			// Check if the MD5 hash starts with 5 zeros and stop if it does
			currHash := GetMD5Hash(curr_hash_input)
			if BeginsWith5Zeros(currHash) {

				// save the current values for position and value
				currPos := currHash[5]
				currVal := currHash[6]

				// add the value at the position if the position is empty and the position is valid
				if currPos >= '0' && currPos <= '7' && decrypOut[currPos-'0'] == 0 {
					decrypOut[currPos-'0'] = currVal
					counter++ // increase char counter if a new character was added
				} else {
					curr_try++
					break
				}
			}

			// move to the next hash input
			curr_try++
		}
	}

	// print result
	fmt.Printf("The password for part 2 is: ")
	fmt.Println(string(decrypOut))
}
