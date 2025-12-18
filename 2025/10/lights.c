#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024
#define MAX_TUPLES 1000
#define MAX_QUEUE 1000000

typedef struct {
    int* values;
    int count;
} Tuple;

typedef struct {
    unsigned long long state;
    int steps;
} QueueItem;

void parse_line(char* line, char* char_array, int* char_count, Tuple* tuples, int* tuple_count) {
    *char_count = 0;
    *tuple_count = 0;
    
    char* ptr = line;
    
    // Parse character array between [ ]
    while (*ptr && *ptr != '[') ptr++;
    if (*ptr == '[') {
        ptr++;
        while (*ptr && *ptr != ']') {
            char_array[(*char_count)++] = *ptr;
            ptr++;
        }
        if (*ptr == ']') ptr++;
    }
    
    // Parse tuples
    while (*ptr) {
        while (*ptr && *ptr != '(') ptr++;
        if (!*ptr) break;
        
        ptr++;
        
        // Count numbers in this tuple
        char* temp_ptr = ptr;
        int count = 1;
        while (*temp_ptr && *temp_ptr != ')') {
            if (*temp_ptr == ',') count++;
            temp_ptr++;
        }
        
        tuples[*tuple_count].values = malloc(count * sizeof(int));
        tuples[*tuple_count].count = count;
        
        // Parse numbers
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
}

int bfs_solve(unsigned long long initial, unsigned long long goal, 
              unsigned long long* buttons, int num_buttons) {
    
    // Queue for BFS
    QueueItem* queue = malloc(MAX_QUEUE * sizeof(QueueItem));
    int queue_start = 0, queue_end = 0;
    
    // Hash set to track visited states (simple array for now)
    // For a better solution, use a proper hash table
    unsigned long long* visited = malloc(MAX_QUEUE * sizeof(unsigned long long));
    int visited_count = 0;
    
    // Start BFS
    queue[queue_end++] = (QueueItem){initial, 0};
    visited[visited_count++] = initial;
    
    while (queue_start < queue_end) {
        QueueItem current = queue[queue_start++];
        
        // Check if we reached the goal
        if (current.state == goal) {
            free(queue);
            free(visited);
            return current.steps;
        }
        
        // Try pressing each button
        for (int i = 0; i < num_buttons; i++) {
            // Apply button (XOR toggles the bits)
            unsigned long long new_state = current.state ^ buttons[i];
            
            // Check if we've seen this state before
            int seen = 0;
            for (int j = 0; j < visited_count; j++) {
                if (visited[j] == new_state) {
                    seen = 1;
                    break;
                }
            }
            
            if (!seen && queue_end < MAX_QUEUE && visited_count < MAX_QUEUE) {
                visited[visited_count++] = new_state;
                queue[queue_end++] = (QueueItem){new_state, current.steps + 1};
            }
        }
    }
    
    // No solution found
    free(queue);
    free(visited);
    return -1;
}

int main() {
    FILE *file_ptr = fopen("input1.txt", "r");
    if (file_ptr == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    char line_buffer[MAX_LINE_LENGTH];
    int line_num = 0;
    long long total = 0;  // Sum of all minimum button presses
    
    // Read file line by line
    while (fgets(line_buffer, MAX_LINE_LENGTH, file_ptr) != NULL) {
        line_num++;
        
        char char_array[MAX_LINE_LENGTH];
        int char_count = 0;
        
        Tuple tuples[MAX_TUPLES];
        int tuple_count = 0;
        
        // Parse this line
        parse_line(line_buffer, char_array, &char_count, tuples, &tuple_count);
        
        printf("\n=== Line %d ===\n", line_num);
        printf("Goal: ");
        for (int i = 0; i < char_count; i++) {
            printf("%c", char_array[i]);
        }
        printf(" (%d lights, %d buttons)\n", char_count, tuple_count);
        
        // Convert goal state to bitfield
        unsigned long long goal_state = 0;
        for (int i = 0; i < char_count; i++) {
            if (char_array[i] == '#') {
                goal_state |= (1ULL << i);
            }
        }
        
        // Convert buttons to bitfields
        unsigned long long buttons[MAX_TUPLES];
        for (int i = 0; i < tuple_count; i++) {
            buttons[i] = 0;
            for (int j = 0; j < tuples[i].count; j++) {
                int light_idx = tuples[i].values[j];
                buttons[i] |= (1ULL << light_idx);
            }
        }
        
        // Solve with BFS
        unsigned long long initial_state = 0;  // All lights off
        int result = bfs_solve(initial_state, goal_state, buttons, tuple_count);
        
        if (result != -1) {
            printf("Minimum presses: %d\n", result);
            total += result;
        } else {
            printf("No solution found!\n");
        }
        
        // Cleanup tuples
        for (int i = 0; i < tuple_count; i++) {
            free(tuples[i].values);
        }
    }
    
    printf("\n=== Total: %lld ===\n", total);
    
    fclose(file_ptr);
    return 0;
}