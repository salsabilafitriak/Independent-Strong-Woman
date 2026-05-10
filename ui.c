#include <stdio.h>
#include <stdlib.h>
#include "ui.h"

#define RESET   "\033[0m"
#define RED     "\033[1;31m"
#define GREEN   "\033[1;32m"
#define YELLOW  "\033[1;33m"
#define BLUE    "\033[1;34m"
#define CYAN    "\033[1;36m"
#define WHITE   "\033[1;37m"

void printMissionShort(int wave, int step) {
    printf(YELLOW " [ MISSION GOAL ]" RESET);
    if (wave == 1) {
        if (step == 1) printf("\n | ACTION: " WHITE "cd [folder]" RESET " -> Scan GHOST_FILE.txt");
        else printf("\n | ACTION: " RED "kill" RESET " -> Attack (2x needed)");
    } 
    else if (wave == 2) {
        if (step == 1) printf("\n | ACTION: " WHITE "cd [folder]" RESET " -> Locate encrypted Virus");
        else if (step == 2) printf("\n | ACTION: " WHITE "cd .." RESET " -> Find " CYAN "SECRET_KEY.txt" RESET);
        else printf("\n | ACTION: " RED "kill [code]" RESET " -> (3x attack needed)");
    } 
    else if (wave == 3) {
        if (step == 1) printf("\n | ACTION: " WHITE "cd [folder]" RESET " -> Find Final Boss");
        else printf("\n | ACTION: " RED "kill [A/B]" RESET " -> Answer Quiz (4x hit)");
    }
}

void drawUI(char *loc, int wave, int vHP, int pHP, char *msg, Folder *root, Folder *current, int step, int u_lim, int s_lim) {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
    printf(CYAN "======================================================================" RESET "\n");
    printf(CYAN "      SYSTEM DEFENSE: " WHITE "OS.Kill()" CYAN " | " YELLOW "WAVE %d" RESET "\n", wave);
    printf(CYAN "======================================================================" RESET "\n");
    
    int p_fill = (pHP > 0) ? pHP / 10 : 0;
    int v_fill = (vHP > 0) ? vHP / 20 : 0; 
    printf(" PLAYER HP: " GREEN "[%-10.*s]" RESET " %3d | ", p_fill, "##########", pHP);
    printf("VIRUS HP: " RED "[%-10.*s]" RESET " %3d\n", v_fill, "!!!!!!!!!!", vHP);
    printf("----------------------------------------------------------------------\n");
    printf(BLUE " POSITION : " WHITE "%-18s" RESET " |", loc);
    printMissionShort(wave, step); 
    printf("\n----------------------------------------------------------------------\n");
    printf(GREEN " COMMANDS:" RESET " " WHITE "cd, kill, save [1-3], load [1-3]" RESET "\n");
    printf("----------------------------------------------------------------------\n");
    printf(YELLOW " [SYSTEM LOG]" RESET " >> %s\n", msg);
    printf(CYAN "======================================================================" RESET "\n");
}

void drawGameOver(void) { printf(RED "\n [!!!] SYSTEM FAILURE: DEFEATED [!!!]\n" RESET); }
void drawVictory(void) { printf(GREEN "\n [+] SYSTEM SECURED: MISSION ACCOMPLISHED [+]\n" RESET); }
