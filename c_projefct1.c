#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define the structure for a flight
typedef struct Flight {
    char fromPlace[50];
    char toPlace[50];
    // Add more flight details here
    struct Flight* left;
    struct Flight* right;
} Flight;

// Function to create a new flight node
Flight* createFlight(const char* fromPlace, const char* toPlace) {
    Flight* newFlight = (Flight*)malloc(sizeof(Flight));
    strcpy(newFlight->fromPlace, fromPlace);
    strcpy(newFlight->toPlace, toPlace);
    newFlight->left = NULL;
    newFlight->right = NULL;
    return newFlight;
}

// Function to insert a new flight into the binary search tree
Flight* insertFlight(Flight* root, const char* fromPlace, const char* toPlace) {
    if (root == NULL) {
        return createFlight(fromPlace, toPlace);
    }

    int cmp = strcmp(fromPlace, root->fromPlace);
    if (cmp < 0) {
        root->left = insertFlight(root->left, fromPlace, toPlace);
    } else if (cmp > 0) {
        root->right = insertFlight(root->right, fromPlace, toPlace);
    } else {
        // Handle cases where fromPlace is the same
        cmp = strcmp(toPlace, root->toPlace);
        if (cmp < 0) {
            root->left = insertFlight(root->left, fromPlace, toPlace);
        } else {
            root->right = insertFlight(root->right, fromPlace, toPlace);
        }
    }

    return root;
}

// Function to perform an in-order traversal of the binary search tree
void inOrderTraversal(Flight* root) {
    if (root != NULL) {
        inOrderTraversal(root->left);
        printf("From: %s, To: %s\n", root->fromPlace, root->toPlace);
        inOrderTraversal(root->right);
    }
}

// Function to search for a flight by place names
Flight* searchFlight(Flight* root, const char* fromPlace, const char* toPlace) {
    if (root == NULL) {
        return NULL;
    }

    int cmpFrom = strcmp(fromPlace, root->fromPlace);
    int cmpTo = strcmp(toPlace, root->toPlace);

    if (cmpFrom == 0 && cmpTo == 0) {
        return root;
    } else if (cmpFrom < 0 || (cmpFrom == 0 && cmpTo < 0)) {
        return searchFlight(root->left, fromPlace, toPlace);
    } else {
        return searchFlight(root->right, fromPlace, toPlace);
    }
}

int main() {
    Flight* root = NULL;

    // Insert flights into the binary search tree
    root = insertFlight(root, "New York", "Los Angeles");
    root = insertFlight(root, "Chicago", "Dallas");
    root = insertFlight(root, "San Francisco", "Seattle");
    root = insertFlight(root, "Boston", "Miami");

    // Perform an in-order traversal to display flight information
    printf("Flight Information:\n");
    inOrderTraversal(root);

    // Allow the user to search for a flight by place names
    char fromPlace[50];
    char toPlace[50];
    printf("\nEnter the 'From' place: ");
    scanf("%s", fromPlace);
    printf("Enter the 'To' place: ");
    scanf("%s", toPlace);

    Flight* result = searchFlight(root, fromPlace, toPlace);
    if (result != NULL) {
        printf("Flight from %s to %s found!\n", fromPlace, toPlace);
    } else {
        printf("Flight from %s to %s not found.\n", fromPlace, toPlace);
    }

    return 0;
}
