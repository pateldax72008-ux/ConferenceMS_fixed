#include "Conference.h"
#include <iomanip>

// ---------- constructor / destructor ----------
Conference::Conference()
    : speakerCount(0), speakerCap(INITIAL_CAP),
      participantCount(0), participantCap(INITIAL_CAP),
      sessionCount(0), sessionCap(INITIAL_CAP),
      registrationCount(0), registrationCap(INITIAL_CAP),
      nextSpeakerId(1), nextParticipantId(1), nextSessionId(1), nextRegistrationId(1) {
    speakers      = new Speaker[speakerCap];
    participants  = new Participant[participantCap];
    sessions      = new Session[sessionCap];
    registrations = new Registration[registrationCap];
}

Conference::~Conference() {
    delete[] speakers;
    delete[] participants;
    delete[] sessions;
    delete[] registrations;
}

void Conference::clearAll() {
    speakerCount = participantCount = sessionCount = registrationCount = 0;
    nextSpeakerId = nextParticipantId = nextSessionId = nextRegistrationId = 1;
}

// ---------- private helpers ----------
int Conference::findSpeaker(int id) const {
    for (int i = 0; i < speakerCount; i++) if (speakers[i].getId() == id) return i;
    return -1;
}
int Conference::findParticipant(int id) const {
    for (int i = 0; i < participantCount; i++) if (participants[i].getId() == id) return i;
    return -1;
}
int Conference::findSession(int id) const {
    for (int i = 0; i < sessionCount; i++) if (sessions[i].getId() == id) return i;
    return -1;
}
int Conference::findRegistration(int id) const {
    for (int i = 0; i < registrationCount; i++) if (registrations[i].getId() == id) return i;
    return -1;
}
int Conference::countActive(int sessionId) const {
    int n = 0;
    for (int i = 0; i < registrationCount; i++)
        if (registrations[i].isActive() && registrations[i].getSessionId() == sessionId) n++;
    return n;
}
string Conference::speakerName(int id) const {
    int i = findSpeaker(id);
    return i == -1 ? "Unknown" : speakers[i].getName();
}

// ---------- 1. ADD ----------
int Conference::addSpeaker(const string& name, const string& topic) {
    Speaker s(nextSpeakerId, name, topic);          // throws if invalid, before we change anything
    if (speakerCount == speakerCap) grow(speakers, speakerCount, speakerCap);
    speakers[speakerCount++] = s;
    return nextSpeakerId++;
}

int Conference::addParticipant(const string& name, const string& phone) {
    for (int i = 0; i < participantCount; i++)
        if (participants[i].getPhone() == phone)
            throw runtime_error("A participant with this phone number already exists.");
    Participant p(nextParticipantId, name, phone);
    if (participantCount == participantCap) grow(participants, participantCount, participantCap);
    participants[participantCount++] = p;
    return nextParticipantId++;
}

int Conference::scheduleSession(const string& title, int speakerId, const string& time, int capacity) {
    if (findSpeaker(speakerId) == -1) throw runtime_error("Speaker ID not found. Add the speaker first.");
    for (int i = 0; i < sessionCount; i++)
        if (sessions[i].getSpeakerId() == speakerId && sessions[i].getTime() == time)
            throw runtime_error("This speaker already has a session at " + time + ".");
    Session s(nextSessionId, title, speakerId, time, capacity);
    if (sessionCount == sessionCap) grow(sessions, sessionCount, sessionCap);
    sessions[sessionCount++] = s;
    return nextSessionId++;
}

// ---------- 2. DISPLAY ----------
void Conference::displaySpeakers() const {
    if (speakerCount == 0) { cout << "No speakers yet.\n"; return; }
    for (int i = 0; i < speakerCount; i++) speakers[i].display();
}
void Conference::displayParticipants() const {
    if (participantCount == 0) { cout << "No participants yet.\n"; return; }
    for (int i = 0; i < participantCount; i++) participants[i].display();
}
void Conference::displaySessions() const {
    if (sessionCount == 0) { cout << "No sessions yet.\n"; return; }
    for (int i = 0; i < sessionCount; i++) {
        const Session& s = sessions[i];
        cout << "[Session #" << s.getId() << "] " << s.getTitle()
             << " | Speaker: " << speakerName(s.getSpeakerId())
             << " | Time: " << s.getTime()
             << " | Seats: " << countActive(s.getId()) << "/" << s.getCapacity() << "\n";
    }
}
void Conference::displayRegistrations() const {
    if (registrationCount == 0) { cout << "No registrations yet.\n"; return; }
    for (int i = 0; i < registrationCount; i++) {
        const Registration& r = registrations[i];
        int p = findParticipant(r.getParticipantId());
        int s = findSession(r.getSessionId());
        cout << "[Registration #" << r.getId() << "] "
             << (p == -1 ? "?" : participants[p].getName()) << " -> "
             << (s == -1 ? "?" : sessions[s].getTitle())
             << " | " << (r.isActive() ? "ACTIVE" : "CANCELLED") << "\n";
    }
}

