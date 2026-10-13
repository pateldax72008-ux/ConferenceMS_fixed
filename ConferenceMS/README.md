# Conference Management System (C++ console project)

## Build
    g++ -std=c++11 -o conference main.cpp Conference.cpp FileManager.cpp
(Needs g++ 8 or newer. On Windows use MinGW g++ or Visual Studio / Code::Blocks with C++17.)

## Run
    ./conference        (Windows: conference.exe)
Run it from this folder so it finds/creates the `Data/` folder.

## How to use
Pick a number 1-9 from the menu. Add a Speaker first, then a Session
(needs the Speaker ID), then Participants, then use option 6 to register.
Data is loaded automatically at start and saved on Exit (option 9).

## Files
- main.cpp           menu only
- Conference.h/.cpp  system class (dynamic arrays + all logic)
- FileManager.h/.cpp save()/load() to Data/*.txt
- Person.h           abstract base class; Speaker.h, Participant.h inherit from it
- Session.h, Registration.h, Utils.h
- Data/              speakers.txt, participants.txt, sessions.txt, registrations.txt


## UML and verification
- `ClassDiagram.puml` contains the UML class diagram source (render it with PlantUML if an image/PDF is required).
- `TESTING_NOTES.md` records the fixes and build/runtime checks.
