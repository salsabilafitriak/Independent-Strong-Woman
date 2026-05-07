#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "ui.h"

void drawCyberUI(
    int playerHP,
    int virusHP,
    int wave,
    char location[],
    char logMessage[]
) {

    system("cls");

    printf("========================================================================================================\n");
    printf("                                VIRUS ERADICATION SYSTEM                                                \n");
    printf("========================================================================================================\n");

    printf(" HP: %d/100        VIRUS HP: %d        WAVE: %d        LOCATION: %s\n",
           playerHP, virusHP, wave, location);

    printf("========================================================================================================\n");

    printf(" COMMANDS                                  SYSTEM TREE\n");
    printf("--------------------------------------------------------------------------------------------------------\n");

    printf(" cd [folder]                         \n");
    printf(" cd ..                               \n");
    printf(" ls                                  \n");
    printf(" kill                                \n");
    printf(" attack [code]                       \n");
    printf(" time                                \n");
    printf(" history                             \n");
    printf(" exit                                \n");
    printf(" open (to open txt file)                               \n");

    printf("\n");

    /*
        TREE VISUAL
    */

    if (wave == 1) {

        printf("                                              [ ROOT ]\n");
        printf("                                             /        \\\n");
        printf("                                            /          \\\n");
        printf("                                   [ System ]        [ Users ]\n");
        printf("                                      /                    \\\n");
        printf("                                     /                      \\\n");
        printf("                               [ GHOST ]                [ Temp ]\n");
    }

    else if (wave == 2) {

        printf("                                              [ ROOT ]\n");
        printf("                                   /               |                \\\n");
        printf("                                  /                |                 \\\n");
        printf("                           [ System ]         [ Users ]         [ Database ]\n");
        printf("                              |                   |                    |\n");
        printf("                         [ Virus ]           [ Logs ]         [ SECRET_KEY ]\n");
    }

    else if (wave == 3) {

        printf("                                                [ ROOT ]\n");
        printf("                                /                  |                   \\\n");
        printf("                               /                   |                    \\\n");
        printf("                        [ System ]            [ Users ]            [ Database ]\n");
        printf("                           /   \\                  |                    /    \\\n");
        printf("                          /     \\                 |                   /      \\\n");
        printf("                     [ Logs ] [ Temp ]       [ Quiz ]         [ ANSWER_A ] [ ANSWER_B ]\n");
        printf("                           |\n");
        printf("                      [ MUTATED VIRUS ]\n");
    }

    printf("\n");
    printf("========================================================================================================\n");

    printf(" PLAYER STATUS                           STACK HISTORY\n");
    printf("--------------------------------------------------------------------------------------------------------\n");

    printf(" ADMIN USER\n");
    printf(" PLAYER HP : %d\n", playerHP);
    printf(" VIRUS HP  : %d\n", virusHP);
    printf(" WAVE      : %d\n", wave);

    printf("\n");

    printf("========================================================================================================\n");

    printf(" ==================== MISSION MANUAL ====================\n");

    printf(" NAVIGATION:  'cd [folder]' to enter | 'cd ..' to exit\n");

    if (wave == 1) {

        printf(" OBJECTIVE:   Find 'GHOST_FILE.txt' in Explorer.\n");
        printf(" ACTION:      Move to folder and type 'kill'.\n");
    }

    else if (wave == 2) {

        printf(" OBJECTIVE:   Find 'SECRET_KEY.txt'.\n");
        printf(" ACTION:      Type 'attack [4-digit code]'.\n");
    }

    else if (wave == 3) {

        printf(" OBJECTIVE:   Answer Data Structure Quiz.\n");
        printf(" ACTION:      Type 'attack A' or 'attack B'.\n");
    }

    printf(" ========================================================\n");

    printf("\n");

    printf(" LOG:\n");

    printf("--------------------------------------------------------\n");

    printf(" %s\n", logMessage);

    printf("\n");

    printf(" Admin@OS.Kill >> ");

    printf("\n");

    printf("========================================================================================================\n");
}