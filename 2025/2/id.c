#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define MAX_LINE_LENGTH 1024 // Define a maximum length for a line

// main function that will determine of an id is valid. Result 1 will mean true and 0 will mean false
int is_valid(long long input) {
    // Count digits without log
    long long temp = input;
    int length = 0;
    while (temp > 0) {
        length++;
        temp /= 10;
    }
    
    // Try all possible pattern lengths (from 1 to length/2)
    for (int pattern_len = 1; pattern_len <= length / 2; pattern_len++) {
        // Check if length is divisible by pattern_len
        if (length % pattern_len != 0) {
            continue;
        }
        
        // Extract the pattern (first pattern_len digits)
        int pattern[pattern_len];
        long long temp_input = input;
        int input_arr[length];
        
        // Build array of all digits (reversed)
        for (int i = 0; i < length; i++) {
            input_arr[i] = temp_input % 10;
            temp_input /= 10;
        }
        
        // Check if all segments match the first segment
        int is_repeating = 1;
        for (int i = 0; i < length; i++) {
            // Compare with the corresponding position in the first pattern
            int pattern_pos = i % pattern_len;
            int first_segment_pos = length - pattern_len + pattern_pos;
            
            if (input_arr[i] != input_arr[first_segment_pos]) {
                is_repeating = 0;
                break;
            }
        }
        
        if (is_repeating) {
            return 0; // Invalid (pattern repeats)
        }
    }
    
    return 1; // Valid (no repeating pattern found)
}

// Main function
int main() {
    FILE *file_ptr;
    char line_buffer[MAX_LINE_LENGTH]; // Buffer to store each line
    long long beginning_id;
    long long end_id;
    long long result = 0;

    // for testing (example input)
    char line_test[MAX_LINE_LENGTH]  = "11-22,95-115,998-1012,1188511880-1188511890,222220-222224,1698522-1698528,446443-446449,38593856-38593862,565653-565659,824824821-824824827,2121212118-2121212124";

    // Open the file in read mode ("r")
    file_ptr = fopen("input1.txt", "r");

    // Check if the file was opened successfully
    if (file_ptr == NULL) {
        perror("Error opening file"); // Print a system error message
        exit(EXIT_FAILURE);           // Exit with an error code
    }

    // Read the file 
    fgets(line_buffer, MAX_LINE_LENGTH, file_ptr);

    // The character that separates the id ranges
    char *d = ",";

    // get the single id ranges as substrings
    char *current_sub_string = strtok(line_buffer, d);  
    while (current_sub_string != NULL) {

        // read in and print the current numbers in the substring, or in other words, the range beginning and end
        sscanf(current_sub_string, "%lld-%lld", &beginning_id, &end_id);
        
        for (long long i = beginning_id; i < end_id + 1; i++)
        {
            if (is_valid(i) == 0)
            {
                result += i;
            }
        }
        // Get the next substring
        current_sub_string = strtok(NULL, d);  
    }

    // print the result
    printf("\nThe resulting sum is: %lld\n\n", result);

    // Close the file
    fclose(file_ptr);

    return 0; // Indicate successful execution
}