// RUNTIME POLYMORPHISM: one Person* pointer type, but p->display() runs the
// Speaker version or the Participant version depending on the real object.
void Conference::displayAllPeople() const {
    if (speakerCount + participantCount == 0) { cout << "No people yet.\n"; return; }
    for (int i = 0; i < speakerCount; i++) {
        Person* p = &speakers[i];
        p->display();
    }
    for (int i = 0; i < participantCount; i++) {
        Person* p = &participants[i];
        p->display();
    }
}

// ---------- 3. SEARCH ----------
void Conference::searchSpeakers(const string& keyword) const {
    string k = toLower(keyword);
    bool found = false;
    for (int i = 0; i < speakerCount; i++)
        if (contains(toLower(speakers[i].getName()), k) || contains(toLower(speakers[i].getTopic()), k) ||
            to_string(speakers[i].getId()) == keyword) { speakers[i].display(); found = true; }
    if (!found) cout << "No matching speakers.\n";
}
void Conference::searchParticipants(const string& keyword) const {
    string k = toLower(keyword);
    bool found = false;
    for (int i = 0; i < participantCount; i++)
        if (contains(toLower(participants[i].getName()), k) || contains(participants[i].getPhone(), keyword) ||
            to_string(participants[i].getId()) == keyword) { participants[i].display(); found = true; }
    if (!found) cout << "No matching participants.\n";
}
void Conference::searchSessions(const string& keyword) const {
    string k = toLower(keyword);
    bool found = false;
    for (int i = 0; i < sessionCount; i++)
        if (contains(toLower(sessions[i].getTitle()), k) || sessions[i].getTime() == keyword ||
            to_string(sessions[i].getId()) == keyword) { sessions[i].display(); found = true; }
    if (!found) cout << "No matching sessions.\n";
}

// ---------- 4. UPDATE ----------
void Conference::updateSpeaker(int id, const string& name, const string& topic) {
    int i = findSpeaker(id);
    if (i == -1) throw runtime_error("Speaker ID not found.");
    Speaker updated = speakers[i];
    if (!name.empty()) updated.setName(name);
    if (!topic.empty()) updated.setTopic(topic);
    speakers[i] = updated; // Commit only after every supplied field validates.
}
void Conference::updateParticipant(int id, const string& name, const string& phone) {
    int i = findParticipant(id);
    if (i == -1) throw runtime_error("Participant ID not found.");
    if (!phone.empty()) {
        for (int j = 0; j < participantCount; j++)
            if (j != i && participants[j].getPhone() == phone)
                throw runtime_error("A participant with this phone number already exists.");
    }
    Participant updated = participants[i];
    if (!name.empty()) updated.setName(name);
    if (!phone.empty()) updated.setPhone(phone);
    participants[i] = updated; // Avoid partially applying an invalid update.
}
void Conference::updateSession(int id, const string& title, const string& time, int capacity) {
    int i = findSession(id);
    if (i == -1) throw runtime_error("Session ID not found.");
    if (!time.empty()) {
        if (!isValidTime(time)) throw invalid_argument("Time must be HH:MM (24-hour), e.g. 14:30.");
        for (int j = 0; j < sessionCount; j++)
            if (j != i && sessions[j].getSpeakerId() == sessions[i].getSpeakerId() && sessions[j].getTime() == time)
                throw runtime_error("That speaker already has a session at " + time + ".");
    }
    if (capacity != -1 && capacity < countActive(id))
        throw runtime_error("Capacity cannot be lower than current registrations.");
    Session updated = sessions[i];
    if (!title.empty()) updated.setTitle(title);
    if (!time.empty()) updated.setTime(time);
    if (capacity != -1) updated.setCapacity(capacity);
    sessions[i] = updated; // Commit only after all validations succeed.
}

