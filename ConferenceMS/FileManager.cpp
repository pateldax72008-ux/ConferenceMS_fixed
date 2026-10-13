#include "FileManager.h"
#include <fstream>

FileManager::FileManager(const string& folder) : dir(folder) {}

void FileManager::save(const Conference& c) const {
    ofstream sp(dir + "speakers.txt"), pa(dir + "participants.txt"),
             se(dir + "sessions.txt"), re(dir + "registrations.txt");
    if (!sp || !pa || !se || !re) throw runtime_error("Could not write to the Data folder. Make sure a folder named Data exists next to the program.");
    for (int i = 0; i < c.speakerCount; i++)      sp << c.speakers[i].toRecord() << "\n";
    for (int i = 0; i < c.participantCount; i++)  pa << c.participants[i].toRecord() << "\n";
    for (int i = 0; i < c.sessionCount; i++)      se << c.sessions[i].toRecord() << "\n";
    for (int i = 0; i < c.registrationCount; i++) re << c.registrations[i].toRecord() << "\n";
    cout << "Data saved to " << dir << "\n";
}

void FileManager::load(Conference& c) const {
    c.clearAll();
    int loaded = 0, skipped = 0;
    string line;

    ifstream sp(dir + "speakers.txt");
    while (getline(sp, line)) {
        if (line.empty()) continue;
        try {
            auto p = split(line, '|');
            if (p.size() != 3) throw invalid_argument("bad line");
            size_t used = 0;
            int id = stoi(p[0], &used);
            if (used != p[0].size() || id < 1) throw invalid_argument("bad speaker ID");
            for (int i = 0; i < c.speakerCount; ++i)
                if (c.speakers[i].getId() == id) throw invalid_argument("duplicate speaker ID");
            Speaker s(id, p[1], p[2]);
            if (c.speakerCount == c.speakerCap) Conference::grow(c.speakers, c.speakerCount, c.speakerCap);
            c.speakers[c.speakerCount++] = s;
            if (id >= c.nextSpeakerId) c.nextSpeakerId = id + 1;
            loaded++;
        } catch (const exception&) { skipped++; }
    }
    ifstream pa(dir + "participants.txt");
    while (getline(pa, line)) {
        if (line.empty()) continue;
        try {
            auto p = split(line, '|');
            if (p.size() != 3) throw invalid_argument("bad line");
            size_t used = 0;
            int id = stoi(p[0], &used);
            if (used != p[0].size() || id < 1) throw invalid_argument("bad participant ID");
            for (int i = 0; i < c.participantCount; ++i) {
                if (c.participants[i].getId() == id) throw invalid_argument("duplicate participant ID");
                if (c.participants[i].getPhone() == p[2]) throw invalid_argument("duplicate participant phone");
            }
            Participant x(id, p[1], p[2]);
            if (c.participantCount == c.participantCap) Conference::grow(c.participants, c.participantCount, c.participantCap);
            c.participants[c.participantCount++] = x;
            if (id >= c.nextParticipantId) c.nextParticipantId = id + 1;
            loaded++;
        } catch (const exception&) { skipped++; }
    }
    ifstream se(dir + "sessions.txt");
    while (getline(se, line)) {
        if (line.empty()) continue;
        try {
            auto p = split(line, '|');
            if (p.size() != 5) throw invalid_argument("bad line");
            size_t used = 0;
            int id = stoi(p[0], &used);
            if (used != p[0].size() || id < 1) throw invalid_argument("bad session ID");
            size_t speakerUsed = 0, capacityUsed = 0;
            int speakerId = stoi(p[2], &speakerUsed);
            int capacity = stoi(p[4], &capacityUsed);
            if (speakerUsed != p[2].size() || capacityUsed != p[4].size() || c.findSpeaker(speakerId) == -1)
                throw invalid_argument("invalid session reference or number");
            for (int i = 0; i < c.sessionCount; ++i)
                if (c.sessions[i].getId() == id) throw invalid_argument("duplicate session ID");
            Session s(id, p[1], speakerId, p[3], capacity);
            if (c.sessionCount == c.sessionCap) Conference::grow(c.sessions, c.sessionCount, c.sessionCap);
            c.sessions[c.sessionCount++] = s;
            if (id >= c.nextSessionId) c.nextSessionId = id + 1;
            loaded++;
        } catch (const exception&) { skipped++; }
    }
    ifstream re(dir + "registrations.txt");
    while (getline(re, line)) {
        if (line.empty()) continue;
        try {
            auto p = split(line, '|');
            if (p.size() != 4) throw invalid_argument("bad line");
            size_t used = 0, participantUsed = 0, sessionUsed = 0;
            int id = stoi(p[0], &used);
            int participantId = stoi(p[1], &participantUsed);
            int sessionId = stoi(p[2], &sessionUsed);
            if (used != p[0].size() || participantUsed != p[1].size() || sessionUsed != p[2].size() ||
                id < 1 || (p[3] != "0" && p[3] != "1") ||
                c.findParticipant(participantId) == -1 || c.findSession(sessionId) == -1)
                throw invalid_argument("invalid registration data");
            for (int i = 0; i < c.registrationCount; ++i)
                if (c.registrations[i].getId() == id) throw invalid_argument("duplicate registration ID");
            Registration r(id, participantId, sessionId, p[3] == "1");
            if (c.registrationCount == c.registrationCap) Conference::grow(c.registrations, c.registrationCount, c.registrationCap);
            c.registrations[c.registrationCount++] = r;
            if (id >= c.nextRegistrationId) c.nextRegistrationId = id + 1;
            loaded++;
        } catch (const exception&) { skipped++; }
    }
    if (loaded == 0 && skipped == 0) cout << "No saved data found - starting fresh.\n";
    else cout << "Loaded " << loaded << " record(s)" << (skipped ? ", skipped " + to_string(skipped) + " damaged line(s)" : "") << ".\n";
}
