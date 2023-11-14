#include <stdio.h>
#include <string.h>
#include <time.h>

// structure for an event
typedef struct {
    // YYYY,MM,DD
    char date[11]; 
    char type[50];
    char category[60];
    char description[100];
} Event;

// array for events
Event events[100];
int eventCount = 0;

// basic verdion of functions
void displayMenu();// did this one, have to implement others
void loadDatabase();
void addEvent();
void saveDatabase();
void displayEventsByCategory();
void displayEventsByDate();
void displayEventsLastAndNextMonth();

int main() {
    int choice;
    do {
        displayMenu();
        scanf("%d", &choice);
        switch (choice) {
            case 1: loadDatabase(); break;
            case 2: addEvent(); break;
            case 3: saveDatabase(); break;
            case 4: displayEventsByCategory(); break;
            case 5: displayEventsByDate(); break;
            case 6: displayEventsLastAndNextMonth(); break;
            case 7: printf("Exiting program.\n"); break;
            default: printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 7);

    return 0;
}

void displayMenu() {
    printf("\n--- Menu ---\n");
    printf("1. Load existing database\n");
    printf("2. Add new events\n");
    printf("3. Save database\n");
    printf("4. Display an events belong to a category\n");
    printf("5. Display all events for selected day\n");
    printf("6. display the event of last month that wil happen next week as well\n");
    printf("7. End\n");
    printf("Enter your choice: ");
}

void addEvent() {
    if (eventCount >= 100) {
        printf("Event limit reached!!!!!\n");
        return;
    }

    Event newEvent;
    
    printf("Enter the date (YYYY-MM-DD): ");
    scanf("%s", newEvent.date); // use fgets in future???****
    
    printf("Enter type (birthday, nameday, or other anniversary): ");
    scanf("%s", newEvent.type);

    printf("Enter a category (like family, relative, etc.): ");
    scanf("%s", newEvent.category);

    printf("Enter the name or description: ");
    scanf("%s", newEvent.description);

    // Adding the new event to the array
    events[eventCount] = newEvent;
    eventCount++;

    printf("Event added successfully!\n");
}


// other functions wirte in there:


