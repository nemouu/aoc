#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 4096
#define NUM_ROWS 4

// remove newline at the end of a string
void remove_newline(char* str) {
    int len = strlen(str);
    if(len > 0 && str[len-1] == '\n') {
        str[len-1] = '\0';
    }
}

int main() {
    FILE *file_ptr = fopen("input1.txt", "r");
    if (file_ptr == NULL) {
        perror("Error opening file");
        return 1;
    }
    
    // Read all lines
    char raw_grid[100][MAX_LINE_LENGTH];
    int num_lines = 0;
    
    while (fgets(raw_grid[num_lines], MAX_LINE_LENGTH, file_ptr) != NULL) {
        remove_newline(raw_grid[num_lines]);
        num_lines++;
    }
    fclose(file_ptr);
    
    // Find operators in last row
    int last_row = num_lines - 1;
    int operator_positions[2000];
    char operators[2000];
    int num_operators = 0;
    
    for(int i = 0; i < strlen(raw_grid[last_row]); i++) {
        char ch = raw_grid[last_row][i];
        if(ch == '*' || ch == '+') {
            operator_positions[num_operators] = i;
            operators[num_operators] = ch;
            num_operators++;
        }
    }
    
    // Process each column
    long long grand_total = 0;
    
    for(int op_idx = 0; op_idx < num_operators; op_idx++) {
        int op_col = operator_positions[op_idx];
        char op = operators[op_idx];
        
        // Find the extent of numbers in this column
        int left_bound = op_col;
        int right_bound = op_col + 1;
        
        // Expand left
        for(int row = 0; row < NUM_ROWS; row++) {
            for(int i = op_col; i >= 0; i--) {
                if(i < strlen(raw_grid[row]) && isdigit(raw_grid[row][i])) {
                    if(i < left_bound) left_bound = i;
                } else {
                    break;
                }
            }
        }
        
        // Expand right
        for(int row = 0; row < NUM_ROWS; row++) {
            for(int i = op_col; i < strlen(raw_grid[row]); i++) {
                if(isdigit(raw_grid[row][i])) {
                    if(i + 1 > right_bound) right_bound = i + 1;
                } else {
                    break;
                }
            }
        }
        
        // Read character positions from right to left
        long long column_result = 0;
        int first_number = 1;
        
        for(int char_pos = right_bound - 1; char_pos >= left_bound; char_pos--) {
            char number_str[100] = "";
            int digit_count = 0;
            
            for(int row = 0; row < NUM_ROWS; row++) {
                if(char_pos < strlen(raw_grid[row])) {
                    char ch = raw_grid[row][char_pos];
                    if(isdigit(ch)) {
                        number_str[digit_count++] = ch;
                    }
                }
            }
            number_str[digit_count] = '\0';
            
            if(digit_count > 0) {
                long long num = atoll(number_str);
                
                if(first_number) {
                    column_result = num;
                    first_number = 0;
                } else {
                    if(op == '+') {
                        column_result += num;
                    } else {
                        column_result *= num;
                    }
                }
            }
        }
        
        grand_total += column_result;
    }
    printf("Grand Total: %lld\n", grand_total);

    // Add these debug lines:
    printf("\nFound %d operators\n", num_operators);
    printf("Total lines read: %d\n", num_lines);
    printf("NUM_ROWS: %d\n", NUM_ROWS);
    
    return 0;
}