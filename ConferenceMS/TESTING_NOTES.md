# Conference Management System — Fix and Test Notes

## Build verification

Verified with GCC using:

```bash
g++ -std=c++11 -Wall -Wextra -Wpedantic -fsanitize=address,undefined -g -o conference main.cpp Conference.cpp FileManager.cpp
```

The project compiled without compiler warnings/errors. A scripted end-to-end run successfully added a speaker, participant, and session, registered the participant, generated a report, saved the records, and confirmed the four data files each contained one record.

## Bugs fixed

1. **Duplicate participant phone on update:** changing a participant's phone to another participant's phone is now rejected, matching the add-participant rule.
2. **Partial updates:** speaker, participant, and session updates now validate a temporary copy before committing, so an invalid field does not leave earlier fields partially changed.
3. **Unsafe data loading:** loaded records now reject malformed/non-positive IDs, duplicate IDs, duplicate participant phone numbers, invalid speaker references in sessions, invalid participant/session references in registrations, and registration status values other than `0` or `1`.
4. **Numeric parsing during load:** IDs and numeric fields are checked for trailing junk rather than accepting a valid numeric prefix.

## Submission checklist status

- [x] Source code included
- [x] Builds successfully with C++11 and strict warnings enabled
- [x] End-to-end add/register/report/save test passed
- [x] UML class diagram source included (`ClassDiagram.puml`)
- [ ] Render/export the UML diagram if the instructor requires an image/PDF
- [ ] Add screenshots of the running application (the ZIP did not contain screenshots)
- [ ] Fill the institution's provided project report template (the template was not included in the ZIP)
- [ ] Create a GitHub repository, push the final source, and insert its URL in the report
- [ ] Verify final formatting in the report: Times New Roman, 12 pt, justified

Run the program from the `ConferenceMS` folder so it can access `Data/`.
