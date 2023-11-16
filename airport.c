#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Maximum number of vertices (airports) in the graph
#define MAX_VERTICES 100

// Maximum length of an airport name
#define MAX_AIRPORT_NAME 50

// Structure to represent a node in the adjacency list
struct Node {
    char airport[MAX_AIRPORT_NAME];
    struct Node* next;
};

// Structure to represent the graph
struct Graph {
    int vertices;
    struct Node* adjacencyList[MAX_VERTICES];
};

// Function prototypes
int getIndex(struct Graph* graph, const char* airport);
void addEdge(struct Graph* graph, const char* src, const char* dest);
int bfs(struct Graph* graph, const char* start, const char* end, char path[MAX_VERTICES][1000]);
void trim(char* str);
void printUniqueNodes(struct Graph* graph);

// Function to print all unique nodes in the graph
void printUniqueNodes(struct Graph* graph) {
    printf("Debug: Inside printUniqueNodes\n");
    printf("Unique nodes in the graph: ");
    for (int i = 0; i < graph->vertices; ++i) {
        printf("%s ", graph->adjacencyList[i]->airport);
    }
    printf("\n");
}

// Function to get the index of an airport in the graph
int getIndex(struct Graph* graph, const char* airport) {
    printf("Debug: Inside getIndex\n");
    for (int i = 0; i < graph->vertices; ++i) {
        if (strcmp(graph->adjacencyList[i]->airport, airport) == 0) {
            return i;
        }
    }
    return -1; // Airport not found in the graph
}

// Function to add an edge to the graph
void addEdge(struct Graph* graph, const char* src, const char* dest) {
    printf("Debug: Inside addEdge\n");
    int srcIndex = getIndex(graph, src);
    int destIndex = getIndex(graph, dest);

    // Create a new node for the destination and add it to the adjacency list of the source
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    if (!newNode) {
        printf("Memory allocation error for newNode\n");
        exit(1);
    }
    strcpy(newNode->airport, dest);
    newNode->next = graph->adjacencyList[srcIndex];
    graph->adjacencyList[srcIndex] = newNode;

    // Assuming the graph is undirected, so we add an edge from dest to src as well
    newNode = (struct Node*)malloc(sizeof(struct Node));
    if (!newNode) {
        printf("Memory allocation error for newNode\n");
        exit(1);
    }
    strcpy(newNode->airport, src);
    newNode->next = graph->adjacencyList[destIndex];
    graph->adjacencyList[destIndex] = newNode;
}

// Function to perform Breadth-First Search to find the shortest path
int bfs(struct Graph* graph, const char* start, const char* end, char path[MAX_VERTICES][1000]) {
    printf("Debug: Inside bfs\n");
    // ... (no changes to this function)
}

// Function to trim leading and trailing spaces from a string
void trim(char* str) {
    printf("Debug: Inside trim\n");
    int start = 0, end = strlen(str) - 1;
    while (isspace(str[start])) {
        start++;
    }
    while (end > start && isspace(str[end])) {
        end--;
    }
    memmove(str, str + start, end - start + 1);
    str[end - start + 1] = '\0';
}

int main() {
    printf("I am in main\n");
    FILE* file = fopen("airport_data.txt", "r,ccs=UTF-8");
    if (!file) {
        printf("Error opening file.\n");
        return 1;
    }
    printf("Opened the file\n");
    struct Graph graph;
    graph.vertices = 0;
    printf("check 1\n");
    // Read data from the text file and construct the graph
    char line[256];
    while (fgets(line, sizeof(line), file)) {
        char* airport = strtok(line, ",");
        while (airport) {
            trim(airport); // Trim leading and trailing spaces
            int index = getIndex(&graph, airport);
            if (index == -1) {
                if (graph.vertices >= MAX_VERTICES) {
                    printf("Error: Too many vertices in the graph.\n");
                    fclose(file);
                    return 1;
                }
                graph.adjacencyList[graph.vertices] = (struct Node*)malloc(sizeof(struct Node));
                if (!graph.adjacencyList[graph.vertices]) {
                    printf("Memory allocation error for graph.adjacencyList\n");
                    fclose(file);
                    return 1;
                }
                strcpy(graph.adjacencyList[graph.vertices]->airport, airport);
                graph.adjacencyList[graph.vertices]->next = NULL;
                graph.vertices++;
            }
            printf("Read airport: %s\n", airport);
            airport = strtok(NULL, ",");
        }
    }
    printf("check 2\n");
    // Reset the file position indicator to the beginning of the file
    fseek(file, 0, SEEK_SET);
    printf("check 3\n");
    // Add edges to the graph based on text data
    while (fgets(line, sizeof(line), file)) {
        char* src = strtok(line, ",");
        char* dest = strtok(NULL, ",");
        while (dest) {
            dest[strcspn(dest, "\n")] = 0; // Remove newline character
            trim(src);
            trim(dest);
            printf("Adding edge: %s -> %s\n", src, dest);
            addEdge(&graph, src, dest);
            dest = strtok(NULL, ",");
        }
    }
    printf("check 4\n");
    fclose(file);

    // Print all unique nodes in the graph
    printUniqueNodes(&graph);

    // User input
    char from[MAX_AIRPORT_NAME], to[MAX_AIRPORT_NAME];
    printf("Enter the source airport: ");
    scanf("%s", from);
    printf("Enter the destination airport: ");
    scanf("%s", to);

    // Trim leading and trailing spaces from user input
    trim(from);
    trim(to);

    // Check if airports exist in the graph
    int fromIndex = getIndex(&graph, from);
    int toIndex = getIndex(&graph, to);
    if (fromIndex == -1 || toIndex == -1) {
        printf("One or both airports do not exist in the graph.\n");
        return 1;
    }

    // Initialize path array
    char path[MAX_VERTICES][1000] = {0};
    strcat(path[fromIndex], from);

    // Find the shortest path using BFS
    if (bfs(&graph, from, to, path)) {
        printf("Shortest path from %s to %s: %s\n", from, to, path[toIndex]);
    } else {
        printf("No path found from %s to %s.\n", from, to);
    }

    return 0;
}