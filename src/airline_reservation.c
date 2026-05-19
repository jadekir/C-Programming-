#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SEATS 12
#define NAME_LEN 64

typedef struct {
    int seat_number;
    int reserved;
    char passenger[NAME_LEN];
} Seat;

static void trim_newline(char *text) {
    size_t len = strlen(text);
    if (len > 0 && text[len - 1] == '\n') {
        text[len - 1] = '\0';
    }
}

static void initialize(Seat seats[]) {
    for (int i = 0; i < SEATS; i++) {
        seats[i].seat_number = i + 1;
        seats[i].reserved = 0;
        seats[i].passenger[0] = '\0';
    }
}

static void list_seats(const Seat seats[]) {
    puts("\nSeat Map");
    for (int i = 0; i < SEATS; i++) {
        printf("Seat %2d: %s", seats[i].seat_number, seats[i].reserved ? "Reserved" : "Open");
        if (seats[i].reserved) {
            printf(" - %s", seats[i].passenger);
        }
        putchar('\n');
    }
}

static int read_seat_number(void) {
    char buffer[32];
    printf("Seat number (1-%d): ", SEATS);
    if (!fgets(buffer, sizeof(buffer), stdin)) {
        return -1;
    }
    return atoi(buffer);
}

static void reserve_seat(Seat seats[]) {
    int seat_number = read_seat_number();
    if (seat_number < 1 || seat_number > SEATS) {
        puts("Invalid seat number.");
        return;
    }

    Seat *seat = &seats[seat_number - 1];
    if (seat->reserved) {
        puts("That seat is already reserved.");
        return;
    }

    printf("Passenger name: ");
    if (!fgets(seat->passenger, sizeof(seat->passenger), stdin)) {
        puts("Could not read passenger name.");
        return;
    }
    trim_newline(seat->passenger);
    if (seat->passenger[0] == '\0') {
        puts("Passenger name cannot be blank.");
        return;
    }

    seat->reserved = 1;
    printf("Reserved seat %d for %s.\n", seat->seat_number, seat->passenger);
}

static void cancel_seat(Seat seats[]) {
    int seat_number = read_seat_number();
    if (seat_number < 1 || seat_number > SEATS || !seats[seat_number - 1].reserved) {
        puts("No reservation found for that seat.");
        return;
    }

    seats[seat_number - 1].reserved = 0;
    seats[seat_number - 1].passenger[0] = '\0';
    puts("Reservation canceled.");
}

static void find_passenger(const Seat seats[]) {
    char query[NAME_LEN];
    printf("Passenger search: ");
    if (!fgets(query, sizeof(query), stdin)) {
        return;
    }
    trim_newline(query);

    for (int i = 0; i < SEATS; i++) {
        if (seats[i].reserved && strstr(seats[i].passenger, query) != NULL) {
            printf("Found %s in seat %d.\n", seats[i].passenger, seats[i].seat_number);
            return;
        }
    }
    puts("Passenger not found.");
}

static void print_boarding_pass(const Seat seats[]) {
    int seat_number = read_seat_number();
    if (seat_number < 1 || seat_number > SEATS || !seats[seat_number - 1].reserved) {
        puts("No reservation found for that seat.");
        return;
    }

    const Seat *seat = &seats[seat_number - 1];
    puts("\n-------------------------");
    puts("BOARDING PASS");
    printf("Passenger: %s\n", seat->passenger);
    printf("Seat:      %d\n", seat->seat_number);
    puts("Flight:    UCF-2026");
    puts("-------------------------");
}

int main(void) {
    Seat seats[SEATS];
    char choice[16];
    initialize(seats);

    for (;;) {
        puts("\n1. List seats");
        puts("2. Reserve seat");
        puts("3. Cancel reservation");
        puts("4. Search passenger");
        puts("5. Print boarding pass");
        puts("0. Exit");
        printf("Choice: ");

        if (!fgets(choice, sizeof(choice), stdin)) {
            break;
        }

        switch (atoi(choice)) {
            case 1: list_seats(seats); break;
            case 2: reserve_seat(seats); break;
            case 3: cancel_seat(seats); break;
            case 4: find_passenger(seats); break;
            case 5: print_boarding_pass(seats); break;
            case 0: return 0;
            default: puts("Choose a valid option.");
        }
    }

    return 0;
}
