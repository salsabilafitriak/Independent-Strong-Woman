#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "test_logic.h"
#include "time_travel.h"
#include "stack.h"
#include "ui.h"        /* drawUI, drawManual, drawGameOver, drawVictory */

#ifdef _WIN32
    #include <windows.h>
#else
    #include <unistd.h>
    #include <strings.h>
    #define Sleep(ms)       usleep((ms) * 1000)
    #define Beep(freq, dur) ((void)0)
    #define _stricmp        strcasecmp
#endif

/* ------------------------------------------------------------------ */
/*  Global flags                                                        */
/* ------------------------------------------------------------------ */
int can_exit = 0;

/* ------------------------------------------------------------------ */
/*  Platform helpers                                                    */
/* ------------------------------------------------------------------ */
#ifdef _WIN32
BOOL WINAPI ConsoleHandler(DWORD ctrlType) {
    if (!can_exit) { Beep(400, 300); return TRUE; }
    return FALSE;
}

void forceLockWindow(void) {
    HWND hwnd = GetConsoleWindow();
    if (hwnd) {
        HMENU hmenu = GetSystemMenu(hwnd, FALSE);
        if (hmenu) DeleteMenu(hmenu, SC_CLOSE, MF_BYCOMMAND);
        SetWindowLong(hwnd, GWL_STYLE,
                      GetWindowLong(hwnd, GWL_STYLE) & ~WS_SYSMENU);
    }
}
#else
void forceLockWindow(void) { /* no-op on Linux */ }
#endif

void createDirectoryPlatform(const char *path) {
#ifdef _WIN32
    CreateDirectory(path, NULL);
#else
    char cmd[256];
    snprintf(cmd, sizeof(cmd), "mkdir -p %s", path);
    system(cmd);
#endif
}

void createHiddenFile(char *folder, char *filename, char *content) {
    char p[150];
    snprintf(p, sizeof(p), "%s/%s", folder, filename);
    FILE *f = fopen(p, "w");
    if (f) { fprintf(f, "%s", content); fclose(f); }
}

void clearFiles(char *folders[], int count) {
    char p[150];
    for (int i = 0; i < count; i++) {
        snprintf(p, sizeof(p), "%s/GHOST_FILE.txt", folders[i]); remove(p);
        snprintf(p, sizeof(p), "%s/SECRET_KEY.txt", folders[i]); remove(p);
        snprintf(p, sizeof(p), "%s/ANSWER_A.txt",   folders[i]); remove(p);
        snprintf(p, sizeof(p), "%s/ANSWER_B.txt",   folders[i]); remove(p);
    }
}

