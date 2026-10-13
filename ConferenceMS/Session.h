// Session.h - one talk/event in the conference
#pragma once
#include "Utils.h"
#include <stdexcept>

class Session {
private:
    int sessionId;
    string title;
    int speakerId;      // links to a Speaker
    string time;        // "HH:MM"
    int capacity;       // max seats
public:
    Session() : sessionId(0), title("Untitled"), speakerId(0), time("00:00"), capacity(50) {}
    Session(int id, const string& t, int spk, const string& tm, int cap)
        : sessionId(id), speakerId(spk) {
        setTitle(t); setTime(tm); setCapacity(cap);
    }
    ~Session() {}

    int getId() const { return sessionId; }
    string getTitle() const { return title; }
    int getSpeakerId() const { return speakerId; }
    string getTime() const { return time; }
    int getCapacity() const { return capacity; }

    void setTitle(const string& t) {
        if (t.empty() || t.find('|') != string::npos)
            throw invalid_argument("Title cannot be empty or contain '|'.");
        title = t;
    }
    void setTime(const string& tm) {
        if (!isValidTime(tm)) throw invalid_argument("Time must be HH:MM (24-hour), e.g. 14:30.");
        time = tm;
    }
    void setCapacity(int c) {
        if (c < 1 || c > 500) throw invalid_argument("Capacity must be between 1 and 500.");
        capacity = c;
    }

    void display() const {
        cout << "[Session #" << sessionId << "] " << title << " | Speaker ID: " << speakerId
             << " | Time: " << time << " | Capacity: " << capacity << "\n";
    }
    string toRecord() const {
        return to_string(sessionId) + "|" + title + "|" + to_string(speakerId) + "|" +
               time + "|" + to_string(capacity);
    }
};
