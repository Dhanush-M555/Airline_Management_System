#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_FLIGHTS 10
#define MAX_SEATS 50

// Define the structure for a flight
typedef struct Flight {
    int flightNumber;
    int availableSeats[MAX_SEATS];
} Flight;

// Create an array to store flight data
Flight flights[MAX_FLIGHTS];

// Initialize flight data
void initializeFlights() {
    for (int i = 0; i < MAX_FLIGHTS; i++) {
        flights[i].flightNumber = 1001 + i;
        for (int j = 0; j < MAX_SEATS; j++) {
            flights[i].availableSeats[j] = 1; // 1 represents available, 0 represents booked
        }
    }
}

// Display available flights
void displayFlights() {
    printf("Available Flights:\n");
    for (int i = 0; i < MAX_FLIGHTS; i++) {
        printf("Flight %d\n", flights[i].flightNumber);
    }
}

// Function to book a seat on a flight
bool bookSeat(int flightIndex, int seatNumber) {
    if (flightIndex < 0 || flightIndex >= MAX_FLIGHTS || seatNumber < 1 || seatNumber > MAX_SEATS) {
        return false; // Invalid flight or seat
    }

    if (flights[flightIndex].availableSeats[seatNumber - 1] == 1) {
        flights[flightIndex].availableSeats[seatNumber - 1] = 0; // Book the seat
        return true; // Booking successful
    } else {
        return false; // Seat is already booked
    }
}

int main() {
    initializeFlights();
    
    int choice;
    do {
        printf("\n1. Display Available Flights\n");
        printf("2. Book a Seat\n");
        printf("3. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                displayFlights();
                break;

            case 2:
                int flightIndex, seatNumber;
                printf("Enter the flight index (0-9): ");
                scanf("%d", &flightIndex);
                printf("Enter the seat number (1-50): ");
                scanf("%d", &seatNumber);

                if (bookSeat(flightIndex, seatNumber)) {
                    printf("Seat booked successfully!\n");
                } else {
                    printf("Seat booking failed. The seat may already be booked or the flight is invalid.\n");
                }
                break;

            case 3:
                printf("Exiting program.\n");
                break;

            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 3);

    return 0;
}
