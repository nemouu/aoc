#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

#define MAX_LINE_LENGTH 1024
#define MAX_TUPLES 100
#define MAX_COUNTERS 20

typedef struct {
    int* values;
    int count;
} Tuple;

int answer, N;
int contain[MAX_COUNTERS], need[MAX_COUNTERS], t[MAX_TUPLES];
int goal_size;

void parse_line(char* line, Tuple* tuples, int* tuple_count, int* goals, int* goal_count) {
    *tuple_count = 0;
    *goal_count = 0;
    
    char* ptr = line;
    
    // Skip [.##.]
    while (*ptr && *ptr != ']') ptr++;
    if (*ptr == ']') ptr++;
    
    // Parse buttons
    while (*ptr && *ptr != '{') {
        while (*ptr && *ptr != '(' && *ptr != '{') ptr++;
        if (!*ptr || *ptr == '{') break;
        
        ptr++;
        
        char* temp_ptr = ptr;
        int count = 1;
        while (*temp_ptr && *temp_ptr != ')') {
            if (*temp_ptr == ',') count++;
            temp_ptr++;
        }
        
        tuples[*tuple_count].values = malloc(count * sizeof(int));
        tuples[*tuple_count].count = count;
        
        int idx = 0;
        while (*ptr && *ptr != ')') {
            if (isdigit(*ptr)) {
                tuples[*tuple_count].values[idx++] = atoi(ptr);
                while (isdigit(*ptr)) ptr++;
            } else {
                ptr++;
            }
        }
        
        if (*ptr == ')') ptr++;
        (*tuple_count)++;
    }
    
    // Parse goals
    if (*ptr == '{') {
        ptr++;
        while (*ptr && *ptr != '}') {
            if (isdigit(*ptr)) {
                goals[(*goal_count)++] = atoi(ptr);
                while (isdigit(*ptr)) ptr++;
            } else {
                ptr++;
            }
        }
    }
}

int max_element(int* arr, int size) {
    int max = arr[0];
    for (int i = 1; i < size; i++) {
        if (arr[i] > max) max = arr[i];
    }
    return max;
}

int min_int(int a, int b) {
    return a < b ? a : b;
}

void rec(int mask, int cur_taken) {
    // All buttons processed
    if (mask == (1 << N) - 1) {
        // Verify all needs are satisfied
        for (int i = 0; i < goal_size; i++) {
            if (need[i] != 0) return;
        }
        if (cur_taken < answer) {
            printf("Found solution: %d\n", cur_taken);
            answer = cur_taken;
        }
        return;
    }
    
    // Pruning: best possible remaining
    int best_ret = max_element(need, goal_size);
    if (best_ret + cur_taken >= answer) return;
    
    // Find best button to try next (one affecting counter with fewest options)
    int best_id = -1, best_cnt = INT_MAX;
    for (int i = 0; i < N; i++) {
        if (mask & (1 << i)) continue;  // Already used
        
        int cnt = INT_MAX;
        for (int j = 0; j < goal_size; j++) {
            if (t[i] & (1 << j)) {  // Button i affects counter j
                cnt = min_int(cnt, contain[j]);
            }
        }
        
        if (cnt < best_cnt) {
            best_cnt = cnt;
            best_id = i;
        }
    }
    
    if (best_id == -1) return;
    
    // Find which counters this button affects
    int me[MAX_COUNTERS], me_size = 0;
    for (int i = 0; i < goal_size; i++) {
        if (t[best_id] & (1 << i)) {
            me[me_size++] = i;
        }
    }
    
    // If this is the ONLY button affecting some counter, we MUST use it
    if (best_cnt == 1) {
        int must_value = -1;
        for (int i = 0; i < me_size; i++) {
            int p = me[i];
            if (contain[p] == 1) {
                if (must_value == -1) {
                    must_value = need[p];
                } else if (must_value != need[p]) {
                    return;  // Contradiction
                }
            }
        }
        
        if (must_value == -1) return;
        
        // Check if feasible
        for (int i = 0; i < me_size; i++) {
            if (must_value > need[me[i]]) {
                return;  // Would overshoot
            }
        }
        
        // Apply forced move
        for (int i = 0; i < me_size; i++) {
            need[me[i]] -= must_value;
            contain[me[i]] -= 1;
        }
        
        rec(mask | (1 << best_id), cur_taken + must_value);
        
        // Backtrack
        for (int i = 0; i < me_size; i++) {
            need[me[i]] += must_value;
            contain[me[i]] += 1;
        }
        return;
    }
    
    // Try different numbers of presses for this button
    int mx = INT_MAX;
    for (int i = 0; i < me_size; i++) {
        mx = min_int(mx, need[me[i]]);
        contain[me[i]] -= 1;
    }
    
    // Try pressing mx times (maximum useful)
    for (int i = 0; i < me_size; i++) {
        need[me[i]] -= mx;
    }
    rec(mask | (1 << best_id), cur_taken + mx);
    
    // Try pressing fewer times
    for (int take = 1; take <= mx; take++) {
        for (int i = 0; i < me_size; i++) {
            need[me[i]] += 1;
        }
        rec(mask | (1 << best_id), cur_taken + mx - take);
    }
    
    // Backtrack
    for (int i = 0; i < me_size; i++) {
        contain[me[i]] += 1;
    }
}

int solve(Tuple* buttons, int num_buttons, int* goals, int num_goals) {
    N = num_buttons;
    goal_size = num_goals;
    
    // Initialize needs
    for (int i = 0; i < num_goals; i++) {
        need[i] = goals[i];
    }
    
    // Initialize contain (how many buttons affect each counter)
    for (int i = 0; i < num_goals; i++) {
        contain[i] = 0;
    }
    
    // Convert buttons to bitmasks and count affects
    for (int i = 0; i < num_buttons; i++) {
        t[i] = 0;
        for (int j = 0; j < buttons[i].count; j++) {
            int counter = buttons[i].values[j];
            t[i] |= (1 << counter);
            contain[counter]++;
        }
    }
    
    answer = INT_MAX;
    rec(0, 0);
    return answer;
}

int main() {
    FILE *file_ptr = fopen("input1.txt", "r");
    if (file_ptr == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    char line_buffer[MAX_LINE_LENGTH];
    int line_num = 0;
    long long total = 0;
    
    while (fgets(line_buffer, MAX_LINE_LENGTH, file_ptr) != NULL) {
        line_num++;
        
        Tuple tuples[MAX_TUPLES];
        int tuple_count = 0;
        int goals[MAX_COUNTERS];
        int goal_count = 0;
        
        parse_line(line_buffer, tuples, &tuple_count, goals, &goal_count);
        
        printf("Line %d: ", line_num);
        fflush(stdout);
        
        int result = solve(tuples, tuple_count, goals, goal_count);
        
        if (result != INT_MAX) {
            printf("%d presses\n", result);
            total += result;
        } else {
            printf("FAILED\n");
        }
        
        for (int i = 0; i < tuple_count; i++) {
            free(tuples[i].values);
        }
    }
    
    printf("\nTOTAL: %lld\n", total);
    
    fclose(file_ptr);
    return 0;
}