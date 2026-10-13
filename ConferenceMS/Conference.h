// Conference.h - the SYSTEM class. Owns all data in DYNAMIC ARRAYS and does all the logic.
#pragma once
#include "Speaker.h"
#include "Participant.h"
#include "Session.h"
#include "Registration.h"

class Conference {
private:
    static const int INITIAL_CAP = 4;

    // Dynamic arrays: size is unknown in advance, so we allocate with new[]
    // and grow them (double the size) when full. Released with delete[] in destructor.
    Speaker*      speakers;      int speakerCount,      speakerCap;
    Participant*  participants;  int participantCount,  participantCap;
    Session*      sessions;      int sessionCount,      sessionCap;
    Registration* registrations; int registrationCount, registrationCap;

    int nextSpeakerId, nextParticipantId, nextSessionId, nextRegistrationId;

    // Make a bigger array, copy old items, free the old one
    template <class T>
    static void grow(T*& arr, int count, int& cap) {
        int newCap = cap * 2;
        T* bigger = new T[newCap];
        for (int i = 0; i < count; i++) bigger[i] = arr[i];
        delete[] arr;
        arr = bigger;
        cap = newCap;
    }
    // Delete item at idx by shifting everything after it one step left
    template <class T>
    static void removeAt(T* arr, int& count, int idx) {
        for (int i = idx; i < count - 1; i++) arr[i] = arr[i + 1];
        count--;
    }

    // return index in array, or -1 if not found
    int findSpeaker(int id) const;
    int findParticipant(int id) const;
    int findSession(int id) const;
    int findRegistration(int id) const;
    int countActive(int sessionId) const;
    string speakerName(int id) const;

    friend class FileManager;   // FileManager is allowed to fill our arrays when loading

public:
    Conference();
    ~Conference();
    Conference(const Conference&) = delete;             // prevent accidental copy
    Conference& operator=(const Conference&) = delete;  // (would double-delete memory)

    void clearAll();

    // 1. ADD
    int addSpeaker(const string& name, const string& topic);
    int addParticipant(const string& name, const string& phone);
    int scheduleSession(const string& title, int speakerId, const string& time, int capacity);

    // 2. DISPLAY
    void displaySpeakers() const;
    void displayParticipants() const;
    void displaySessions() const;
    void displayRegistrations() const;
    void displayAllPeople() const;      // runtime polymorphism demo

    // 3. SEARCH (by keyword or id)
    void searchSpeakers(const string& keyword) const;
    void searchParticipants(const string& keyword) const;
    void searchSessions(const string& keyword) const;

    // 4. UPDATE (empty string / -1 = keep old value)
    void updateSpeaker(int id, const string& name, const string& topic);
    void updateParticipant(int id, const string& name, const string& phone);
    void updateSession(int id, const string& title, const string& time, int capacity);

    // 5. DELETE
    void deleteSpeaker(int id);
    void deleteParticipant(int id);
    void deleteSession(int id);

    // 6. MAIN TRANSACTION
    int registerParticipant(int participantId, int sessionId);
    void cancelRegistration(int registrationId);

    // 7. REPORT
    void report() const;
};
