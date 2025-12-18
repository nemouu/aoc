#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define MAX_LINE_LENGTH 1024 // Define a maximum length for a line

// we want to convert the input lines to coordinates
typedef struct {
    int  col_num, row_num;
} Coord;

// given four natural numbers calculate the area of the rectangle that they span up
long long rect_area(int x1, int y1, int x2, int y2) {
    long long res = (long long)((abs(x1 - x2)) + 1)*((abs(y1 - y2)) + 1);
    return res;
}

// Main function
int main() {
    // Initialize file reading
    FILE *file_ptr;
    char line_buffer[MAX_LINE_LENGTH]; // Buffer to store each line

    // Open the file in read mode ("r")
    file_ptr = fopen("inputtest.txt", "r");

    // Check if the file was opened successfully
    if (file_ptr == NULL) {
        perror("Error opening file"); // Print a system error message
        exit(EXIT_FAILURE);           // Exit with an error code
    }

    // we go through the file first to determine the length
    int file_len = 0;
    while (fgets(line_buffer, MAX_LINE_LENGTH, file_ptr) != NULL) {
        file_len++;
    }
    rewind(file_ptr);

    // Initialize coordinate array
    Coord co_arr[file_len];

    // Intialize array of edge tiles
    Coord edge_arr[1000];
    int edge_size = 1;

    // and variables to read in data
    int start_pos = 0;
    int in_col;
    int in_row;
    long long curr_res = 0;
    long long result = 0;

    // Read the file line by line until the end of the file
    while (fgets(line_buffer, MAX_LINE_LENGTH, file_ptr) != NULL) {

        // reading in the data
        sscanf(line_buffer, "%d,%d", &in_col, &in_row);
        co_arr[start_pos].col_num = in_col;
        co_arr[start_pos].row_num = in_row;
        start_pos++; 
    }
   
    // print the array the check if it was read in correctly
    for (int i = 0; i < file_len; i++)
    {
        printf("(%d,%d)\n", co_arr[i].col_num, co_arr[i].row_num);
    }

    // print some test results
    // printf("(%d,%d) and (%d,%d) give %d\n", 2, 5, 9, 7, rect_area(2, 5, 9, 7));
    // printf("(%d,%d) and (%d,%d) give %d\n", 7, 1, 11, 7, rect_area(7, 1, 11, 7));
    // printf("(%d,%d) and (%d,%d) give %d\n", 7, 3, 2, 3, rect_area(7, 3, 2, 3));
    // printf("(%d,%d) and (%d,%d) give %d\n", 2, 5, 11, 1, rect_area(2, 5, 11, 1));

    for (int i = 0; i < file_len; i++)
    {
        for (int j = i + 1; j < file_len; j++)
        {
            
            curr_res = rect_area(co_arr[i].col_num, co_arr[i].row_num, co_arr[j].col_num, co_arr[j].row_num);

            // print to check what was given over and calculated
            // printf("(%d,%d) and (%d,%d) give %d\n", co_arr[i].col_num, co_arr[i].row_num, co_arr[j].col_num, co_arr[j].row_num, curr_res);
            
            if (result < curr_res)
            {
                result = curr_res;
            }       
        }
    }
    
    // Print result - the result was 2147468414 (2147483647) before
    printf("\nResult: %lld\n\n", result);

    // free arrays that were used
    // free();

    // Close the file
    fclose(file_ptr);
    return 0; // Indicate successful execution
}