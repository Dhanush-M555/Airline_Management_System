#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <ctype.h>
#include <string.h>
#include <windows.h>

#define MAX_AIRPORTS 100
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE   "\x1b[34m"
#define ANSI_COLOR_RED    "\x1b[31m"
#define ANSI_COLOR_RESET   "\x1b[0m"

struct Airport {
    char name[50];
};

struct Edge {
    int dest;
    int time;
    struct Edge* next;
};


struct Vertex {
    struct Airport airport;
    struct Edge* edges;
};

struct Graph {
    int numAirports;
    struct Vertex* vertices;
};

void toLowerCase(char* str) {
    for (int i = 0; str[i]; i++) {
        str[i] = tolower(str[i]);
    }
}

struct Graph* initializeGraph(int numAirports) {
    struct Graph* graph = (struct Graph*)malloc(sizeof(struct Graph));
    graph->numAirports = numAirports;
    graph->vertices = (struct Vertex*)malloc(numAirports * sizeof(struct Vertex));
    for (int i = 0; i < numAirports; i++) {
        graph->vertices[i].edges = NULL;
    }
    return graph;
}

void addEdge(struct Graph* graph, int src, int dest, int time) {
    struct Edge* newEdge = (struct Edge*)malloc(sizeof(struct Edge));
    newEdge->dest = dest;
    newEdge->time = time;
    newEdge->next = graph->vertices[src].edges;
    graph->vertices[src].edges = newEdge;
}

int findAirportIndex(struct Graph* graph, const char* airportName) {
    char lowercaseName[50];
    strcpy(lowercaseName, airportName);
    toLowerCase(lowercaseName);
    for (int i = 0; i < graph->numAirports; i++) {
        char currentAirportName[50];
        strcpy(currentAirportName, graph->vertices[i].airport.name);
        toLowerCase(currentAirportName);
        if (strcmp(lowercaseName, currentAirportName) == 0) {
            return i;
        }
    }
    return -1;
}

void readInputFromFile(struct Graph* graph, const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error opening file %s.\n", filename);
        exit(EXIT_FAILURE);
    }
    fscanf(file, "%d", &graph->numAirports);
    for (int i = 0; i < graph->numAirports; i++) {
        fscanf(file, "%s", graph->vertices[i].airport.name);
        toLowerCase(graph->vertices[i].airport.name);
    }
    int numRoutes;
    fscanf(file, "%d", &numRoutes);
    for (int i = 0; i < numRoutes; i++) {
        char srcAirport[50], destAirport[50];
        int time;
        fscanf(file, "%s %s %d", srcAirport, destAirport, &time);
        toLowerCase(srcAirport);
        toLowerCase(destAirport);
        int srcIndex = findAirportIndex(graph, srcAirport);
        int destIndex = findAirportIndex(graph, destAirport);
        if (srcIndex != -1 && destIndex != -1) {
            addEdge(graph, srcIndex, destIndex, time);
            addEdge(graph, destIndex, srcIndex, time); // Assuming bidirectional routes
        } else {
            printf("Invalid airport names in the file. Please check your input.\n");
            exit(EXIT_FAILURE);
        }
    }
    fclose(file);
}

void dijkstra(struct Graph* graph, int src, int* time, int* prev) {
    int visited[MAX_AIRPORTS] = {0};
    for (int i = 0; i < graph->numAirports; i++) {
        time[i] = INT_MAX;
        prev[i] = -1;
    }
    time[src] = 0;
    for (int count = 0; count < graph->numAirports - 1; count++) {
        int minTime = INT_MAX;
        int minIndex;
        for (int i = 0; i < graph->numAirports; i++) {
            if (!visited[i] && time[i] < minTime) {
                minTime = time[i];
                minIndex = i;
            }
        }
        visited[minIndex] = 1;
        struct Edge* currentEdge = graph->vertices[minIndex].edges;
        while (currentEdge != NULL) {
            int newTime = time[minIndex] + currentEdge->time;  // Changed from 'weight' to 'time'
            if (!visited[currentEdge->dest] && newTime < time[currentEdge->dest]) {
                time[currentEdge->dest] = newTime;
                prev[currentEdge->dest] = minIndex;
            }
            currentEdge = currentEdge->next;
        }
    }
}

void setConsoleColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void printLine(int length, int color) {
    setConsoleColor(color);
    for (int i = 0; i < length; i++) {
        putchar('*');
    }
    printf("\n");
    setConsoleColor(7);
}

void printColoredPath(struct Graph* graph, int* prev, int current) {
    if (prev[current] != -1) {
        printColoredPath(graph, prev, prev[current]);
    }
    if (current % 2 == 0) {
        printf(ANSI_COLOR_CYAN "%s" ANSI_COLOR_YELLOW "->" ANSI_COLOR_RESET, graph->vertices[current].airport.name);
    } else {
        printf(ANSI_COLOR_GREEN "%s" ANSI_COLOR_YELLOW "->" ANSI_COLOR_RESET, graph->vertices[current].airport.name);
    }
}

int main() {
    int numAirports;
    struct Graph* graph = initializeGraph(MAX_AIRPORTS);
    readInputFromFile(graph, "airport_input.txt");
    char sourceAirport[50], destAirport[50];
    printLine(28, 10);
    setConsoleColor(11);
    printf("Enter the source airport: ");
    scanf("%s", sourceAirport);
    printf("Enter the destination airport: ");
    scanf("%s", destAirport);
    printLine(28, 10);
    int sourceIndex = findAirportIndex(graph, sourceAirport);
    int destIndex = findAirportIndex(graph, destAirport);
    if (sourceIndex != -1 && destIndex != -1) {
        int time[MAX_AIRPORTS];
        int prev[MAX_AIRPORTS];
        dijkstra(graph, sourceIndex, time, prev);
        if (time[destIndex] != INT_MAX) {
            printf("Shortest time from %s to %s:" ANSI_COLOR_RED " %d minutes\n", sourceAirport, destAirport, time[destIndex]);
            printf(ANSI_COLOR_BLUE "Path: ");
            printColoredPath(graph, prev, destIndex);
            printf("\n");
        } else {
            printf("No path found from %s to %s.\n", sourceAirport, destAirport);
        }
    } else {
        printf("Invalid airport names. Please check your input.\n");
    }
    free(graph->vertices);
    free(graph);
    return 0;
}
