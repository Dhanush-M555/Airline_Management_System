#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <stddef.h>
#define MAX_ROUTES 10
#define MAX_FLIGHTS 1000

typedef struct flight {
    char *flight_name;
    char *flight_code;
    char *route[MAX_ROUTES];
    int duration[MAX_ROUTES];
    char *delayed_at;
    int delayed_by;
    char *currently_at;
    int start_timing;
    int active_days[7];
    char * status;
} flight;

typedef struct TrieNode {
    struct TrieNode *children[26];  // Assuming only alphabetical characters
    int isEndOfWord;
    int flightIndex;
} TrieNode;

typedef struct Trie {
    TrieNode *root;
} Trie;

TrieNode* createTrieNode() {
    TrieNode *node = (TrieNode*)malloc(sizeof(TrieNode));
    for (int i = 0; i < 26; i++) {
        node->children[i] = NULL;
    }
    node->isEndOfWord = 0;
    node->flightIndex = -1;
    return node;
}

int my_strcmp(const char *str1, const char *str2) {
    while (*str1 && (*str1 == *str2)) {
        ++str1;
        ++str2;
    }
    return *(unsigned char *)str1 - *(unsigned char *)str2;
}

size_t my_strlen(const char *str) {
    const char *s = str;
    while (*s)
        ++s;
    return s - str;
}

int my_strncmp(const char *str1, const char *str2, size_t n) {
    while (n && *str1 && (*str1 == *str2)) {
        ++str1;
        ++str2;
        --n;
    }
    if (n == 0) {
        return 0;
    }
    return *(unsigned char *)str1 - *(unsigned char *)str2;
}

void insertFlightName(Trie *trie, const char *flightName, int flightIndex) {
    TrieNode *current = trie->root;
    for (int i = 0; flightName[i] != '\0'; i++) {
        int index = flightName[i] - 'a';
        if (!current->children[index]) {
            current->children[index] = createTrieNode();
        }
        current = current->children[index];
    }
    current->isEndOfWord = 1;
    current->flightIndex = flightIndex;
}

void display_all_flights_with_prefix(flight flights[], int flight_count, const char *prefix) {
    const char *days[] = {"Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun"};
    printf("\033[1;34m%-15s%-15s%-15s%-15s%-15s%-15s%-15s%-15s\n",
            "Flight Name", "Flight Code", "Status", "Total Duration", "Delayed At", "Currently At", "Start Timing", "Active Days", "Route");
    for (int i = 0; i < flight_count; i++) {
        if (my_strlen(flights[i].flight_name) >= my_strlen(prefix) &&
            my_strncmp(flights[i].flight_name, prefix, my_strlen(prefix)) == 0) {
            printf("%-15s%-15s", flights[i].flight_name, flights[i].flight_code);
            if (my_strcmp(flights[i].status, "inactive") == 0) {
                printf("\033[1;31m%-15s\033[0m", flights[i].status);
            } else if (my_strcmp(flights[i].status, "on land") == 0) {
                printf("\033[1;32m%-15s\033[0m", flights[i].status);
            } else if (my_strcmp(flights[i].status, "on air") == 0) {
                printf("\033[1;34m%-15s\033[0m", flights[i].status);
            }
            int total_duration = 0;
            for (int j = 0; j < MAX_ROUTES; j++) {
                total_duration += flights[i].duration[j];
            }
            printf("%-15d", total_duration);
            printf("%-15s%-15s%-15d", flights[i].delayed_at, flights[i].currently_at, flights[i].start_timing);
            printf("\033[1;32m");
            for (int j = 0; j < 7; j++) {
                if (flights[i].active_days[j]) {
                    printf("%s ", days[j]);
                }
            }
            printf("\033[0m");
            printf("%-10s"," ");
            for (int j = 0; j < MAX_ROUTES; j++) {
                if(j==0)
                {
                    printf("\033[0m %s", flights[i].route[j]);
                    continue;
                }
                if (my_strlen(flights[i].route[j]) > 0) {
                    printf("\033[1;35m->\033[0m %s", flights[i].route[j]);
                }
            }
            printf("\n");
        }
    }
}

void searchSimilarFlightNames(Trie *trie, const char *prefix, flight flights[], int flight_count) {
    TrieNode *current = trie->root;
    for (int i = 0; prefix[i] != '\0'; i++) {
        int index = prefix[i] - 'a';
        if (!current->children[index]) {
            printf("No flights found with similar names.\n");
            return;
        }
        current = current->children[index];
    }
    display_all_flights_with_prefix(flights, flight_count, prefix);
}