/* ------------------------------------------------------------------ */
/*  Main game loop                                                      */
/* ------------------------------------------------------------------ */
void runGameLogic(void) {
#ifdef _WIN32
    srand((unsigned)time(NULL));
    forceLockWindow();
    SetConsoleCtrlHandler(ConsoleHandler, TRUE);
#else
    srand((unsigned)time(NULL));
#endif

    char cmd[50], input[50];
    char loc_display[50] = "C:/Root";
    char log_msg[200]    = "System initialized. Hunt down the GHOST_FILE!";

    /* 8 folders total; Wave 1 uses first 4, Waves 2-3 use all 8 */
    char *folders[] = {
        "System", "Users", "Database", "Temp",
        "Config", "Logs",  "Drivers",  "Backup"
    };

    int wave         = 1;
    int vHP          = 100;
    int pHP          = 100;
    int folder_limit = 4;
    int q_index      = 0;
    int current_pos  = -1;
    int virus_pos, key_pos, secret_code;
    int save_count   = 0;

    /* Data Structure quiz pool */
    char *questions[] = {
        "Which structure uses LIFO (Last-In First-Out)?",
        "Which structure represents a Hierarchy?",
        "Which structure uses FIFO (First-In First-Out)?"
    };
    char *ans_a[]   = { "Stack", "Tree",  "Queue" };
    char *ans_b[]   = { "Queue", "Stack", "Stack" };
    char *correct[] = { "A",     "A",     "A"     };

    /* Wave 1 setup */
    for (int i = 0; i < 4; i++) createDirectoryPlatform(folders[i]);
    virus_pos = rand() % 4;
    createHiddenFile(folders[virus_pos], "GHOST_FILE.txt",
                     "Do you really think you can find me with such a simple system?");

    /* ══════════════ MAIN LOOP ══════════════ */
    while (1) {
        drawUI(loc_display, wave, vHP, pHP, log_msg);

        /* Defeat check */
        if (pHP <= 0) {
            can_exit = 1;
            drawGameOver();
            Sleep(3000);
            exit(0);
        }

        /* Victory check */
        if (vHP <= 0 && wave == 3) {
            can_exit = 1;
            drawVictory();
            break;
        }

        printf(" Admin@OS.Kill >> ");
        if (scanf("%49s", cmd) != 1) continue;

        /* ─── cd ─── */
        if (strcmp(cmd, "cd") == 0) {
            if (scanf("%49s", input) != 1) continue;

            int found = 0;
            if (strcmp(input, "..") == 0) {
                current_pos = -1;
                strcpy(loc_display, "C:/Root");
                found = 1;
            } else {
                for (int i = 0; i < folder_limit; i++) {
                    if (strcmp(input, folders[i]) == 0) {
                        current_pos = i;
                        snprintf(loc_display, sizeof(loc_display),
                                 "C:/Root/%s", folders[i]);
                        found = 1;
                        break;
                    }
                }
            }

            if (!found) {
                strcpy(log_msg, "Error: folder not found. Check available nodes.");
            } else {
                char act_msg[60];
                snprintf(act_msg, sizeof(act_msg), "cd %s", input);
                pushAction(act_msg);
                strcpy(log_msg, "Navigating...");
            }
        }

        /* ─── kill ─── */
        else if (strcmp(cmd, "kill") == 0) {

            /* Wave 1 */
            if (wave == 1) {
                if (current_pos == virus_pos) {
                    vHP -= 50;
                    Beep(1000, 200);
                    pushAction("kill: W1 Hit");

                    if (vHP <= 0) {
                        /* Transition to Wave 2 */
                        wave = 2; vHP = 100; folder_limit = 8;
                        for (int i = 4; i < 8; i++)
                            createDirectoryPlatform(folders[i]);

                        virus_pos   = rand() % 8;
                        key_pos     = rand() % 8;
                        secret_code = 1000 + rand() % 8999;

                        createHiddenFile(folders[virus_pos], "GHOST_FILE.txt",
                            "The core of despair... are you brave enough to open it?");
                        char code_str[20];
                        snprintf(code_str, sizeof(code_str), "KEY:%d", secret_code);
                        createHiddenFile(folders[key_pos], "SECRET_KEY.txt", code_str);

                        strcpy(log_msg, "WAVE 2 UNLOCKED! Find the SECRET_KEY first.");
                    } else {
                        /* Virus survived, relocate */
                        virus_pos = rand() % 4;
                        clearFiles(folders, 4);
                        createHiddenFile(folders[virus_pos], "GHOST_FILE.txt",
                            "Too slow! I wasn't there to begin with.");
                        strcpy(log_msg, "Virus survived and relocated!");
                    }
                } else {
                    pHP -= 20;
                    pushAction("kill: W1 Miss");
                    strcpy(log_msg, "MISS! Wrong folder. HP -20.");
                }
            }

            /* Wave 2 */
            else if (wave == 2) {
                int guess;
                if (scanf("%d", &guess) != 1) {
                    int c; while ((c = getchar()) != '\n' && c != EOF);
                    pHP -= 20;
                    pushAction("kill: W2 Invalid");
                    strcpy(log_msg, "FAILED! Format: kill [4-digit code]. HP -20.");
                } else {
                    if (current_pos == virus_pos && guess == secret_code) {
                        vHP -= 50;
                        Beep(1200, 200);
                        pushAction("kill: W2 Hit");

                        if (vHP <= 0) {
                            /* Transition to Wave 3 */
                            wave = 3; vHP = 150;
                            clearFiles(folders, 8);
                            q_index   = rand() % 3;
                            virus_pos = rand() % 8;
                            createHiddenFile(folders[virus_pos], "GHOST_FILE.txt",
                                             questions[q_index]);
                            createHiddenFile(folders[virus_pos], "ANSWER_A.txt",
                                             ans_a[q_index]);
                            createHiddenFile(folders[virus_pos], "ANSWER_B.txt",
                                             ans_b[q_index]);
                            strcpy(log_msg, "FINAL WAVE! Use your Data Structure knowledge!");
                        } else {
                            /* Virus survived, relocate */
                            virus_pos   = rand() % 8;
                            key_pos     = rand() % 8;
                            secret_code = 1000 + rand() % 8999;
                            clearFiles(folders, 8);
                            createHiddenFile(folders[virus_pos], "GHOST_FILE.txt",
                                "Pathetic... this body is just a fragment of the darkness.");
                            char code_str[20];
                            snprintf(code_str, sizeof(code_str), "CODE:%d", secret_code);
                            createHiddenFile(folders[key_pos], "SECRET_KEY.txt", code_str);
                            strcpy(log_msg, "Hit! Virus relocated. Find the new key.");
                        }
                    } else {
                        pHP -= 20;
                        pushAction("kill: W2 Miss");
                        strcpy(log_msg, "FAILED! Wrong folder or wrong code. HP -20.");
                    }
                }
            }

            /* Wave 3 */
            else if (wave == 3) {
                char choice[10];
                if (scanf("%9s", choice) == 1) {
                    if (current_pos == virus_pos) {
                        if (_stricmp(choice, correct[q_index]) == 0) {
                            vHP -= 50;
                            Beep(1500, 300);
                            pushAction("kill: W3 Correct");

                            if (vHP > 0) {
                                /* Virus still alive, new question */
                                q_index   = rand() % 3;
                                virus_pos = rand() % 8;
                                clearFiles(folders, 8);
                                createHiddenFile(folders[virus_pos], "GHOST_FILE.txt",
                                                 questions[q_index]);
                                createHiddenFile(folders[virus_pos], "ANSWER_A.txt",
                                                 ans_a[q_index]);
                                createHiddenFile(folders[virus_pos], "ANSWER_B.txt",
                                                 ans_b[q_index]);
                                strcpy(log_msg, "Correct! Virus weakened but still alive...");
                            }
                        } else {
                            pHP -= 30;
                            pushAction("kill: W3 Wrong Answer");
                            strcpy(log_msg, "WRONG ANSWER! Critical damage! HP -30.");
                        }
                    } else {
                        pHP -= 30;
                        pushAction("kill: W3 Wrong Folder");
                        strcpy(log_msg, "WRONG FOLDER! No target here! HP -30.");
                    }
                }
            }
        }

        /* ─── undo ─── */
        else if (strcmp(cmd, "undo") == 0) {
            popAction();
            strcpy(log_msg, "Last action removed from the stack (undo).");
        }

        /* ─── save ─── */
        else if (strcmp(cmd, "save") == 0) {
            if (save_count < 3) {
                save_count++;
                save_checkpoint(pHP, loc_display, NULL);
                snprintf(log_msg, sizeof(log_msg),
                         "Checkpoint saved! (%d/3 saves used)", save_count);
                pushAction("save");
            } else {
                strcpy(log_msg, "Save limit reached (3/3). Cannot save anymore!");
            }
        }

        /* ─── travel ─── */
        else if (strcmp(cmd, "travel") == 0) {
            int version;
            if (scanf("%d", &version) == 1) {
                time_travel(version, &pHP, loc_display, NULL);
                current_pos = -1;
                strcpy(log_msg, "System restored to previous checkpoint.");
                pushAction("travel");
            } else {
                strcpy(log_msg, "Usage: travel [checkpoint number]");
            }
        }

        /* ─── unknown ─── */
        else {
            strcpy(log_msg, "Unknown command. Try: cd, kill, save, travel, undo.");
        }
    }

    clearFiles(folders, 8);
}
