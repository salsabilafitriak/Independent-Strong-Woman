#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ui.h"

#define RESET   "\033[0m"
#define RED     "\033[1;31m"
#define YELLOW  "\033[1;33m"
#define CYAN    "\033[1;36m"
#define WHITE   "\033[1;37m"
#define DIM     "\033[2m"

void drawUI(char *loc, int wave, int vHP, int pHP, char *msg,
            Folder *root, Folder *current, int step, int depth, char *action) {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif

    printf(CYAN "======================================================================\n");
    printf("  OS.Kill()  |  WAVE %d / 3  |  SYSTEM DEFENSE PROTOCOL\n", wave);
    printf("======================================================================\n" RESET);

    int p_fill = (pHP > 0) ? pHP / 10 : 0;
    int v_fill = (vHP > 0) ? vHP / 20 : 0;
    printf("\n");
    printf("  PLAYER  [%-10.*s] %3d HP\n", p_fill, "##########", pHP);
    printf("  VIRUS   " RED "[%-10.*s]" RESET " %3d HP\n", v_fill, "!!!!!!!!!!", vHP);
    printf("\n");

    printf(CYAN "----------------------------------------------------------------------\n" RESET);
    printf("  LOCATION  >>  " WHITE "%s\n" RESET, loc);
    
    // MENAMPILKAN ACTION STEP DI SINI (Warna Kuning agar terlihat beda)
    printf("  " YELLOW "%s\n" RESET, action); 
    
    printf(CYAN "----------------------------------------------------------------------\n" RESET);

    printf("\n  [ OBJECTIVE ]\n");
    if (wave == 1) {
        printf("  Navigate the file system to find the virus.\n");
        printf("  Once inside the infected folder, type 'kill' to attack.\n");
        printf("  " DIM "(Virus HP: 100 | Requires 2 hits)\n" RESET);
    } else if (wave == 2) {
        printf("  The virus is encrypted. Find the SECRET_KEY first,\n");
        printf("  then use 'kill [code]' to decrypt and attack.\n");
        printf("  " DIM "(Virus HP: 150 | Requires 3 hits)\n" RESET);
    } else {
        printf("  The virus has evolved. Answer the quiz correctly to damage it.\n");
        printf("  Use 'kill [A/B]' to respond. Wrong answers drain YOUR HP.\n");
        printf("  " DIM "(Virus HP: 200 | Requires 4 correct answers)\n" RESET);
    }

    printf("\n  [ COMMANDS ]\n");
    printf("  cd [folder]   -- enter a folder\n");
    printf("  cd ..         -- go back\n");
    if (wave == 1)
        printf("  kill          -- attack virus (must be inside infected folder)\n");
    else if (wave == 2)
        printf("  kill [code]   -- attack with decryption key\n");
    else
        printf("  kill [A/B]    -- answer quiz to attack\n");
    printf("  save [1-3]    -- save current state\n");
    printf("  load [1-3]    -- load saved state\n");

    printf("\n" CYAN "----------------------------------------------------------------------\n" RESET);
    printf("  LOG >> %s\n", msg);
    printf(CYAN "======================================================================\n\n" RESET);
}

void drawWaveTransition(int wave, int type) {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
    printf("\n\n");
    printf(CYAN "======================================================================\n" RESET);
    if (type == 0) {
        printf("  WAVE %d INITIATED\n", wave);
        if (wave == 1) printf("  The virus has been detected in the file system.\n");
        if (wave == 2) printf("  The virus has mutated -- it is now encrypted.\n");
        if (wave == 3) printf("  FINAL WAVE -- The virus core has awakened.\n");
    } else {
        printf("  WAVE %d CLEARED -- Virus weakened.\n", wave - 1);
        printf("  Preparing next sequence...\n");
    }
    printf(CYAN "======================================================================\n" RESET);
    printf("\n  Press Enter to continue...");
    getchar();
}

void drawGameOver(void) {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
    printf("\n\n");
    printf(RED "======================================================================\n");
    printf("  SYSTEM FAILURE\n");
    printf("  The virus has taken full control of the OS.\n");
    printf("  All processes terminated.\n");
    printf("======================================================================\n" RESET);
    printf("\n");
}

void drawVictory(void) {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
    printf("\n\n");
    printf(CYAN "======================================================================\n" RESET);
    printf("  [+] SYSTEM SECURED: MISSION ACCOMPLISHED [+]\n");
    printf("  [!] SYSTEM RECOVERED [!]\n");
    printf("  Congratulations! You truly are a GENIUS!\n");
    printf(CYAN "======================================================================\n" RESET);
    printf("\n");
}
