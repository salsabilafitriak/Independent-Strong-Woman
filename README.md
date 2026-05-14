# OS.Kill() — System Defense Game

> A terminal-based hacking defense game built in C, developed as a final project for **CPE112: Programming with Data Structures** (2/2025 semester).

---

## Team Members

| Name | GitHub Username |
|------|----------------|
| Member 1 | `@salsabilafitriak` |
| Member 2 | `@plai-cloud` |
| Member 3 | `@thhs16` |

> **Commit Hash:** `42c75e2`

---

## Project Overview

**OS.Kill()** is a CLI-based game where the player acts as a system administrator defending an OS against a virus spreading through the file system. The player must navigate a 2-level virtual directory tree, locate the virus hidden inside a subfolder, and eliminate it across 3 increasingly difficult waves.

### Game Mechanics

- **Wave 1 — Detection:** Navigate into subfolders to find the virus. Type `kill` when inside the infected folder. Requires 2 hits.
- **Wave 2 — Encryption:** The virus is encrypted. Find the `SECRET_KEY.txt` hidden in another folder, then use `kill [code]` to attack. Requires 3 hits.
- **Wave 3 — Logic Lock:** The virus is protected by a quiz. Answer data structure questions correctly with `kill [A/B]` to deal damage. Wrong answers drain player HP. Requires 4 correct answers.

After every `kill` attempt (hit or miss), the virus randomly relocates to a different folder — always a different one from before.

The player wins by depleting the virus's HP across all 3 waves. The game ends early if the player's HP reaches 0.

---

## Data Structures Used

### 1. Array — Fixed-Size Collections

Arrays are used in multiple places throughout the project:
- `children[MAX_CHILDREN]` inside each `Folder` struct stores child folder pointers.
- `questions[]` and `answers[]` in `test_logic.c` store the Wave 3 quiz content.
- `save_used[]` and `load_used[]` in `time_travel.c` track which save/load slots have been used.
- `BFSQueue.data[]` in `bfs.c` — a fixed-size circular array used as the queue for BFS traversal.

**Why an Array?**
- All collection sizes are fixed and known at compile time — arrays are the most direct and efficient structure.
- O(1) random access by index for quiz questions and save slots.
- The BFS queue uses a circular array for O(1) enqueue and dequeue without dynamic allocation.

**Alternative considered:** Linked List — adds unnecessary pointer overhead and heap allocation when the collection size never changes.

### 2. Tree (N-ary Tree) — File System Representation

The virtual OS file system is a 2-level N-ary tree:

```
Root
├── System/
│   ├── Kernel/
│   └── Services/
├── Users/
│   ├── Admin/
│   └── Guest/
├── Database/
│   ├── Records/
│   └── Cache/
└── Network/
    ├── Firewall/
    └── Proxy/
```

Virus is always planted at level 2 (leaf nodes). Player must `cd` into a subroot first, then `cd` into a child to find and kill it.

**Why a Tree?**
- Naturally models hierarchical folder structures — exactly how real file systems work.
- `cd [folder]` maps directly to parent-child traversal.
- Supports BFS traversal for virus spreading across levels.

**Alternative considered:** Linked List — loses parent-child hierarchy, making navigation unnatural and BFS spread impossible.

### 3. Stack — Navigation History

A singly-linked stack (`StackNode`) stores the player's folder traversal history, enabling `cd ..` (go back) and automatic repositioning after a `kill` command.

**Why a Stack?**
- Navigation is inherently LIFO: the last folder entered is the first to exit when going back.
- `push_history()` on `cd`, `pop_history()` on `cd ..` or after `kill`.
- O(1) push and pop — constant time regardless of depth.

**Alternative considered:** Array-based history — fixed size limit and requires index tracking; dynamic linked stack is more flexible.

---

## Algorithms Implemented

- **BFS (Breadth-First Search)** — `bfs.c` implements BFS using an array-based circular queue. Used for virus spreading across the tree each wave, and to locate the infected node from root. Wave 1 spreads to 1 neighbor, Wave 2 to 2, Wave 3 to all.
- **Tree Traversal** — `find_child()` linearly scans a node's children to locate a target folder by name, simulating real OS directory lookup.
- **Randomized Virus Placement** — `plantVirus()` uses `rand()` with a do-while constraint ensuring the virus always relocates to a different folder than before.
- **Save/Load State** — `time_travel.c` implements slot-based game state persistence using file I/O, allowing players to save and restore a previous timeline.

---

## Project Structure

```
OS.Kill/
├── main.c           # Entry point
├── test_logic.c/h   # Core game loop and wave logic
├── fileSystem.c/h   # N-ary Tree (folder) data structure
├── history.c/h      # Stack (navigation history)
├── Bfs.c/h          # BFS algorithm with array-based queue
├── time_travel.c/h  # Save/load system
├── ui.c/h           # Terminal UI rendering
├── .gitignore       # Excludes generated folders and save files
└── README.md
```

---

## How to Build & Run

### Requirements
- GCC (Linux/macOS) or MinGW (Windows)
- A terminal that supports ANSI color codes

### Compile

```bash
gcc main.c test_logic.c fileSystem.c history.c Bfs.c time_travel.c ui.c -o oskill
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
| `kill` | Attack the virus — Wave 1 (must be inside infected folder) |
| `kill [code]` | Decrypt and attack with secret key — Wave 2 |
| `kill [A/B]` | Answer quiz question to attack — Wave 3 |
| `save [1-3]` | Save current game state to a slot |
| `load [1-3]` | Load a previously saved game state |

---

## 🧪 Sample Input / Output

```
======================================================================
  OS.Kill()  |  WAVE 1 / 3  |  SYSTEM DEFENSE PROTOCOL
======================================================================

  PLAYER  [##########]  100 HP
  VIRUS   [!!!!!!!!!!]  100 HP

----------------------------------------------------------------------
  LOCATION  >>  C:/Root/
----------------------------------------------------------------------

  [ OBJECTIVE ]
  Navigate the file system to find the virus.
  Once inside the infected folder, type 'kill' to attack.

  [ COMMANDS ]
  cd [folder]   -- enter a folder
  cd ..         -- go back
  kill          -- attack virus (must be inside infected folder)
  save [1-3]    -- save current state
  load [1-3]    -- load saved state

----------------------------------------------------------------------
  LOG >> System Online. Welcome back, Genius!
======================================================================

 Admin@OS.Kill >> cd System
 LOG >> Folder accessed. System clean.

 Admin@OS.Kill >> cd Kernel
 LOG >> W1 Virus Detected. Execute Kill Protocol.

 Admin@OS.Kill >> kill
 LOG >> CRITICAL HIT! Virus weakened!
```

---

## Known Limitations & Future Improvements

- Tree depth is fixed at 2 levels. A deeper recursive tree would better simulate a real file system.
- Save slots can only be used once per session (by design, as a game balance mechanic).
- BFS spread currently affects infection levels but could be extended to multi-node simultaneous infection for higher difficulty.
- A timed mode could be added to increase pressure on the player.
