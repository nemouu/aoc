#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define MAX_LINE_LENGTH 1024 // Define a maximum length for a line

// declare global variables for this version since we want to use recursion
int rows = 0;
int cols = 0;
char grid[1000][1000];      
long long cache[1000][1000];

// remove newline at the end of a string
void remove_newline(char* str) {
    int len = strlen(str);
    if(len > 0 && str[len-1] == '\n') {
        str[len-1] = '\0';
    }
}

// In your recursive function:
long long count_paths(int row, int col) {   
    // Check bounds first
    if(col < 0 || col >= cols) {
        return 0;
    } else if (row >= rows) {
        return 1;
    }
    
    // Check cache
    if(cache[row][col] != -1) {
        return cache[row][col];
    }
    
    // Calculate the result
    long long result;
    if(grid[row][col] == '^') {
        result = count_paths(row+1, col-1) + count_paths(row+1, col+1);
    } else {
        result = count_paths(row+1, col);
    }
    
    // Store in cache before returning
    cache[row][col] = result;
    return result;
}

// Main function
int main() {
    FILE *file_ptr;
    char line_buffer[MAX_LINE_LENGTH]; // Buffer to store each line
    long long result = 0;

    // Open the file in read mode ("r")
    file_ptr = fopen("input1.txt", "r");

    // Check if the file was opened successfully
    if (file_ptr == NULL) {
        perror("Error opening file"); // Print a system error message
        exit(EXIT_FAILURE);           // Exit with an error code
    }

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

    rewind(file_ptr);

    // Read the file line by line until the end of the file
    while (fgets(line_buffer, MAX_LINE_LENGTH, file_ptr) != NULL) {
        remove_newline(line_buffer);  // Remove \n first
        if (cols == 0) { cols = strlen(line_buffer); }
        for(int i = 0; i < cols; i++) {
            grid[rows][i] = line_buffer[i];
        }
        rows++;
    }

    // Initialize all to -1
    for(int i = 0; i < rows; i++) {
        for(int j = 0; j < cols; j++) {
            cache[i][j] = -1;
        }
    }

    // Now call the recursive function
    result = count_paths(0, start_position);

    // print outs for testing
    // printf("-----TEST-----\n\n");
    // for (int i = 0; i < rows; i++)
    // {
    //     for (int j = 0; j < cols; j++)
    //     { printf("%c", grid[i][j]); }
    //     printf("\n");
    // }
    
    // print result
    printf("\nResult: %lld\n\n", result);

    // Close the file
    fclose(file_ptr);
    return 0; // Indicate successful execution
}