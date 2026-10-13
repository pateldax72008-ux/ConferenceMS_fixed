// Participant.h - a person who attends sessions (inherits from Person)
#pragma once
#include "Person.h"

class Participant : public Person {
private:
    string phone;
public:
    Participant() : Person(), phone("0000000000") {}
    Participant(int id, const string& n, const string& p) : Person(id, n) { setPhone(p); }
    ~Participant() {}

    string getPhone() const { return phone; }
    void setPhone(const string& p) {
        if (!isValidPhone(p)) throw invalid_argument("Phone must be exactly 10 digits.");
        phone = p;
    }

    void display() const override {
        cout << "[Participant #" << id << "] " << name << " | Phone: " << phone << "\n";
    }
    string toRecord() const override { return to_string(id) + "|" + name + "|" + phone; }
    string getRole() const override { return "Participant"; }
};
