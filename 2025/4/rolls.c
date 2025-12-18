#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define MAX_LINE_LENGTH 1024 // Define a maximum length for a line

// Return length of a string while considering special end of line characters
int length_of_str(char in_str[]) {
    int len = strlen(in_str);
    if(len > 0 && in_str[len-1] == '\n') {
        in_str[len-1] = '\0';
        len--;
    }
    return len;
}

// Read file into 2D matrix
char** read_matrix(const char* filename, int* rows, int* cols) {
    FILE *file_ptr = fopen(filename, "r");
    if (file_ptr == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }
    
    char line_buffer[MAX_LINE_LENGTH];
    *rows = 0;
    *cols = 0;
    
    // Count dimensions
    while (fgets(line_buffer, MAX_LINE_LENGTH, file_ptr) != NULL) {
        if (*rows == 0) {
            *cols = length_of_str(line_buffer);
        }
        (*rows)++;
    }
    
    // Allocate matrix
    char** matrix = malloc(*rows * sizeof(char*));
    for(int i = 0; i < *rows; i++) {
        matrix[i] = malloc(*cols * sizeof(char));
    }
    
    // Fill matrix
    rewind(file_ptr);
    int row = 0;
    while (fgets(line_buffer, MAX_LINE_LENGTH, file_ptr) != NULL) {
        int len = length_of_str(line_buffer);
        memcpy(matrix[row], line_buffer, len);
        row++;
    }
    
    fclose(file_ptr);
    return matrix;
}

// Free matrix memory
void free_matrix(char** matrix, int rows) {
    for(int i = 0; i < rows; i++) {
        free(matrix[i]);
    }
    free(matrix);
}

// Count neighbors around a position
int count_neighbors(char** matrix, int rows, int cols, int i, int j, int layer) {
    int count = 0;
    for (int m = i - layer; m <= i + layer; m++) {
        for (int n = j - layer; n <= j + layer; n++) {
            // Check bounds and skip center
            if (m >= 0 && n >= 0 && m < rows && n < cols && !(m == i && n == j)) {
                if (matrix[m][n] == '@') {
                    count++;
                }
            }
        }
    }
    return count;
}

// Simulate one round, return number of removals
int simulate_round(char** matrix, int rows, int cols, int layer) {
    int removals = 0;
    
    for(int i = 0; i < rows; i++) {
        for(int j = 0; j < cols; j++) {
            if (matrix[i][j] == '@') {
                int neighbors = count_neighbors(matrix, rows, cols, i, j, layer);
                if (neighbors < 4) {
                    matrix[i][j] = '.';
                    removals++;
                }
            }
        }
    }
    
    return removals;
}

// Main function
int main() {
    int rows, cols;
    char** matrix = read_matrix("input1.txt", &rows, &cols);
    
    int result = 0;
    int layer = 1;
    int removals;
    
    // Run simulation until no more changes
    do {
        removals = simulate_round(matrix, rows, cols, layer);
        result += removals;
    } while (removals > 0);
    
    printf("Number of rolls that can be accessed by a forklift: %d\n", result);
    
    free_matrix(matrix, rows);
    return 0;
}
