#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define MAX_LINE_LENGTH 1024 // Define a maximum length for a line

// Turns a string of numbers into and array of numbers
int* turn_to_arr(char in_str[], int in_len) {
    int* arr = malloc(in_len * sizeof(int));  // Allocate on heap
    
    // Good practice to check
    if(arr == NULL) {  
        return NULL;
    }
    
    for(int i = 0; i < in_len; i++) {
        arr[i] = in_str[i] - '0';
    }
    
    return arr;
}

// Return length of a string while considering special end of line characters
int length_of_str(char in_str[]) {
    int len = strlen(in_str);
    if(len > 0 && in_str[len-1] == '\n') {
        in_str[len-1] = '\0';
        len--;
    }
    return len;
}

// gets the power of a number, for example: multiply_by_power(9, 1) → 9 * 10 = 90
long long multiply_by_power(int num, long long power) {
    long long result = num;
    for(int i = 0; i < power; i++) {
        result *= 10;
    }
    return result;
}

// contains the main algorithm that find the maximum joltage
long long find_max_jolts(int batt_num, int* num_arr, int len_arr) {
    long long result = 0;
    int offset = 0;
    
    for (int i = 0; i < batt_num; i++)
    {
        int max_pos[2] = {0, 0};
        int* internal_arr = num_arr + offset;
        int internal_len = len_arr - offset;

        // Print the array
        //for(int i = 0; i < internal_len; i++) {
        //    printf("%d", internal_arr[i]);
        //}
        //printf("\n\n");

        for (int j = 0; j < internal_len - (batt_num - 1 - i); j++)
        {
            if (max_pos[0] < internal_arr[j])
            {
                max_pos[0] = internal_arr[j];
                max_pos[1] = j;
                //printf("curr max: %d\n", max_pos[0]);
                //printf("curr max pos: %d\n", max_pos[1]);
            }
            
        }
        offset += max_pos[1] + 1;
        result += multiply_by_power(max_pos[0], batt_num - 1 - i);
        
    }
    return result;
}

// Main function
int main() {
    FILE *file_ptr;
    char line_buffer[MAX_LINE_LENGTH]; // Buffer to store each line
    long long result = 0;

    // for testing (example input)
    char line_test[MAX_LINE_LENGTH]  = "234234234234278";

    // Open the file in read mode ("r")
    file_ptr = fopen("input1.txt", "r");

    // Check if the file was opened successfully
    if (file_ptr == NULL) {
        perror("Error opening file"); // Print a system error message
        exit(EXIT_FAILURE);           // Exit with an error code
    }

    // Read the file line by line until the end of the file
    while (fgets(line_buffer, MAX_LINE_LENGTH, file_ptr) != NULL) {

        int curr_len = length_of_str(line_buffer);
        int* in_turned_arr = turn_to_arr(line_buffer, curr_len);

        // Print the array
        // for(int i = 0; i < curr_len; i++) {
        //     printf("%d", in_turned_arr[i]);
        // }
        // printf("\n\n");

        result += find_max_jolts(12, in_turned_arr, curr_len);
        printf("Result: %lld\n\n", result);

        // free the arrays every time
        free(in_turned_arr);
    }
    printf("Result: %lld\n\n", result);

    // Close the file
    fclose(file_ptr);
    return 0; // Indicate successful execution
}
