#include <stdio.h>
#include <stdlib.h>
#include "ui.h"
#include "stack.h"    /* getStackSize(), getAction() */

/*
 * ui.c -- Terminal UI for OS.Kill()
 *
 * Responsible ONLY for rendering the terminal display.
 * No game logic, no data manipulation here.
 *
 * Data Structure used: Stack (reads history entries to display in HUD).
 */

#ifdef _WIN32
    #include <windows.h>
    #define CLEAR_SCREEN()  system("cls")
#else
    #include <unistd.h>
    #define CLEAR_SCREEN()  system("clear")
#endif

/* Max history entries shown in the HUD at once */
#define HISTORY_MAX 5

/* ------------------------------------------------------------------ */
/*  drawManual                                                          */
/* ------------------------------------------------------------------ */

void drawManual(int wave) {
    printf("+---------------------------------------------------------+\n");
    printf("|                    MISSION MANUAL                      |\n");
    printf("+---------------------------------------------------------+\n");
    printf("| NAVIGATION                                              |\n");
    printf("|   cd [folder]   -- move into a folder                  |\n");
    printf("|   cd ..         -- return to C:/Root                   |\n");
    printf("| SYSTEM COMMANDS                                         |\n");
    printf("|   save          -- create a checkpoint  (max 3 times)  |\n");
    printf("|   travel [n]    -- restore checkpoint n                |\n");
    printf("|   undo          -- remove last entry from action stack  |\n");
    printf("+---------------------------------------------------------+\n");

    if (wave == 1) {
        printf("| WAVE 1 OBJECTIVE                                        |\n");
        printf("|   Find GHOST_FILE.txt hidden in one of the folders.    |\n");
        printf("|   Navigate to that folder, then type:                  |\n");
        printf("|     >> kill                                             |\n");
    } else if (wave == 2) {
        printf("| WAVE 2 OBJECTIVE                                        |\n");
        printf("|   Find GHOST_FILE.txt AND read SECRET_KEY.txt.         |\n");
        printf("|   Navigate to the GHOST_FILE folder, then type:        |\n");
        printf("|     >> kill [4-digit code from SECRET_KEY]              |\n");
    } else if (wave == 3) {
        printf("| WAVE 3 OBJECTIVE  (FINAL WAVE)                         |\n");
        printf("|   Open GHOST_FILE.txt -- answer the Data Structure quiz.|\n");
        printf("|   Check ANSWER_A.txt / ANSWER_B.txt for hints.         |\n");
        printf("|   Navigate to the GHOST_FILE folder, then type:        |\n");
        printf("|     >> kill A     or     >> kill B                      |\n");
    }

    printf("+---------------------------------------------------------+\n");
}

/* ------------------------------------------------------------------ */
/*  drawUI                                                              */
/* ------------------------------------------------------------------ */

void drawUI(char *loc, int wave, int vHP, int pHP, char *msg) {
    CLEAR_SCREEN();

#ifdef _WIN32
    /* Red while virus is alive; green when defeated */
    system(vHP > 0 ? "color 0C" : "color 0A");
#endif

    /* Header */
    printf("+=========================================================+\n");
    printf("|          OS.Kill()  --  VIRUS ELIMINATION SYSTEM       |\n");
    printf("+=========================================================+\n");

    /* Wave and location */
    printf("| WAVE : %-3d                                              |\n", wave);
    printf("| NODE : %-49s|\n", loc);

    /* Player HP bar  (#  = health remaining) */
    int p_fill = pHP / 10;
    printf("| PLAYER HP : [%3d] [", pHP);
    for (int i = 0; i < 10; i++) printf(i < p_fill ? "#" : ".");
    printf("]                  |\n");

    /* Virus HP bar  (! = threat remaining) */
    int v_fill = vHP / 10;
    printf("| VIRUS  HP : [%3d] [", vHP);
    for (int i = 0; i < 10; i++) printf(i < v_fill ? "!" : ".");
    printf("]                  |\n");

    printf("+---------------------------------------------------------+\n");

    /* Action history from Stack */
    printf("| HISTORY (Stack -- last %d actions)                      |\n",
           HISTORY_MAX);
    printf("|  ");
    int size  = getStackSize();
    int start = (size > HISTORY_MAX) ? size - HISTORY_MAX : 0;
    int shown = 0;
    for (int i = start; i < size; i++) {
        printf("[%s] ", getAction(i));
        shown++;
    }
    if (shown == 0) printf("(empty)");
    printf("\n");
    printf("+---------------------------------------------------------+\n\n");

    /* Inline mission manual */
    drawManual(wave);

    /* Log message */
    printf("\n  LOG >> %s\n", msg);
    printf("---------------------------------------------------------\n");
}

/* ------------------------------------------------------------------ */
/*  drawGameOver                                                        */
/* ------------------------------------------------------------------ */

void drawGameOver(void) {
#ifdef _WIN32
    system("color 4F");
#endif
    printf("\n");
    printf("+=========================================================+\n");
    printf("|                                                         |\n");
    printf("|          !! CRITICAL FAILURE !!  OS DESTROYED          |\n");
    printf("|          Player HP reached 0.  The virus won.          |\n");
    printf("|                                                         |\n");
    printf("+=========================================================+\n");
}

/* ------------------------------------------------------------------ */
/*  drawVictory                                                         */
/* ------------------------------------------------------------------ */

void drawVictory(void) {
#ifdef _WIN32
    system("color 0A");
#endif
    printf("\n");
    printf("+=========================================================+\n");
    printf("|                                                         |\n");
    printf("|          [ SUCCESS ]  VIRUS PURGED.                    |\n");
    printf("|          System stability restored.  OS is safe.       |\n");
    printf("|                                                         |\n");
    printf("+=========================================================+\n");
    printf("\n  Press Enter to exit...\n");
    getchar();
    getchar();
}
