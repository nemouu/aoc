#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 1024

typedef struct {
    long long start;
    long long end;
} Range;

// Return length of a string while considering special end of line characters
int length_of_str(char in_str[]) {
    int len = strlen(in_str);
    if(len > 0 && in_str[len-1] == '\n') {
        in_str[len-1] = '\0';
        len--;
    }
    return len;
}

// Compare function for qsort (probably not necessary in this example for its more general like this)
int compare_ranges(const void* a, const void* b) {
    Range* r1 = (Range*)a;
    Range* r2 = (Range*)b;
    if(r1->start < r2->start) return -1;
    if(r1->start > r2->start) return 1;
    return 0;
}

// Here we are merging the ranges while cutting double numbers. First we sort them and then we 
// check whether the current and the next range overlap and act accordingly regarding numbers that
// would be contained multiple times
long long count_numbers_in_merged_ranges(Range* ranges, int count) {
    if(count == 0) return 0;
    
    // Sort ranges by start position
    qsort(ranges, count, sizeof(Range), compare_ranges);
    
    // Merge overlapping/adjacent ranges and count
    long long total = 0;
    long long current_start = ranges[0].start;
    long long current_end = ranges[0].end;
    
    for(int i = 1; i < count; i++) {
        if(ranges[i].start <= current_end + 1) {
            // Overlapping or adjacent - extend current range
            if(ranges[i].end > current_end) {
                current_end = ranges[i].end;
            }
        } else {
            // Gap - count current range and start new one
            total += current_end - current_start + 1;
            current_start = ranges[i].start;
            current_end = ranges[i].end;
        }
    }
    
    // Add final range
    total += current_end - current_start + 1;
    return total;
}

int main() {
    FILE *file_ptr = fopen("input1.txt", "r");
    if(!file_ptr) {
        perror("Error opening file");
        return 1;
    }
    
    char line_buffer[MAX_LINE_LENGTH]; // Buffer to store each line
    
    // Read all ranges from section 1
    Range all_ranges[1000];
    int range_count = 0;
    
    // we load the ranges and the number of IDs seperately
    while (fgets(line_buffer, MAX_LINE_LENGTH, file_ptr) != NULL) {
        int len = length_of_str(line_buffer);
        if(len == 0) break;  // Empty line = end of section 1
        
        sscanf(line_buffer, "%lld-%lld", 
               &all_ranges[range_count].start, 
               &all_ranges[range_count].end);
        range_count++;
    }
    
    printf("Loaded %d ranges from section 1\n", range_count);
    
    // Read all IDs from section 2
    long long ids[2000];
    int id_count = 0;
    
    while (fgets(line_buffer, MAX_LINE_LENGTH, file_ptr) != NULL) {
        int len = length_of_str(line_buffer);
        if(len > 0) {
            ids[id_count++] = atoll(line_buffer);
        }
    }
    
    printf("Loaded %d IDs from section 2\n", id_count);
    
    // Part 1: Count how many IDs are fresh
    int fresh_count = 0;
    for(int i = 0; i < id_count; i++) {
        for(int j = 0; j < range_count; j++) {
            if(ids[i] >= all_ranges[j].start && ids[i] <= all_ranges[j].end) {
                fresh_count++;
                break;  // ID is fresh, no need to check more ranges (I had extra loops to check that before)
            }
        }
    }
    
    printf("Number of IDs of fresh ingredients: %d\n\n", fresh_count);
    
    // Part 2: Count all unique IDs in all ranges
    long long total_fresh_ids = count_numbers_in_merged_ranges(all_ranges, range_count);
    
    printf("Total number of fresh IDs: %lld\n", total_fresh_ids);
    
    fclose(file_ptr);
    return 0;
}