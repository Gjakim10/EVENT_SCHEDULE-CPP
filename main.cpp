#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <sstream>
#include <iomanip>
#include <limits> // For input clearing

using namespace std;

// Structure for an Event
struct Event {
    string title;
    string date; // Format: YYYY-MM-DD
    string time; // Format: HH:MM

    // Overload the < operator for sorting
    bool operator<(const Event &e) const {
        if (date == e.date)
            return time < e.time;
        return date < e.date;
    }
};

// Function Prototypes
void loadEvents(vector<Event> &events);
void saveEvents(const vector<Event> &events);
void addEvent(vector<Event> &events);
void viewEvents(const vector<Event> &events);
void editEvent(vector<Event> &events);
void deleteEvent(vector<Event> &events);
void searchEvent(const vector<Event> &events);
void clearInputStream();
bool validateDate(const string &date);
bool validateTime(const string &time);

int main() {
    vector<Event> events;
    loadEvents(events); // Load events from file at startup

    // Welcome message
    cout << "======================================\n";
    cout << "    Welcome to the Event Scheduler!   \n";
    cout << "======================================\n";
    cout << "Press Enter to continue...\n";
    cin.ignore(); // Wait for Enter key
    cin.get();

    int choice;
    do {
        cout << "\nEvent Scheduler Menu\n";
        cout << "1. Add Event\n";
        cout << "2. View Events\n";
        cout << "3. Edit Event\n";
        cout << "4. Delete Event\n";
        cout << "5. Search Event\n";
        cout << "6. View Total Events\n";
        cout << "7. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                addEvent(events);
                saveEvents(events); // Save after every change
                break;
            case 2:
                viewEvents(events);
                break;
            case 3:
                editEvent(events);
                saveEvents(events);
                break;
            case 4:
                deleteEvent(events);
                saveEvents(events);
                break;
            case 5:
                searchEvent(events);
                break;
            case 6:
                cout << "Total events scheduled: " << events.size() << "\n";
                break;
            case 7:
                cout << "Thank you for using the Event Scheduler! Goodbye!\n";
                break;
            default:
                cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 7);

    return 0;
}

// Helper: Clear input stream
void clearInputStream() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

// Helper: Validate date format (YYYY-MM-DD)
bool validateDate(const string &date) {
    if (date.size() != 10 || date[4] != '-' || date[7] != '-')
        return false;
    return isdigit(date[0]) && isdigit(date[1]) && isdigit(date[2]) &&
           isdigit(date[3]) && isdigit(date[5]) && isdigit(date[6]) &&
           isdigit(date[8]) && isdigit(date[9]);
}

// Helper: Validate time format (HH:MM)
bool validateTime(const string &time) {
    if (time.size() != 5 || time[2] != ':')
        return false;
    return isdigit(time[0]) && isdigit(time[1]) && isdigit(time[3]) &&
           isdigit(time[4]);
}

// Load events from a file
void loadEvents(vector<Event> &events) {
    ifstream file("events.txt");
    if (!file.is_open()) return;

    Event event;
    while (getline(file, event.title)) {
        getline(file, event.date);
        getline(file, event.time);
        events.push_back(event);
    }

    file.close();
    sort(events.begin(), events.end()); // Sort events after loading
}

// Save events to a file
void saveEvents(const vector<Event> &events) {
    ofstream file("events.txt");
    for (const auto &event : events) {
        file << event.title << "\n" << event.date << "\n" << event.time << "\n";
    }
    file.close();
}

// Add a new event
void addEvent(vector<Event> &events) {
    Event newEvent;
    clearInputStream();

    cout << "Enter event title: ";
    getline(cin, newEvent.title);

    do {
        cout << "Enter event date (YYYY-MM-DD): ";
        cin >> newEvent.date;
    } while (!validateDate(newEvent.date));

    do {
        cout << "Enter event time (HH:MM): ";
        cin >> newEvent.time;
    } while (!validateTime(newEvent.time));

    events.push_back(newEvent);
    sort(events.begin(), events.end()); // Keep events sorted
    cout << "Event added successfully!\n";
}

// View all events
void viewEvents(const vector<Event> &events) {
    if (events.empty()) {
        cout << "No events scheduled.\n";
        return;
    }

    cout << "\nScheduled Events:\n";
    for (const auto &event : events) {
        cout << event.date << " " << event.time << " - " << event.title << "\n";
    }
}

// Edit an existing event
void editEvent(vector<Event> &events) {
    string titleToEdit;
    clearInputStream();
    cout << "Enter the title of the event to edit: ";
    getline(cin, titleToEdit);

    for (auto &event : events) {
        if (event.title == titleToEdit) {
            cout << "Editing Event: " << event.title << "\n";

            cout << "Enter new title (or press Enter to keep unchanged): ";
            string newTitle;
            getline(cin, newTitle);
            if (!newTitle.empty()) event.title = newTitle;

            cout << "Enter new date (YYYY-MM-DD, or press Enter to keep unchanged): ";
            string newDate;
            getline(cin, newDate);
            if (!newDate.empty() && validateDate(newDate)) event.date = newDate;

            cout << "Enter new time (HH:MM, or press Enter to keep unchanged): ";
            string newTime;
            getline(cin, newTime);
            if (!newTime.empty() && validateTime(newTime)) event.time = newTime;

            sort(events.begin(), events.end()); // Re-sort after editing
            cout << "Event updated successfully!\n";
            return;
        }
    }

    cout << "Event not found.\n";
}

// Delete an event
void deleteEvent(vector<Event> &events) {
    string titleToDelete;
    clearInputStream();
    cout << "Enter the title of the event to delete: ";
    getline(cin, titleToDelete);

    auto it = remove_if(events.begin(), events.end(),
                        [&titleToDelete](const Event &event) {
                            return event.title == titleToDelete;
                        });

    if (it != events.end()) {
        events.erase(it, events.end());
        cout << "Event deleted successfully!\n";
    } else {
        cout << "Event not found.\n";
    }
}

// Search for an event
void searchEvent(const vector<Event> &events) {
    string titleToSearch;
    clearInputStream();
    cout << "Enter the title of the event to search: ";
    getline(cin, titleToSearch);

    bool found = false;
    for (const auto &event : events) {
        if (event.title.find(titleToSearch) != string::npos) {
            cout << event.date << " " << event.time << " - " << event.title << "\n";
            found = true;
        }
    }

    if (!found) {
        cout << "No events matching \"" << titleToSearch << "\" were found.\n";
    }
}

