// Person.h - ABSTRACT BASE CLASS (Inheritance + Runtime Polymorphism)
// Speaker and Participant both "are a" Person.
#pragma once
#include "Utils.h"
#include <stdexcept>

class Person {
protected:                      // derived classes can use these, outsiders cannot
    int id;
    string name;
public:
    Person() : id(0), name("Unknown") {}                       // default constructor
    Person(int i, const string& n) : id(i) { setName(n); }     // parameterized constructor
    virtual ~Person() {}                                       // virtual destructor

    int getId() const { return id; }
    string getName() const { return name; }
    void setName(const string& n) {                            // validation lives here
        if (n.empty() || n.find('|') != string::npos)
            throw invalid_argument("Name cannot be empty or contain '|'.");
        name = n;
    }

    // "= 0" makes these PURE VIRTUAL: each child must supply its own version.
    virtual void display() const = 0;
    virtual string toRecord() const = 0;
    virtual string getRole() const = 0;
};
