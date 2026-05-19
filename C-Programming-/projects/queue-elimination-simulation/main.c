/* COP 3502C Assignment 2
This program is written by: Jade Kirkpatrick*/
#include <stdio.h>
#include <stdlib.h>

#define MAX_STUDENTS 10000
#define MAX_GARAGES 10

typedef struct Student {
    int sequence_number;
    struct Student *next;
} Student;

typedef struct {
    int garage_num;
    Student *front;
    Student *back;
    int size;
    int k; // Skip count
    int th; // Threshold
} Queue;

// Function to create a new student
Student* createStudent(int sequence) {
    Student *newStudent = (Student*)malloc(sizeof(Student));
    newStudent->sequence_number = sequence;
    newStudent->next = NULL;
    return newStudent;
}

// Function to check if all queues are empty except for the queue at the given index
int checkIfQueuesEmpty(Queue *queues, int num_garages, int index) {
    // Check if all other queues are empty
    for (int i = 0; i < num_garages; i++) {
        if (i != index && queues[i].size > 0) {
            return 0; // At least one other queue is not empty
        }
    }

    // Check if the queue at the given index has exactly one student
    if (queues[index].size == 1) {
        return 1; // All other queues are empty and this one has exactly 1 student
    }

    return 0; // Either the queue at the given index does not have exactly 1 student or other queues are not empty
}

// Function to enqueue a student
void enqueue(Queue *q, int sequence) {
    Student *newStudent = createStudent(sequence);
    if (q->size == 0) {
        q->front = q->back = newStudent;
        newStudent->next = newStudent; // Point to itself for circularity
    } else {
        newStudent->next = q->front; // New student points to the front
        q->back->next = newStudent; // Back's next points to new student
        q->back = newStudent; // Update back pointer
    }
    q->size++;
}

// Function to display the queue for initial status
void displayInitialStatus(Queue *q) {
    if (q->size == 0) return;

    printf("%d ", q->garage_num); // Print garage number
    Student *current = q->front;
    int first = 1; // Flag to handle spacing
    do {
        if (!first) {
            printf(" "); // Print space before subsequent numbers
        }
        printf("%d", current->sequence_number);
        current = current->next;
        first = 0; // After first number, set the flag to false
    } while (current != q->front);
}

// Function to sort students in each queue
void sort_queue(Queue *q) {
    if (q->size <= 1) return; // No need to sort if size is 0 or 1

    // Transfer students to an array for sorting
    int *students = (int *)malloc(q->size * sizeof(int));
    Student *current = q->front;
    for (int i = 0; i < q->size; i++) {
        students[i] = current->sequence_number;
        current = current->next;
    }

    // Sort the array in ascending order
    for (int i = 0; i < q->size - 1; i++) {
        for (int j = 0; j < q->size - i - 1; j++) {
            if (students[j] > students[j + 1]) {
                // Swap
                int temp = students[j];
                students[j] = students[j + 1];
                students[j + 1] = temp;
            }
        }
    }

    // Rebuild the queue with sorted students starting from 1
    current = q->front;
    for (int i = 0; i < q->size; i++) {
        current->sequence_number = i + 1; // Fill with 1 to size
        current = current->next;
    }

    free(students); // Free the array
}

// Function to display all queues sorted by garage number
void displayAllQueues(Queue *queues, int num_garages) {
    printf("Initial status of nonempty queues\n");
    // Sort the queues by garage number
    for (int i = 0; i < num_garages; i++) {
        for (int j = i + 1; j < num_garages; j++) {
            if (queues[i].garage_num > queues[j].garage_num) {
                // Swap queues
                Queue temp = queues[i];
                queues[i] = queues[j];
                queues[j] = temp;
            }
        }
    }

    for (int i = 0; i < num_garages; i++) {
        displayInitialStatus(&queues[i]);
        printf("\n");
    }
}

