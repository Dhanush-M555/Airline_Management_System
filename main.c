#include <stdio.h>
void displayHomePage() {
    // Clear the console screen
    printf("\033[2J\033[H");

    // Set background color
    printf("\033[42m");

    // Set text color
    printf("\033[1;34m");

    // Print decorative line
    printf("*****************************************\n");

    // Print centered title
    printf("          Welcome to FliteLogix          \n");

    // Print decorative line
    printf("*****************************************\n");

    // Reset colors
    printf("\033[0m");

    // Print options horizontally
    printf("\n  1. Book a Flight   2. View Booking   3. Cancel Booking   4. Exit\n\n");
}

int main() {
    displayHomePage();
    
    return 0;
}
