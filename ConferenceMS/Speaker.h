// Speaker.h - a person who gives a talk (inherits from Person)
#pragma once
#include "Person.h"

class Speaker : public Person {
private:
    string topic;
public:
    Speaker() : Person(), topic("N/A") {}
    Speaker(int id, const string& n, const string& t) : Person(id, n) { setTopic(t); }
    ~Speaker() {}

    string getTopic() const { return topic; }
    void setTopic(const string& t) {
        if (t.empty() || t.find('|') != string::npos)
            throw invalid_argument("Topic cannot be empty or contain '|'.");
        topic = t;
    }

    void display() const override {
        cout << "[Speaker #" << id << "] " << name << " | Topic: " << topic << "\n";
    }
    string toRecord() const override { return to_string(id) + "|" + name + "|" + topic; }
    string getRole() const override { return "Speaker"; }
};
