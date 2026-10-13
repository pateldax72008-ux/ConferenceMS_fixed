// main.cpp - only shows the menu and calls Conference / FileManager functions.
#include "Conference.h"
#include "FileManager.h"

void showMenu() {
    cout << "\n===== CONFERENCE MANAGEMENT SYSTEM =====\n"
         << "1. Add Record\n2. Display Records\n3. Search Record\n4. Update Record\n"
         << "5. Delete Record\n6. Register / Cancel (Main Transaction)\n7. Report\n"
         << "8. Save / Load\n9. Exit\n";
}

int pickType() {
    cout << "  1. Speaker  2. Participant  3. Session\n";
    return readInt("  Choose type: ");
}

void addMenu(Conference& c) {
    int t = pickType();
    if (t == 1) {
        string n = readLine("Name: "), topic = readLine("Topic: ");
        int id = c.addSpeaker(n, topic);
        cout << "Speaker added with ID " << id << ".\n";
    } else if (t == 2) {
        string n = readLine("Name: "), ph = readLine("Phone (10 digits): ");
        int id = c.addParticipant(n, ph);
        cout << "Participant added with ID " << id << ".\n";
    } else if (t == 3) {
        string title = readLine("Title: ");
        int spk = readInt("Speaker ID: ");
        string tm = readLine("Time (HH:MM): ");
        int cap = readInt("Capacity: ");
        int id = c.scheduleSession(title, spk, tm, cap);
        cout << "Session scheduled with ID " << id << ".\n";
    } else cout << "Invalid type.\n";
}

void displayMenu(Conference& c) {
    cout << "  1. Speakers  2. Participants  3. Sessions  4. Registrations  5. All people\n";
    int t = readInt("  Choose: ");
    if (t == 1) c.displaySpeakers();
    else if (t == 2) c.displayParticipants();
    else if (t == 3) c.displaySessions();
    else if (t == 4) c.displayRegistrations();
    else if (t == 5) c.displayAllPeople();
    else cout << "Invalid choice.\n";
}

void searchMenu(Conference& c) {
    int t = pickType();
    if (t < 1 || t > 3) { cout << "Invalid type.\n"; return; }
    string k = readLine("Search keyword or ID: ");
    if (k.empty()) { cout << "Keyword cannot be empty.\n"; return; }
    if (t == 1) c.searchSpeakers(k);
    else if (t == 2) c.searchParticipants(k);
    else c.searchSessions(k);
}

void updateMenu(Conference& c) {
    int t = pickType();
    if (t < 1 || t > 3) { cout << "Invalid type.\n"; return; }
    int id = readInt("ID to update: ");
    cout << "(press Enter to keep the old value)\n";
    if (t == 1) {
        string n = readLine("New name: "), tp = readLine("New topic: ");
        c.updateSpeaker(id, n, tp);
    } else if (t == 2) {
        string n = readLine("New name: "), ph = readLine("New phone: ");
        c.updateParticipant(id, n, ph);
    } else {
        string ti = readLine("New title: "), tm = readLine("New time (HH:MM): ");
        string cp = readLine("New capacity: ");
        int cap = -1;
        if (!cp.empty()) {
            if (cp.find_first_not_of("0123456789") != string::npos || cp.size() > 8)
                throw invalid_argument("Capacity must be a number.");
            cap = stoi(cp);
        }
        c.updateSession(id, ti, tm, cap);
    }
    cout << "Record updated.\n";
}

void deleteMenu(Conference& c) {
    int t = pickType();
    if (t < 1 || t > 3) { cout << "Invalid type.\n"; return; }
    int id = readInt("ID to delete: ");
    string sure = readLine("Are you sure? (y/n): ");
    if (sure != "y" && sure != "Y") { cout << "Cancelled.\n"; return; }
    if (t == 1) c.deleteSpeaker(id);
    else if (t == 2) c.deleteParticipant(id);
    else c.deleteSession(id);
    cout << "Record deleted.\n";
}

void transactionMenu(Conference& c) {
    cout << "  1. Register participant to session  2. Cancel a registration\n";
    int t = readInt("  Choose: ");
    if (t == 1) {
        int p = readInt("Participant ID: "), s = readInt("Session ID: ");
        int rid = c.registerParticipant(p, s);
        cout << "Registered! Registration ID: " << rid << "\n";
    } else if (t == 2) {
        c.cancelRegistration(readInt("Registration ID: "));
        cout << "Registration cancelled.\n";
    } else cout << "Invalid choice.\n";
}

void saveLoadMenu(Conference& c, FileManager& fm) {
    cout << "  1. Save now  2. Load from file (replaces current data)\n";
    int t = readInt("  Choose: ");
    if (t == 1) fm.save(c);
    else if (t == 2) fm.load(c);
    else cout << "Invalid choice.\n";
}

int main() {
    Conference conf;
    FileManager fm("Data/");

    try { fm.load(conf); }                       // START: load existing records
    catch (const exception& e) { cout << "Load error: " << e.what() << "\n"; }

    int choice = 0;
    do {
        showMenu();
        choice = readInt("Enter choice: ");
        try {
            switch (choice) {
                case 1: addMenu(conf); break;
                case 2: displayMenu(conf); break;
                case 3: searchMenu(conf); break;
                case 4: updateMenu(conf); break;
                case 5: deleteMenu(conf); break;
                case 6: transactionMenu(conf); break;
                case 7: conf.report(); break;
                case 8: saveLoadMenu(conf, fm); break;
                case 9: fm.save(conf); cout << "Goodbye!\n"; break;   // EXIT: save records
                default: cout << "Invalid choice. Enter 1-9.\n";
            }
        } catch (const exception& e) {               // EXCEPTION HANDLING
            cout << "Error: " << e.what() << "\n";
        }
    } while (choice != 9);
    return 0;
}
