# OS.Kill() — System Defense Game

> A terminal-based hacking defense game built in C, developed as a final project for **CPE112: Programming with Data Structures** (2/2025 semester).

---

## Team Members

| Name | GitHub Username |
|------|----------------|
| Member 1 | `@salsabilafitriak` |
| Member 2 | `@plai-cloud` |
| Member 3 | `@username3` |

> **Commit Hash:** `5e914ce`

---

## Project Overview

**OS.Kill()** is a CLI-based game where the player acts as a system administrator defending an OS against a virus spreading through the file system. The player must navigate a virtual directory tree, locate the virus, and eliminate it across 3 increasingly difficult waves.

### Game Mechanics

- **Wave 1 — Detection:** Find and kill the virus by navigating folders. Requires 2 hits.
- **Wave 2 — Encryption:** The virus is encrypted. Find a secret key hidden in another folder, then use it to decrypt and kill. Requires 3 hits.
- **Wave 3 — Logic Lock:** The virus is protected by a quiz. Answer data structure questions correctly to deal damage. Requires 4 correct answers.

The player wins by depleting the virus's HP across all 3 waves. The game ends early if the player's HP reaches 0.

---

## Data Structures Used

### 1. Array — Fixed-Size Collections

Arrays are used in multiple places throughout the project:
- `children[10]` inside each `Folder` struct stores up to 10 child folder pointers.
- `questions[]` and `answers[]` in `test_logic.c` store the Wave 3 quiz content.
- `save_used[4]` and `load_used[4]` in `time_travel.c` track which save/load slots have been used.

**Why an Array?**
- The number of folders, quiz questions, and save slots are all fixed and known at compile time — arrays are the most direct and efficient structure for fixed-size collections.
- O(1) random access by index makes reading a question or checking a slot instant.
- Zero overhead — no pointers or heap allocations needed for static data.

**Alternative considered:** Linked List — adds unnecessary pointer overhead and dynamic allocation when the collection size never changes.

### 2. Tree (N-ary Tree) — File System Representation

The virtual OS file system is represented as a tree where `Root` is the parent node and directories (`System`, `Users`, `Temp`, etc.) are its children, each stored in the `children[]` array of their parent `Folder` struct.

**Why a Tree?**
- Naturally models hierarchical folder structures — exactly how real file systems work.
- `cd [folder]` maps directly to parent-child traversal.
- Searching for the virus folder is an O(n) scan of children, efficient for small n.

**Alternative considered:** Linked List — would lose the parent-child hierarchy, making navigation unnatural and inefficient.

### 3. Stack — Navigation History

A singly-linked stack (`StackNode`) stores the player's folder traversal history, enabling `cd ..` (go back) and automatic repositioning after a `kill` command.

**Why a Stack?**
- Navigation is inherently LIFO: the last folder entered is the first to exit when going back.
- `push_history()` on `cd`, `pop_history()` on `cd ..` or after `kill`.
- O(1) push and pop — constant time regardless of depth.

**Alternative considered:** Array-based history — fixed size limit and requires index tracking; stack with dynamic nodes is more flexible.

---

## Algorithms Implemented

- **Tree Traversal** — `find_child()` linearly scans a node's children to locate a target folder by name, simulating directory lookup.
- **Randomized Virus Placement** — `plantVirus()` uses `rand()` with a constraint to ensure the virus never re-spawns in the same folder as the previous wave, preventing repetition.
- **Save/Load State** — `time_travel.c` implements a slot-based game state persistence system using file I/O, allowing players to save progress and restore a previous timeline.

---

## 📁 Project Structure

```
OS.Kill/
├── main.c           # Entry point
├── test_logic.c/h   # Core game loop and wave logic
├── fileSystem.c/h   # Tree (folder) data structure
├── history.c/h      # Stack (navigation history)
├── time_travel.c/h  # Save/load system
├── ui.c/h           # Terminal UI rendering
├── .gitignore       # Excludes generated folders (System/, Users/, etc.)
└── README.md
```

---

## How to Build & Run

### Requirements
- GCC (Linux/macOS) or MinGW (Windows)
- A terminal that supports ANSI color codes

### Compile

```bash
gcc main.c test_logic.c fileSystem.c history.c time_travel.c ui.c -o oskill
```

### Run

```bash
./oskill
```

On Windows:
```bash
oskill.exe
```

---

## Commands

| Command | Description |
|---------|-------------|
| `cd [folder]` | Navigate into a subfolder |
| `cd ..` | Go back to the previous folder |
| `kill` | Attack the virus (Wave 1) |
| `kill [code]` | Decrypt and attack with secret key (Wave 2) |
| `kill [A/B]` | Answer quiz question to attack (Wave 3) |
| `save [1-3]` | Save current game state to a slot |
| `load [1-3]` | Load a previously saved game state |

---

## Sample Input / Output

```
 Admin@OS.Kill >> cd System
 [SYSTEM LOG] >> Folder accessed. System clean.

 Admin@OS.Kill >> cd Temp
 [SYSTEM LOG] >> W1 Virus Detected. Execute Kill Protocol.

 Admin@OS.Kill >> kill
 [SYSTEM LOG] >> CRITICAL HIT! But the virus escaped!
```

---

## Known Limitations & Future Improvements

- The folder tree is currently fixed at 1 level of depth (Root → 8 children). A recursive tree with deeper nesting would better simulate a real file system.
- Save slots can only be used once per session (by design, as a game balance mechanic).
- Adding BFS/DFS virus spread across multiple nodes could increase Wave 3 complexity.
- A graphical ASCII map of the file system could improve navigation clarity.


e used once per session (by design, as a game balance mechanic).
- Adding BFS/DFS virus spread across multiple nodes could increase Wave 3 complexity.
- A graphical ASCII map of the file system could improve navigation clarity.
