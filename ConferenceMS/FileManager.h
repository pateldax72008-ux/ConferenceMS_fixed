// FileManager.h - saves/loads everything to text files in the Data/ folder
#pragma once
#include "Conference.h"

class FileManager {
private:
    string dir;     // e.g. "Data/"
public:
    FileManager(const string& folder = "Data/");
    void save(const Conference& c) const;
    void load(Conference& c) const;
};