// Phase 1 elimination
void phase1_elimination(Queue *queues, int num_garages) {
    printf("\nPhase1 elimination\n");
    for (int i = 0; i < num_garages; i++) {
        Queue *q = &queues[i];
        if (q->size == 0) continue; // Skip empty queues
        printf("\nGroup for Garage# %d\n", q->garage_num);

        while (q->size > q->th) {
            int skip = 0;
            Student *current = q->front;
            Student *prev = q->back; // Start from back to manage circular deletion
            while (q->size > q->th) {
                if (skip < q->k - 1) {
                    prev = current;
                    current = current->next;
                    skip++;
                } else {
                    printf("Student# %d eliminated\n", current->sequence_number);
                    prev->next = current->next; // Remove current from the circle
                    if (current == q->front) {
                        q->front = current->next; // Update front if necessary
                    }
                    if (current == q->back) {
                        q->back = prev; // Update back if necessary
                    }
                    Student *to_free = current;
                    current = current->next; // Move to next student
                    free(to_free);
                    q->size--;
                    skip = 0; // Reset skip
                }
            }
        }
    }
}

// Function to perform Phase 2 elimination
void phase2_elimination(Queue *queues, int num_garages) {
    printf("\nPhase2 elimination\n");

    while (1) {
        int max_student = -1;
        int max_index = -1;

        // Find the group with the highest front student
        for (int i = 0; i < num_garages; i++) {
            if (queues[i].size > 0 && queues[i].front->sequence_number > max_student) {
                max_student = queues[i].front->sequence_number;
                max_index = i;
            }
        }

        if (max_index == -1) break; // No students left

        if (checkIfQueuesEmpty(queues, num_garages, max_index) == 1) { // Check if only one student exists
          return;
        }
        // Eliminate the student
        printf("Eliminated student %d from group for garage %d\n",
               queues[max_index].front->sequence_number, queues[max_index].garage_num);

        Student *to_free = queues[max_index].front;
        queues[max_index].front = queues[max_index].front->next;
        queues[max_index].back->next = queues[max_index].front; // Maintain circularity
        queues[max_index].size--;

        if (queues[max_index].size == 0) {
            queues[max_index].front = queues[max_index].back = NULL; // Reset if empty
        }

        free(to_free);
    }
}

// Function to free all allocated memory
void freeAllQueues(Queue *queues, int num_garages) {
    for (int i = 0; i < num_garages; i++) {
        Student *current = queues[i].front;
        while (queues[i].size > 0) {
            Student *to_free = current;
            current = current->next; // Move to next student
            free(to_free);
            queues[i].size--;
        }
        queues[i].front = NULL;
        queues[i].back = NULL;
    }
}

int main() {
    int num_garages;

    // Check return value of scanf
    if (scanf("%d", &num_garages) != 1) {
        fprintf(stderr, "Error reading number of garages\n");
        return 1; // Exit with error
    }

    Queue queues[MAX_GARAGES];

    // Initialize queues
    for (int i = 0; i < num_garages; i++) {
        int gi, ni, ki, thi;

        // Check return value of scanf
        if (scanf("%d %d %d %d", &gi, &ni, &ki, &thi) != 4) {
            fprintf(stderr, "Error reading garage information for garage %d\n", i + 1);
            return 1; // Exit with error
        }

        queues[i].garage_num = gi;
        queues[i].k = ki;
        queues[i].th = thi;
        queues[i].front = NULL;
        queues[i].back = NULL;
        queues[i].size = 0;

        // Enqueue students in reverse order
        for (int j = ni; j >= 1; j--) {
            enqueue(&queues[i], j);
        }
    }

    // Step 1: Print initial status
    displayAllQueues(queues, num_garages);

    // Step 2: Sort each queue
    for (int i = 0; i < num_garages; i++) {
        sort_queue(&queues[i]);
    }

    // Print after ordering status
    printf("\nAfter ordering status of nonempty queues\n");
    for (int i = 0; i < num_garages; i++) {
        displayInitialStatus(&queues[i]);
        printf("\n");
    }

    // Step 3: Perform Phase 1 elimination
    phase1_elimination(queues, num_garages);

    // Step 4: Perform Phase 2 elimination
    phase2_elimination(queues, num_garages);

    // Find and print the winner
    for (int i = 0; i < num_garages; i++) {
        if (queues[i].size == 1) {
            printf("\nStudent %d from the group for garage %d is the winner!\n",
                   queues[i].front->sequence_number, queues[i].garage_num);
            break; // Declare the winner and exit
        }
    }

    // Cleanup: Free all queues
    freeAllQueues(queues, num_garages);

    return 0;
}
