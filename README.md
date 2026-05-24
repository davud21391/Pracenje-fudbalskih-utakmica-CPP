# Football Results Tracker

Console-based C++ application for managing football teams, players, matches, scorers, standings, search, sorting, and file persistence.

## Requirements

- macOS, Linux, or another environment with `make` and `clang++`

## Build and Run

From the project root:

```bash
make run
```

Useful commands:

```bash
make build
make clean
```

## Visual Studio 2022

The repository now also includes:

- `FootballResultsTracker.sln`
- `FootballResultsTracker.vcxproj`

So the project can be opened directly in Visual Studio 2022 for seminar delivery.

## Saved Data

The application saves data into the `data/` folder using four text files:

- `data/timovi.txt`
- `data/igraci.txt`
- `data/utakmice.txt`
- `data/strijelci.txt`

To save current data inside the app, use option `10`.

To load saved data after restarting the app, use option `11`.

## Available Menu Options

1. Add team
2. Add player to team
3. Match management (add, edit, sort)
4. Delete match
5. Record scorers
6. Search matches
7. Show standings table
8. Show matches for a selected team
9. Show top scorers list
10. Save data to files
11. Load data from files
12. Show all teams
13. Show players for a selected team
14. Show all matches
0. Exit

## What the App Currently Supports

- Team creation and listing
- Player creation and listing by team
- Match creation, editing, sorting, and deletion
- Scorer entry with minute and own-goal flag
- Standings table with points and goal difference
- Match search by team, date, or round
- Team-specific match history
- Save/load from text files
- Multithreaded file save operation

## Notes

- Dates are expected in `dd.mm.yyyy` format. The app also accepts an extra trailing dot, for example `12.05.2026.`.
- The standings table is recalculated from saved matches each time, so deleting or loading matches automatically updates table results.
- IDs are generated automatically and restored correctly after loading saved data.
- Saving data uses multiple `std::thread` workers to write separate files in parallel.