void flush_input_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int get_current_time() {
    time_t rawtime;
    struct tm *info;
    time(&rawtime);
    info = localtime(&rawtime);
    return info->tm_hour * 100 + info->tm_min;
}

void init(flight *f) {
    printf("I am in init\n");
    f->status="inactive";
    f->delayed_at="none";
    f->delayed_by=0;
    printf("I am going out of init\n");
    for (int i = 0; i < MAX_ROUTES; ++i) {
    f->route[i] = malloc(1); // Allocate memory for an empty string
    if (f->route[i] == NULL) {
        // Handle allocation failure
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    f->route[i][0] = '\0'; // Set the string to an empty string
}
}

void update_flight_status(flight *f) {
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);
    int current_time = tm_info->tm_hour * 60 + tm_info->tm_min;
    if (f->active_days[tm_info->tm_wday]) {
        // Check if the current time is within the flight's active period
        if (current_time >= f->start_timing * 60) {
            int route_index = 0;
            // Find the route index based on the current time
            for (int i = 0; i < MAX_ROUTES; i++) {
                if (current_time < f->start_timing * 60 + f->duration[i] * 60) {
                    route_index = i;
                    break;
                }
            }
            f->start_timing += f->delayed_by / 60;
            f->delayed_by = 0;
            f->delayed_at = "none";
            f->currently_at = f->route[route_index];
            if (current_time < f->start_timing * 60 + f->duration[route_index] * 60) {
                f->status = "in air";
            } 
            else {
                f->status = "on land";
            }
        } else {
            // Flight is inactive before its start time
            f->status = "inactive";
            f->currently_at = "none";
        }
    } else {
        // Flight is inactive today
        f->status = "inactive";
        f->currently_at = "none";
    }
}

void get_flight_details(flight *new_flight) {
    new_flight->flight_name = (char *)malloc(256 * sizeof(char));
    new_flight->flight_code = (char *)malloc(256 * sizeof(char));

    int c;
    while ((c = getchar()) != '\n' && c != EOF);

    printf("Enter Flight Name: ");
    scanf("%[^\n]", new_flight->flight_name);
    while ((c = getchar()) != '\n' && c != EOF);

    printf("Enter Flight Code: ");
    scanf("%[^\n]", new_flight->flight_code);
    while ((c = getchar()) != '\n' && c != EOF);

    int routes = 0;
    printf("Enter Route (comma-separated airports): ");
    int i;
    for (i = 0; i < MAX_ROUTES; i++) {
        new_flight->route[i] = (char *)malloc(256 * sizeof(char));
        scanf("%255[^,\n]", new_flight->route[i]);
        routes++;

        int nextChar = getchar();
        if (nextChar == '\n' || nextChar == EOF) {
            break;
        } else if (nextChar != ',') {
            printf("Invalid input format. Expected comma-separated values.\n");
            break;
        }
    }
    printf("routes = %d", routes);
    while ((c = getchar()) != '\n' && c != EOF);

    printf("Enter Duration (comma-separated hours): ");
    for (int i = 0; i < routes - 1; i++) {
        if (i > 0) scanf(",");
        scanf("%d", &new_flight->duration[i]);
    }
    while ((c = getchar()) != '\n' && c != EOF);

    printf("Enter Start Timing (24-hour format): ");
    scanf("%d", &new_flight->start_timing);
    while ((c = getchar()) != '\n' && c != EOF);

    printf("Is the flight active on Monday? (1 for Yes, 0 for No): ");
    scanf("%d", &new_flight->active_days[0]);
    printf("Is the flight active on Tuesday? (1 for Yes, 0 for No): ");
    scanf("%d", &new_flight->active_days[1]);
    printf("Is the flight active on Wednesday? (1 for Yes, 0 for No): ");
    scanf("%d", &new_flight->active_days[2]);
    printf("Is the flight active on Thursday? (1 for Yes, 0 for No): ");
    scanf("%d", &new_flight->active_days[3]);
    printf("Is the flight active on Friday? (1 for Yes, 0 for No): ");
    scanf("%d", &new_flight->active_days[4]);
    printf("Is the flight active on Saturday? (1 for Yes, 0 for No): ");
    scanf("%d", &new_flight->active_days[5]);
    printf("Is the flight active on Sunday? (1 for Yes, 0 for No): ");
    scanf("%d", &new_flight->active_days[6]);
}

