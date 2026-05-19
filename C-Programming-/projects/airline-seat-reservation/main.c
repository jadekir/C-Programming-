//COP3223 Fall 2023 Assignment 2.3
//Copyright 2023 Kirkpatrick Jade

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NUM_SEATS 6

int seats[NUM_SEATS] = {0}; // 0 indicates empty seat, 1 indicates occupied seat

void search_seat(int class) {
    int seat;
    if (class == 1) {
        // Search for available seat in first class
        for (seat = 0; seat < 3; seat++) {
            if (seats[seat] == 0) {
                seats[seat] = 1; // Assign seat
                printf("Your seat is assigned to first class seat %d\n", seat + 1);
                return;
            }
        }
    } else {
        // Search for available seat in economy class
        for (seat = 3; seat < NUM_SEATS; seat++) {
            if (seats[seat] == 0) {
                seats[seat] = 1; // Assign seat
                printf("Your seat is assigned to economy seat %d\n", seat + 1);
                return;
            }
        }
    }
}

void print_boarding_pass(int seat, int class) {
    if (class == 1) {
        printf("Boarding Pass\n");
        printf("Seat: %d\n", seat + 1);
        printf("Class: First Class\n");
    } else {
        printf("Boarding Pass\n");
        printf("Seat: %d\n", seat + 1);
        printf("Class: Economy\n");
    }
}

int main() {
    int choice;
    int all_boarded = 0;

    srand(time(NULL)); // Seed the random number generator

    while (!all_boarded) {
        printf("Please type 1 for \"first class\"\n");
        printf("Please type 2 for \"economy\"\n");
        scanf("%d", &choice);

        if (choice == 1) {
            search_seat(1); // Assign seat in first class
        } else if (choice == 2) {
            search_seat(2); // Assign seat in economy class
        } else {
            printf("Invalid choice. Please try again.\n");
            continue;
        }

        printf("Does everyone boarded? (1 for yes, 0 for no)\n");
        scanf("%d", &all_boarded);
    }

    return 0;
}


//Explanation of the solution:

//1. First I start by defining a one-dimensional array `seats` to represent the seating chart. Each element of the array represents a seat, with 0 indicating an empty seat and 1 indicating an occupied seat.

//2. The `search_seat` function takes an argument `class` (1 for first class, 2 for economy) and searches for an available seat in the corresponding class. If a seat is found, it is assigned to the passenger and the seating chart is updated. The function uses a loop to iterate through the seats and checks if each seat is empty.

//3. The `print_boarding_pass` function takes the seat number and class as arguments and prints the passenger's boarding pass with the seat number and class.

//4. In the `main` function, I use a `while` loop to handle multiple passengers. Inside the loop, I display the menu of alternatives and prompt the user for their choice. Based on the choice, we call the `search_seat` function to assign a seat. I then ask if everyone has boarded and update the `all_boarded` variable accordingly.

//5. The program continues to loop until the user indicates that everyone has boarded.
