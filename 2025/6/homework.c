#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define MAX_LINE_LENGTH 4096 // Define a maximum length for a line
#define DELIMITERS " "
#define MAX_ROWS 1000
#define MAX_COLS 1000
#define MAX_STR_LEN 50

// Function that removes newline from input string
void remove_newline(char* str) {
    int len = strlen(str);
    if(len > 0 && str[len-1] == '\n') {
        str[len-1] = '\0';
    }
}

// Main function
int main() {
    FILE *file_ptr;
    char line_buffer[MAX_LINE_LENGTH]; // Buffer to store each line

    // Open the file in read mode ("r")
    file_ptr = fopen("inputtest.txt", "r");

    // Check if the file was opened successfully
    if (file_ptr == NULL) {
        perror("Error opening file"); // Print a system error message
        exit(EXIT_FAILURE);           // Exit with an error code
    }

    // initialize runtime variables
    long long result = 0;
    long long res_i_add;
    long long res_i_mul;
    int i_val = 0;
    int rows = 0;
    int cols = 0;
    int curr_col;
    char* token;

    // Allocate "big enough" grid
    char*** grid = malloc(MAX_ROWS * sizeof(char**));
    for(int i = 0; i < MAX_ROWS; i++) {
        grid[i] = malloc(MAX_COLS * sizeof(char*));
        for(int j = 0; j < MAX_COLS; j++) {
            grid[i][j] = malloc(MAX_STR_LEN * sizeof(char));
        }
    }

    // Read the file line by line until the end of the file and fill our grid
    while (fgets(line_buffer, MAX_LINE_LENGTH, file_ptr) != NULL) {
        
        // we remove the new lines from each row of the input so they do not end up in our string matrix
        remove_newline(line_buffer);

        // get the first substring
        token = strtok(line_buffer, DELIMITERS);

        // init column counter
        curr_col = 0;
        do {
            // copy current token into the right position in the grid
            strcpy(grid[rows][curr_col], token);
            
            // Get the next token (pass NULL to continue from the last position)
            token = strtok(NULL, DELIMITERS);

            // move to next column
            curr_col++;
        } while (token != NULL);

        // Save the actual number of rows and columns of our string matrix for later
        if (cols == 0) { 
            cols = curr_col; 
        }

        // move to the next row
        rows++;
        
        // remove this later
    }

    // do the calculation
    for(int i = 0; i < cols; i++) {
        res_i_add = 0;
        res_i_mul = 1;

        for(int j = 0; j < rows-1; j++) {           
            if (strcmp(grid[rows-1][i], "*") == 0)
            {
                res_i_mul *= atoi(grid[j][i]);
            }
            if (strcmp(grid[rows-1][i], "+") == 0)
            {
                res_i_add += atoi(grid[j][i]);
            }
        }

        if (res_i_mul != 1)
        {
            result += res_i_mul;
        }
        if (res_i_add != 0)
        {
            result += res_i_add;
        }
    }    

    // Print out our grid
    // for(int i = 0; i < rows; i++) {
    //     for(int j = 0; j < cols; j++) {
    //         printf("%s ", grid[i][j]);
    //     }
    //      printf("\n");
    // }
   
    // Print out the final result
    printf("Result: %lld\n\n", result);

    // Free (still all MAX_ROWS × MAX_COLS)
    for(int i = 0; i < MAX_ROWS; i++) {
        for(int j = 0; j < MAX_COLS; j++) {
            free(grid[i][j]);
        }
        free(grid[i]);
    }
    free(grid);

    printf("Freed memory.\n\n");

    // Close the file
    fclose(file_ptr);
    return 0; // Indicate successful execution
}