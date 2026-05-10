**1. Project Description**

Independent-Strong-Woman is a modular terminal-based application developed in the C programming language. The project simulates a cybersecurity mission where the player must navigate a virtual file system to track down and eliminate a mutating virus. This project serves as a practical implementation of computer engineering concepts, specifically focusing on non-linear data structures and modular system design.

**2. Core Concepts and Objectives**
-The primary goal of this project is to demonstrate the integration of several key computing pillars:

a. Data Structure Implementation: Using a Tree structure to represent a hierarchical file system.

b. Modular Programming: Separating the codebase into distinct functional modules (Logic, Data Structure, UI, and File I/O) for better maintainability.

c. Memory Management: Utilizing dynamic memory allocation and pointers to manage system nodes.

d. Persistence: Implementing a save/load system using binary data files.

**3. Technical Architecture**
The application is divided into five main modules, each with specific responsibilities:

A. Main Entry (main.c)
-Coordinates the initialization of the game and triggers the start of the core game loop.

B. Game Logic (test_logic.c / .h)
This is the "engine" of the game. It manages:

-The three-wave progression system.

-The "No Mercy" relocation algorithm (virus moves after every interaction).

-Combat calculations (HP tracking for both player and virus).

-The quiz bank for the final security wave.

C. Tree Data Structure (stack.c / .h)
-Defines the folder-based architecture.

-Each folder is a node containing an array of pointers to its sub-folders (children).

-Includes functions for directory creation, searching, and clearing flags across the tree.

D. Time Travel / File I/O (time_travel.c / .h)

-Handles the creation and reading of .dat files.

-Manages "Energy Slots," ensuring players can only save or load based on specific game rules.

E. User Interface (ui.c / .h)
-Handles all visual output in the terminal, including the health bars, logs, and mission goals.

4. Game Mechanics and Commands
Players interact with the system using standard CLI-style commands:

cd [folder_name]: Changes the current directory to the specified child folder.

cd ..: Returns the user to the Root directory.

kill [input]: Used to attack the virus.

In Wave 2, the user must provide a secret key found in the system.

In Wave 3, the user must provide the correct answer to a computing quiz.

save [1-3]: Saves the current state to the selected data slot.

load [1-3]: Restores the game state from the selected data slot.

5. Security Wave Details
Wave 1: Detection: A simple search-and-destroy mission to introduce the navigation mechanics.

Wave 2: Decryption: The virus is protected by a 4-digit code. Players must explore folders to find a SECRET_KEY.txt file containing the code.

Wave 3: Logic Duel: The virus core is exposed. Players must answer four technical questions correctly to win.

6. Installation and Compilation
To run this project, ensure you have a C compiler installed (such as GCC).

Open your terminal.

Compile the project using the following command:
gcc -o game main.c test_logic.c stack.c time_travel.c ui.c -Wall

Execute the game:
./game

7. Cleanup Script
To remove temporary game files and save data, a bash script is provided:

Run: bash clean.sh

This will remove save_*.dat files and temporary ghost files created during gameplay.

8. Authors
-Salsabila Fitria Khairizahrah - Project Lead / Time Travel and Logic Systems
-Plai
-Rola
Project Team Members - Data Structures and UI Design
