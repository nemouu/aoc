#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 1024
#define MAX_COORDS 1000

typedef struct {
    int col, row;
} Coord;

// Comparison function for qsort - sorts integers in ascending order
int compare_ints(const void* a, const void* b) {
    return (*(int*)a - *(int*)b);
}

// Calculate rectangle area from two corner points
long long rect_area(int x1, int y1, int x2, int y2) {
    return (long long)(abs(x1 - x2) + 1) * (abs(y1 - y2) + 1);
}

int main() {
    FILE *file_ptr = fopen("input1.txt", "r");
    if (file_ptr == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    // ========== STEP 1: Read input coordinates ==========
    Coord coords[MAX_COORDS];
    int num_coords = 0;
    char line_buffer[MAX_LINE_LENGTH];

    while (fgets(line_buffer, MAX_LINE_LENGTH, file_ptr) != NULL && num_coords < MAX_COORDS) {
        sscanf(line_buffer, "%d,%d", &coords[num_coords].col, &coords[num_coords].row);
        num_coords++;
    }
    fclose(file_ptr);

    printf("Read %d coordinates\n", num_coords);

    // ========== STEP 2: Coordinate Compression ==========
    // Goal: Map sparse coordinates (0-98000) to dense coordinates (0-250)
    // This makes our grid small enough to fit in memory
    
    // Collect all column and row values
    int all_col[MAX_COORDS + 2];
    int all_row[MAX_COORDS + 2];
    
    for (int i = 0; i < num_coords; i++) {
        all_col[i] = coords[i].col;
        all_row[i] = coords[i].row;
    }
    
    // Add boundary values to ensure grid edges are covered
    all_col[num_coords] = -1;
    all_row[num_coords] = -1;
    all_col[num_coords + 1] = 100000;
    all_row[num_coords + 1] = 100000;
    
    int num_col = num_coords + 2;
    int num_row = num_coords + 2;

    // Sort the coordinate arrays
    qsort(all_col, num_col, sizeof(int), compare_ints);
    qsort(all_row, num_row, sizeof(int), compare_ints);

    // Remove duplicates - create unique sorted arrays
    int unique_col[MAX_COORDS + 2];
    int unique_row[MAX_COORDS + 2];
    int ucol_count = 0, urow_count = 0;

    unique_col[ucol_count++] = all_col[0];
    for (int i = 1; i < num_col; i++) {
        if (all_col[i] != all_col[i-1]) {
            unique_col[ucol_count++] = all_col[i];
        }
    }

    unique_row[urow_count++] = all_row[0];
    for (int i = 1; i < num_row; i++) {
        if (all_row[i] != all_row[i-1]) {
            unique_row[urow_count++] = all_row[i];
        }
    }

    printf("Compressed grid: %d x %d\n", ucol_count, urow_count);

    // Map original coordinates to compressed coordinates
    // E.g., original (7, 100) might become compressed (2, 5)
    Coord shrunk[MAX_COORDS];
    for (int i = 0; i < num_coords; i++) {
        // Find position of this coordinate in the unique arrays
        int ccol = 0, crow = 0;
        for (int j = 0; j < ucol_count; j++) {
            if (unique_col[j] == coords[i].col) {
                ccol = j;
                break;
            }
        }
        for (int j = 0; j < urow_count; j++) {
            if (unique_row[j] == coords[i].row) {
                crow = j;
                break;
            }
        }
        shrunk[i].col = ccol;
        shrunk[i].row = crow;
    }

    // ========== STEP 3: Create and mark grid ==========
    // Grid values: 0 = unknown, 1 = inside/boundary, 2 = outside
    int** grid = malloc(urow_count * sizeof(int*));
    for (int i = 0; i < urow_count; i++) {
        grid[i] = calloc(ucol_count, sizeof(int));  // Initialize all to 0
    }

    // Mark all boundary tiles (red + green) as INSIDE (value 1)
    // Connect consecutive red tiles with straight lines
    for (int i = 0; i < num_coords; i++) {
        int next = (i + 1) % num_coords;  // Wrap around to close the loop
        int col1 = shrunk[i].col;
        int row1 = shrunk[i].row;
        int col2 = shrunk[next].col;
        int row2 = shrunk[next].row;

        // Fill all tiles between consecutive points
        int min_col = (col1 < col2) ? col1 : col2;
        int max_col = (col1 > col2) ? col1 : col2;
        int min_row = (row1 < row2) ? row1 : row2;
        int max_row = (row1 > row2) ? row1 : row2;

        for (int x = min_col; x <= max_col; x++) {
            for (int y = min_row; y <= max_row; y++) {
                grid[y][x] = 1;
            }
        }
    }

    // ========== STEP 4: Flood fill to mark outside ==========
    // Use BFS from corner (0,0) to mark everything outside as 2
    Coord* queue = malloc(ucol_count * urow_count * sizeof(Coord));
    int queue_start = 0, queue_end = 0;

    queue[queue_end++] = (Coord){0, 0};
    grid[0][0] = 2;  // Mark starting point as OUTSIDE

    while (queue_start < queue_end) {
        Coord co = queue[queue_start++];
        
        // Check 4 orthogonal neighbors (up, down, left, right)
        int dcol[] = {0, 0, 1, -1};
        int drow[] = {1, -1, 0, 0};
        
        for (int d = 0; d < 4; d++) {
            int ncol = co.col + dcol[d];
            int nrow = co.row + drow[d];
            
            // If neighbor is in bounds and unknown (0), mark it as outside
            if (ncol >= 0 && ncol < ucol_count && nrow >= 0 && nrow < urow_count && grid[nrow][ncol] == 0) {
                grid[nrow][ncol] = 2;  // Mark as OUTSIDE
                queue[queue_end++] = (Coord){ncol, nrow};
            }
        }
    }
    free(queue);
    
    // After flood fill:
    // - Value 2 = outside the loop
    // - Value 1 = on the boundary
    // - Value 0 = trapped inside the loop (couldn't reach from outside)

    // ========== STEP 5: Build 2D prefix sum array ==========
    // Prefix sum lets us count tiles in any rectangle in O(1) time
    // prefix[y][x] = count of inside tiles from (0,0) to (x,y)
    long long** prefix = malloc(urow_count * sizeof(long long*));
    for (int i = 0; i < urow_count; i++) {
        prefix[i] = calloc(ucol_count, sizeof(long long));
    }

    for (int y = 0; y < urow_count; y++) {
        for (int x = 0; x < ucol_count; x++) {
            // Count this cell if it's not outside (inside or boundary)
            long long val = (grid[y][x] != 2) ? 1 : 0;
            
            // Build prefix sum using inclusion-exclusion principle
            prefix[y][x] = val;
            if (x > 0) prefix[y][x] += prefix[y][x-1];      // Add left
            if (y > 0) prefix[y][x] += prefix[y-1][x];      // Add top
            if (x > 0 && y > 0) prefix[y][x] -= prefix[y-1][x-1];  // Subtract overlap
        }
    }

    printf("Grid built, checking rectangles...\n");

    // ========== STEP 6: Check all rectangle pairs ==========
    long long max_area = 0;

    for (int i = 0; i < num_coords; i++) {
        for (int j = i + 1; j < num_coords; j++) {
            // Get compressed coordinates for this pair
            int col1 = shrunk[i].col;
            int row1 = shrunk[i].row;
            int col2 = shrunk[j].col;
            int row2 = shrunk[j].row;

            // Find rectangle bounds
            int min_col = (col1 < col2) ? col1 : col2;
            int max_col = (col1 > col2) ? col1 : col2;
            int min_row = (row1 < row2) ? row1 : row2;
            int max_row = (row1 > row2) ? row1 : row2;

            // Expected = total tiles in rectangle
            long long expected = (long long)(max_col - min_col + 1) * (max_row - min_row + 1);
            
            // Actual = count of inside tiles using prefix sum (O(1) query!)
            // Formula: prefix[max] - prefix[left strip] - prefix[top strip] + prefix[corner]
            long long actual = prefix[max_row][max_col];
            if (min_col > 0) actual -= prefix[max_row][min_col - 1];
            if (min_row > 0) actual -= prefix[min_row - 1][max_col];
            if (min_col > 0 && min_row > 0) actual += prefix[min_row - 1][min_col - 1];

            // If all tiles are inside, rectangle is valid!
            if (expected == actual) {
                // Calculate area using ORIGINAL coordinates (not compressed)
                long long area = rect_area(coords[i].col, coords[i].row, 
                                          coords[j].col, coords[j].row);
                if (area > max_area) {
                    max_area = area;
                }
            }
        }
    }

    printf("\nResult: %lld\n", max_area);

    // ========== Cleanup ==========
    for (int i = 0; i < urow_count; i++) {
        free(grid[i]);
        free(prefix[i]);
    }
    free(grid);
    free(prefix);

    return 0;
}