void write_flight_details(flight new_flight) {
    FILE *file = fopen("flights.txt", "a");
    if (file == NULL) {
        printf("Error: Could not open the file for writing.\n");
        return;
    }
    fprintf(file, "\"%s\",\"%s\",[%s,%s,%s],[%d,%d,%d],\"%s\",%d,\"%s\",%d,[%d,%d,%d,%d,%d,%d,%d]\n",
        new_flight.flight_name, new_flight.flight_code,
        new_flight.route[0], new_flight.route[1], new_flight.route[2],
        new_flight.duration[0], new_flight.duration[1], new_flight.duration[2],
        new_flight.delayed_at, new_flight.delayed_by,
        new_flight.currently_at, new_flight.start_timing,
        new_flight.active_days[0], new_flight.active_days[1],
        new_flight.active_days[2], new_flight.active_days[3],
        new_flight.active_days[4], new_flight.active_days[5],
        new_flight.active_days[6]);
    fclose(file);
}

void display_all_flights(flight flights[], int flight_count) {
    const char *days[] = {"Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun"};
    printf("\033[1;34m%-15s%-15s%-15s%-15s%-15s%-15s%-15s%-15s%-15s\n",
            "Flight Name", "Flight Code", "Status", "Total Duration", "Delayed At", "Currently At", "Start Timing", "Active Days", "Route");
    for (int i = 0; i < flight_count; i++) {
        printf("%-15s%-15s", flights[i].flight_name, flights[i].flight_code);
        if (my_strcmp(flights[i].status, "inactive") == 0) {
            printf("\033[1;31m%-15s\033[0m", flights[i].status);
        } else if (my_strcmp(flights[i].status, "on land") == 0) {
            printf("\033[1;32m%-15s\033[0m", flights[i].status);
        } else if (my_strcmp(flights[i].status, "on air") == 0) {
            printf("\033[1;34m%-15s\033[0m", flights[i].status);
        }
        int total_duration = 0;
        for (int j = 0; j < MAX_ROUTES; j++) {
            total_duration += flights[i].duration[j];
        }
        printf("%-15d", total_duration);
        printf("%-15s%-15s%-15d", flights[i].delayed_at, flights[i].currently_at, flights[i].start_timing);
        printf("\033[1;32m");
        for (int j = 0; j < 7; j++) {
            if (flights[i].active_days[j]) {
                printf("%s ", days[j]);
            }
        }
        printf("\033[0m");
        printf("%-10s"," ");
        for (int j = 0; j < MAX_ROUTES; j++) 
            {
                if(j==0)
                {
                    printf("\033[0m %s", flights[i].route[j]);
                    continue;
                }
                if (my_strlen(flights[i].route[j]) > 0) {
                    printf("\033[1;35m->\033[0m %s", flights[i].route[j]);
                }
            }
        printf("\n");
    }
}

void display_single_flight_details(flight flights[], int flight_count, const char *search_key) {
    const char *days[] = {"Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"};
    int found = 0;
    printf("\033[1;34m%-15s%-15s%-15s%-15s%-15s%-15s%-15s%-15s\n",
            "Flight Name", "Flight Code", "Status", "Total Duration", "Delayed At", "Currently At", "Start Timing", "Active Days");
    for (int i = 0; i < flight_count; i++) {
        if (my_strcmp(flights[i].flight_name, search_key) == 0 || my_strcmp(flights[i].flight_code, search_key) == 0) {
            found = 1;
            printf("%-15s%-15s", flights[i].flight_name, flights[i].flight_code);
            if (my_strcmp(flights[i].status, "inactive") == 0) {
                printf("\033[1;31m%-15s\033[0m", flights[i].status);
            } else if (my_strcmp(flights[i].status, "on land") == 0) {
                printf("\033[1;32m%-15s\033[0m", flights[i].status);
            } else if (my_strcmp(flights[i].status, "on air") == 0) {
                printf("\033[1;34m%-15s\033[0m", flights[i].status);
            }
            int total_duration = 0;
            for (int j = 0; j < MAX_ROUTES; j++) {
                total_duration += flights[i].duration[j];
            }
            printf("%-15d", total_duration);
            printf("%-15s%-15s%-15d", flights[i].delayed_at, flights[i].currently_at, flights[i].start_timing);
            printf("\033[1;32m");
            for (int j = 0; j < 7; j++) {
                if (flights[i].active_days[j]) {
                    printf("%s ", days[j]);
                }
            }
            printf("\033[0m");
            printf("\n");
            printf("\033[1;35mRoute:\033[0m");
            for (int j = 0; j <MAX_ROUTES; j++) 
            {
                if(j==0)
                {
                    printf("\033[0m %s", flights[i].route[j]);
                    continue;
                }
                if (my_strlen(flights[i].route[j]) > 0) {
                    printf("\033[1;35m->\033[0m %s", flights[i].route[j]);
                }
            }
            printf("\n");
        }
    }
    if (!found) {
        printf("Flight with name or code '%s' not found.\n", search_key);
    }
}

