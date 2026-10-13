// Registration.h - links ONE participant to ONE session
#pragma once
#include "Utils.h"
#include <stdexcept>

class Registration {
private:
    int registrationId;
    int participantId;
    int sessionId;
    bool active;        // false = cancelled
public:
    Registration() : registrationId(0), participantId(0), sessionId(0), active(false) {}
    Registration(int rid, int pid, int sid, bool act)
        : registrationId(rid), participantId(pid), sessionId(sid), active(act) {}
    ~Registration() {}

    int getId() const { return registrationId; }
    int getParticipantId() const { return participantId; }
    int getSessionId() const { return sessionId; }
    bool isActive() const { return active; }

    void registerParticipant(int rid, int pid, int sid) {
        registrationId = rid; participantId = pid; sessionId = sid; active = true;
    }
    void cancel() {
        if (!active) throw logic_error("This registration is already cancelled.");
        active = false;
    }

    void display() const {
        cout << "[Registration #" << registrationId << "] Participant " << participantId
             << " -> Session " << sessionId << " | " << (active ? "ACTIVE" : "CANCELLED") << "\n";
    }
    string toRecord() const {
        return to_string(registrationId) + "|" + to_string(participantId) + "|" +
               to_string(sessionId) + "|" + (active ? "1" : "0");
    }
};
