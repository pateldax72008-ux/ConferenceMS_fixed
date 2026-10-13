// Utils.h - small helper functions used everywhere (input + validation)
#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <cctype>
#include <cstdlib>
using namespace std;

inline string readLine(const string& prompt) {
    cout << prompt;
    string s;
    if (!getline(cin, s)) { cout << "\n"; exit(0); }   // input closed
    size_t a = s.find_first_not_of(" \t\r");
    if (a == string::npos) return "";
    size_t b = s.find_last_not_of(" \t\r");
    return s.substr(a, b - a + 1);
}

// Keeps asking until the user types a valid whole number
inline int readInt(const string& prompt) {
    while (true) {
        string s = readLine(prompt);
        if (!s.empty() && s.size() < 9 && s.find_first_not_of("0123456789") == string::npos)
            return stoi(s);
        cout << "Please enter a valid number.\n";
    }
}

inline string toLower(string s) {
    for (char& c : s) c = (char)tolower((unsigned char)c);
    return s;
}

inline bool contains(const string& text, const string& part) {
    return text.find(part) != string::npos;
}

inline vector<string> split(const string& line, char sep) {
    vector<string> parts;
    string item;
    stringstream ss(line);
    while (getline(ss, item, sep)) parts.push_back(item);
    if (!line.empty() && line.back() == sep) parts.push_back("");
    return parts;
}

// "HH:MM" 24-hour format
inline bool isValidTime(const string& t) {
    if (t.size() != 5 || t[2] != ':') return false;
    if (!isdigit((unsigned char)t[0]) || !isdigit((unsigned char)t[1]) ||
        !isdigit((unsigned char)t[3]) || !isdigit((unsigned char)t[4])) return false;
    int h = stoi(t.substr(0, 2)), m = stoi(t.substr(3, 2));
    return h >= 0 && h <= 23 && m >= 0 && m <= 59;
}

// exactly 10 digits
inline bool isValidPhone(const string& p) {
    return p.size() == 10 && p.find_first_not_of("0123456789") == string::npos;
}