// ---------- 5. DELETE ----------
void Conference::deleteSpeaker(int id) {
    int i = findSpeaker(id);
    if (i == -1) throw runtime_error("Speaker ID not found.");
    for (int j = 0; j < sessionCount; j++)
        if (sessions[j].getSpeakerId() == id)
            throw runtime_error("Cannot delete: this speaker still has sessions. Delete those first.");
    removeAt(speakers, speakerCount, i);
}
void Conference::deleteParticipant(int id) {
    int i = findParticipant(id);
    if (i == -1) throw runtime_error("Participant ID not found.");
    for (int j = 0; j < registrationCount; j++)
        if (registrations[j].isActive() && registrations[j].getParticipantId() == id)
            throw runtime_error("Cannot delete: participant has active registrations. Cancel them first.");
    removeAt(participants, participantCount, i);
}
void Conference::deleteSession(int id) {
    int i = findSession(id);
    if (i == -1) throw runtime_error("Session ID not found.");
    if (countActive(id) > 0)
        throw runtime_error("Cannot delete: session has active registrations. Cancel them first.");
    removeAt(sessions, sessionCount, i);
}

// ---------- 6. MAIN TRANSACTION: registration ----------
int Conference::registerParticipant(int participantId, int sessionId) {
    int p = findParticipant(participantId);
    if (p == -1) throw runtime_error("Participant ID not found.");
    int s = findSession(sessionId);
    if (s == -1) throw runtime_error("Session ID not found.");

    for (int i = 0; i < registrationCount; i++) {
        if (!registrations[i].isActive() || registrations[i].getParticipantId() != participantId) continue;
        if (registrations[i].getSessionId() == sessionId)
            throw runtime_error("Participant is already registered for this session.");
        int other = findSession(registrations[i].getSessionId());
        if (other != -1 && sessions[other].getTime() == sessions[s].getTime())
            throw runtime_error("Time clash: participant already has a session at " + sessions[s].getTime() + ".");
    }
    if (countActive(sessionId) >= sessions[s].getCapacity())
        throw runtime_error("Session is full.");

    Registration r;
    r.registerParticipant(nextRegistrationId, participantId, sessionId);
    if (registrationCount == registrationCap) grow(registrations, registrationCount, registrationCap);
    registrations[registrationCount++] = r;
    return nextRegistrationId++;
}

void Conference::cancelRegistration(int registrationId) {
    int i = findRegistration(registrationId);
    if (i == -1) throw runtime_error("Registration ID not found.");
    registrations[i].cancel();
}

// ---------- 7. REPORT ----------
void Conference::report() const {
    int active = 0;
    for (int i = 0; i < registrationCount; i++) if (registrations[i].isActive()) active++;

    cout << "\n=========== CONFERENCE REPORT ===========\n";
    cout << "Speakers: " << speakerCount << " | Participants: " << participantCount
         << " | Sessions: " << sessionCount << " | Active registrations: " << active << "\n";

    int best = -1, bestCount = 0;
    for (int i = 0; i < sessionCount; i++) {
        const Session& s = sessions[i];
        int taken = countActive(s.getId());
        cout << "\nSession #" << s.getId() << ": " << s.getTitle() << " (" << s.getTime() << ")"
             << " - " << speakerName(s.getSpeakerId()) << "\n";
        cout << "  Seats filled: " << taken << "/" << s.getCapacity()
             << " (" << (taken * 100 / s.getCapacity()) << "%)\n";
        for (int j = 0; j < registrationCount; j++)
            if (registrations[j].isActive() && registrations[j].getSessionId() == s.getId()) {
                int p = findParticipant(registrations[j].getParticipantId());
                cout << "    - " << (p == -1 ? "?" : participants[p].getName()) << "\n";
            }
        if (taken > bestCount) { bestCount = taken; best = i; }
    }
    if (best != -1) cout << "\nMost popular session: " << sessions[best].getTitle()
                         << " (" << bestCount << " registered)\n";

    cout << "Participants with no registration: ";
    bool any = false;
    for (int i = 0; i < participantCount; i++) {
        bool reg = false;
        for (int j = 0; j < registrationCount; j++)
            if (registrations[j].isActive() && registrations[j].getParticipantId() == participants[i].getId()) reg = true;
        if (!reg) { cout << (any ? ", " : "") << participants[i].getName(); any = true; }
    }
    cout << (any ? "" : "None") << "\n=========================================\n";
}