void update_delay_details(flight *f) {
    flush_input_buffer();
    printf("Enter Delayed At: ");
    scanf("%[^\n]", f->delayed_at);
    printf("Enter Delayed By (in minutes): ");
    scanf("%d", &f->delayed_by);
}

int find_flight_index(flight flights[], int flight_count, const char *search_key) {
    for (int i = 0; i < flight_count; i++) {
        if (strcmp(flights[i].flight_name, search_key) == 0 || strcmp(flights[i].flight_code, search_key) == 0) {
            return i;  // Return the index if a match is found
        }
    }
    return -1;  // Return -1 if no match is found
}

void freeTrie(TrieNode *node) {
    if (!node) {
        return;
    }
    for (int i = 0; i < 26; i++) {
        freeTrie(node->children[i]);
    }
    free(node);
}

int main() {
    system("cls");
    Trie trie;
    trie.root = createTrieNode();
    flight flights[MAX_FLIGHTS];
    int flight_count = 0;
    flight new_flight;
    int choice;
    char search_key[256];
    while (1) 
    {
        printf("\nPress:\n");
        printf("1: To add new flight details\n");
        printf("2: To display all flights\n");
        printf("3: To display flight with certain name or code\n");
        printf("4: To Update flight delay details\n");
        printf("5: To search for flights with similar names\n");
        printf("0: To exit\n");
        printf("Your choice: ");
        scanf("%d", &choice);
        switch (choice) {
            case 1:
                init(&new_flight);
                get_flight_details(&new_flight);
                update_flight_status(&new_flight);
                insertFlightName(&trie, new_flight.flight_name, flight_count);
                flights[flight_count] = new_flight;
                flight_count++;
                write_flight_details(new_flight);
                break;
            case 2:
                system("cls");
                if (flight_count > 0) {
                for (int i = 0; i < flight_count; i++) 
                {
                    update_flight_status(&flights[i]);
                    }
                }
                if (flight_count > 0) {
                    display_all_flights(flights, flight_count);
                } else {
                    printf("No flights available.\n");
                }
                break;
            case 3:
                system("cls");
                if (flight_count > 0) {
                for (int i = 0; i < flight_count; i++) 
                {
                    update_flight_status(&flights[i]);
                    }
                }
                if (flight_count > 0) {
                    printf("Enter flight name or code: ");
                    scanf("%s", search_key);
                    display_single_flight_details(flights, flight_count, search_key);
                } else {
                    printf("No flights available.\n");
                }
                break;
            case 4:
                if (flight_count > 0) {
                    printf("Enter flight name or code for delay details: ");
                    scanf("%s", search_key);
                    int flight_index = find_flight_index(flights, flight_count, search_key);
                    if (flight_index != -1) {
                        update_delay_details(&flights[flight_index]);
                        update_flight_status(&flights[flight_index]);
                        printf("Delay details updated successfully.\n");
                    } else {
                        printf("Flight with name or code '%s' not found.\n", search_key);
                    }
                } else {
                    printf("No flights available.\n");
                }
                break;
            case 5:
            if (flight_count > 0) {
                printf("Enter a prefix to search for flights with similar names: ");
                scanf("%s", search_key);
                searchSimilarFlightNames(&trie, search_key, flights, flight_count);
                } else {
                printf("No flights available.\n");
                }
            break;
            case 0:
                printf("Exiting program.\n");
                freeTrie(trie.root);
                exit(0);
            default:
                printf("Invalid choice. Please enter a valid option.\n");
        }
        if (flight_count > 0) {
            for (int i = 0; i < flight_count; i++) {
                update_flight_status(&flights[i]);
            }
        }
        sleep(3);
    }
    return 0;
}


