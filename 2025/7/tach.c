#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define MAX_LINE_LENGTH 1024 // Define a maximum length for a line

// remove newline at the end of a string
void remove_newline(char* str) {
    int len = strlen(str);
    if(len > 0 && str[len-1] == '\n') {
        str[len-1] = '\0';
    }
}

// Function to check if position is already in beam array
int contains(int beam_array[], int beam_count, int position) {
    for(int i = 0; i < beam_count; i++) {
        if(beam_array[i] == position) {
            return 1;  // Found it
        }
    }
    return 0;  // Not found
}

// Add beam only if not already present
void add_beam(int beam_array[], int* beam_count, int position) {
    if(!contains(beam_array, *beam_count, position)) {
        beam_array[(*beam_count)++] = position;
    }
}

// Main function
int main() {
    FILE *file_ptr;
    char line_buffer[MAX_LINE_LENGTH]; // Buffer to store each line
    long long result = 0;
    
    // we want to save the beams in an array
    int beams_a[100];
    int beams_b[100];
    int* current_beams = beams_a;
    int* next_beams = beams_b;
    int current_count = 1;
    int next_count = 0;

    // Open the file in read mode ("r")
    file_ptr = fopen("input1.txt", "r");

    // Check if the file was opened successfully
    if (file_ptr == NULL) {
        perror("Error opening file"); // Print a system error message
        exit(EXIT_FAILURE);           // Exit with an error code
    }

    // Initialize first beam at S position but this is hardcoded
    // current_beams[0] = 7;

    // We read the first line first and then continue after
    fgets(line_buffer, MAX_LINE_LENGTH, file_ptr);
    remove_newline(line_buffer);

    int start_position = -1;
    for(int i = 0; i < strlen(line_buffer); i++) {
        if(line_buffer[i] == 'S') {
            start_position = i;
            break;
        }
    }

    // Initialize beams at start position
    current_beams[0] = start_position;
    current_count = 1;

    // Read the file line by line until the end of the file
    while (fgets(line_buffer, MAX_LINE_LENGTH, file_ptr) != NULL) {
        remove_newline(line_buffer);
        
        int has_splits = 0;  // Track if anything changed
        next_count = 0;
        
        for(int i = 0; i < current_count; i++) {
            int beam_pos = current_beams[i];
            
            if(line_buffer[beam_pos] == '^') {
                // Hit a splitter!
                has_splits = 1;
                result++;
                add_beam(next_beams, &next_count, beam_pos - 1);
                add_beam(next_beams, &next_count, beam_pos + 1);
            } else {
                // Beam continues
                add_beam(next_beams, &next_count, beam_pos);
            }
        }
        
        // Swap arrays
        int* temp = current_beams;
        current_beams = next_beams;
        next_beams = temp;
        current_count = next_count;
    }

    result = (result * 2 ) - 2;

    printf("\nResult: %lld\n\n", result);

    // Close the file
    fclose(file_ptr);
    return 0; // Indicate successful execution
}