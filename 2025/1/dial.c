#include <stdio.h>
#include <stdlib.h>

#define MAX_LINE_LENGTH 64 // Define a maximum length for a line

// Define the structure for a node in a Doubly Linked List
struct Node {
    int data;
    struct Node* next; // Pointer to the next node
    struct Node* prev; // Pointer to the previous node
};

struct Node* head = NULL;

// inserts a new node at the end of the circular doubly linked list.
void insertEnd(int data) {
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    if (newNode == NULL) {
        printf("Memory allocation failed!\n");
        exit(1);
    }
    newNode->data = data;
    
    if (head == NULL) {
        head = newNode;
        newNode->next = head;
        newNode->prev = head;
    } else {
        struct Node* last = head->prev;
        last->next = newNode;
        newNode->prev = last;
        newNode->next = head;
        head->prev = newNode;
    }
}

// Populates the list with numbers from 0 to 99. 
void populateList_0_to_x(int x) {
    for (int i = 0; i <= x; i++) {
        insertEnd(i);
    }
}

// Searches for a node with a specific data value and returns a pointer to it.
struct Node* findNode(int value) {
    struct Node* current = head;
    if (head == NULL) {
        return NULL;
    }
    do {
        if (current->data == value) {
            return current;
        }
        current = current->next;
    } while (current != head);

    return NULL; // Value not found
}

// Main function
int main() {
    FILE *file_ptr;
    char line_buffer[MAX_LINE_LENGTH]; // Buffer to store each line
    char buff_dir;
    int buff_steps;
    int buff_scan;
    int pw = 0;
    
    populateList_0_to_x(99);

    struct Node* dial = findNode(50);	

    // Open the file in read mode ("r")
    file_ptr = fopen("input1.txt", "r");

    // Check if the file was opened successfully
    if (file_ptr == NULL) {
        perror("Error opening file"); // Print a system error message
        exit(EXIT_FAILURE);         // Exit with an error code
    }

    // Read the file line by line until the end of the file
    while (fgets(line_buffer, MAX_LINE_LENGTH, file_ptr) != NULL) {
        // Process the line (e.g., print it to the console)
        buff_scan = sscanf(line_buffer, "%c%d", &buff_dir, &buff_steps);

        for(int i = 0; i < buff_steps; i++) {
            if (buff_dir == 'R') {
                dial = dial->next;
            } else if (buff_dir == 'L') {
                dial = dial->prev;
            }
            if (dial->data == 0) { pw++; }
        }
    }

    // Print out the result
    printf("The password is: %d", pw);	

    // Close the file
    fclose(file_ptr);

    return 0; // Indicate successful execution
}
