#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define MAX_LINE_LENGTH 1024 // Define a maximum length for a line

// we want to convert the input lines to vectors
typedef struct {
    int x, y, z;
    int parent;
    int size;
} Vec;

// now we want to save the indexes and distance of two vectors in a special data structure
typedef struct {
    double distance;
    int node1;
    int node2;
} Pair;

// we create this to tell qsort how to compare these Pairs
int compare_pairs(const void* a, const void* b) {
    Pair* pair1 = (Pair*)a;
    Pair* pair2 = (Pair*)b;
    
    if (pair1->distance < pair2->distance)
    {
        return -1;
    } else if (pair1->distance > pair2->distance) {
        return 1;
    } else {
        return 0;
    }
}

// we create this to tell qsort how to compare integers (reverse order)
int compare_ints_rev(const void* a, const void* b) {
    int* i1 = (int*)a;
    int* i2 = (int*)b;
    if(*i1 < *i2) return 1;
    if(*i1 > *i2) return -1;
    return 0;
}

// given two input vectors this function calculates and return the straigt line distance
double sld_3d(Vec in1, Vec in2) {
    return sqrt(pow(in1.x - in2.x, 2) + pow(in1.y - in2.y, 2) + pow(in1.z - in2.z, 2));
}

// function to find the root junction of a branch
int find_root(Vec nodes[], int node_index) {
    // Start at the current junction
    int current = node_index;
    
    // Keep following parents until we find the root
    while (nodes[current].parent != -1) {
        current = nodes[current].parent;
    }   
    return current;  // This is the root
}

// function that merges two branches of junctions if they are in seperate groups
int merge_branches(Vec nodes[], int index1, int index2) {
    int root1 = find_root(nodes, index1);
    int root2 = find_root(nodes, index2);
    
    if (root1 != root2) {
        nodes[root2].parent = root1;
        nodes[root1].size += nodes[root2].size;
        return 1; // a merge happened
    }
    return 0; // it was root1 == root2 so no merge happend
}

// Main function
int main() {
    // Initialize file reading
    FILE *file_ptr;
    char line_buffer[MAX_LINE_LENGTH]; // Buffer to store each line

    // Open the file in read mode ("r")
    file_ptr = fopen("input1.txt", "r");

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

    // Initialize vector array and fill with default values
    Vec vector_arr[file_len];
    for(int i = 0; i < file_len; i++) {
        vector_arr[i].parent = -1;
        vector_arr[i].size = 1;
    }

    // calculate and assign value of Pair array
    Pair* pair_arr = malloc((file_len * (file_len - 1)) / 2 * sizeof(Pair));
    int num_pairs = 0;

    // and variables to read in data
    int start_pos = 0;
    int conns = 0;
    int in_x;
    int in_y;
    int in_z;
    int size_count = 0;
    int sizes[file_len];
    long long result = 0;

    // part 2 variables
    int num_groups = file_len;
    int last_node1 = -1;
    int last_node2 = -1;

    // Read the file line by line until the end of the file
    while (fgets(line_buffer, MAX_LINE_LENGTH, file_ptr) != NULL) {

        // reading in the data
        sscanf(line_buffer, "%d,%d,%d", &in_x, &in_y, &in_z);
        vector_arr[start_pos].x = in_x;
        vector_arr[start_pos].y = in_y;
        vector_arr[start_pos].z = in_z;
        start_pos++; 
    }

    // calculating the distances
    for(int i = 0; i < file_len - 1; i++) {
        for(int j = i + 1; j < file_len; j++) {
            pair_arr[num_pairs].distance = sld_3d(vector_arr[i], vector_arr[j]);
            pair_arr[num_pairs].node1 = i;
            pair_arr[num_pairs].node2 = j;
            num_pairs++;
        }
    }

    // sorting the distances to obtain the shortest ones
    qsort(pair_arr, num_pairs, sizeof(Pair), compare_pairs);

    // making connections
    for (int i = 0; i < num_pairs; i++) {
        if (num_groups == 1) { break; }  // All connected!
        
        int merged = merge_branches(vector_arr, pair_arr[i].node1, pair_arr[i].node2);
        
        if (merged) {
            num_groups--;
            last_node1 = pair_arr[i].node1;
            last_node2 = pair_arr[i].node2;
        }
    }

    // After loop, calculate result
    // This one liner also work due to type promotion result = (long long)vector_arr[last_node1].x * vector_arr[last_node2].x
    long long first_x = (long long)vector_arr[last_node1].x;
    long long second_x = (long long)vector_arr[last_node2].x;
    result = first_x * second_x;
    printf("\n\nResult: %lld\n\n", result);

    free(pair_arr);

    // Close the file
    fclose(file_ptr);
    return 0; // Indicate successful execution
}