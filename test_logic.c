#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <time.h>

int can_exit = 0;

BOOL WINAPI ConsoleHandler(DWORD ctrlType) {
    if (!can_exit) { Beep(400, 300); return TRUE; }
    return FALSE; 
}

void forceLockWindow() {
    HWND hwnd = GetConsoleWindow();
    if (hwnd) {
        HMENU hmenu = GetSystemMenu(hwnd, FALSE);
        if (hmenu) DeleteMenu(hmenu, SC_CLOSE, MF_BYCOMMAND);
        SetWindowLong(hwnd, GWL_STYLE, GetWindowLong(hwnd, GWL_STYLE) & ~WS_SYSMENU);
    }
}

void createHiddenFile(char* folder, char* filename, char* content) {
    char p[100]; sprintf(p, "%s/%s", folder, filename);
    FILE *f = fopen(p, "w"); if(f) { fprintf(f, "%s", content); fclose(f); }
}

void clearFiles(char* folders[], int count) {
    char p1[100], p2[100], p3[100], p4[100];
    for(int i = 0; i < count; i++) {
        sprintf(p1, "%s/GHOST_FILE.txt", folders[i]); remove(p1);
        sprintf(p2, "%s/SECRET_KEY.txt", folders[i]); remove(p2);
        sprintf(p3, "%s/ANSWER_A.txt", folders[i]); remove(p3);
        sprintf(p4, "%s/ANSWER_B.txt", folders[i]); remove(p4);
    }
}

void drawManual(int wave) {
    printf("==================== MISSION MANUAL ====================\n");
    printf(" NAVIGATION:  'cd [folder]' to enter | 'cd ..' to exit\n");
    if (wave == 1) {
        printf(" OBJECTIVE:   Find 'GHOST_FILE.txt' in Explorer.\n");
        printf(" ACTION:      Move to folder and type 'kill'.\n");
    } 
    else if (wave == 2) {
        printf(" OBJECTIVE:   Find 'GHOST_FILE' and 'SECRET_KEY'.\n");
        printf(" COMMAND:     Type 'attack [4-digit code]'.\n");
    } 
    else if (wave == 3) {
        printf(" OBJECTIVE:   Read 'GHOST_FILE' for Data Structure Quiz.\n");
        printf(" ACTION:      Find 'ANSWER_A' or 'ANSWER_B' inside.\n");
        printf(" COMMAND:     Type 'attack A' or 'attack B'.\n");
    }
    printf("========================================================\n");
}

void drawUI(char* loc, int wave, int vHP, int pHP, char* msg) {
    system("cls");
    if (vHP > 0) system("color 0C"); else system("color 0A");
    printf(" [ OS.Kill() - STABILITY: %d%% ]\n", pHP);
    printf(" WAVE: %d | PLAYER HP: [%d] | VIRUS HP: [%d]\n", wave, pHP, vHP);
    printf(" CURRENT_NODE: %s\n\n", loc);
    drawManual(wave);
    printf("\n LOG: %s\n", msg);
    printf("--------------------------------------------------------\n");
}

int main() {
    srand(time(NULL)); forceLockWindow(); SetConsoleCtrlHandler(ConsoleHandler, TRUE);

    char cmd[50], input[50], loc_display[50] = "C:/Root", log_msg[200];
    char* folders[] = {"System", "Users", "Database", "Temp", "Config", "Logs", "Drivers", "Backup"};
    
    int wave = 1, vHP = 100, pHP = 100, folder_limit = 4, q_index = 0;
    int current_pos = -1, virus_pos, key_pos, secret_code;
    
    char* questions[] = {"Which structure uses LIFO?", "Which structure is a Hierarchy?", "Which uses FIFO?"};
    char* ans_a[] = {"Stack", "Tree", "Queue"};
    char* ans_b[] = {"Queue", "Stack", "Stack"};
    char* correct[] = {"A", "A", "A"};

    // Setup Wave 1
    for(int i = 0; i < 4; i++) CreateDirectory(folders[i], NULL);
    virus_pos = rand() % 4;
    createHiddenFile(folders[virus_pos], "GHOST_FILE.txt", "VIRUS_V1");

    while (1) {
        drawUI(loc_display, wave, vHP, pHP, log_msg);
        
        if (pHP <= 0) {
            can_exit = 1; system("color 4F");
            printf("\n !!! [CRITICAL FAILURE]: OS DESTROYED !!!\n");
            Sleep(3000); exit(0);
        }

        if (vHP <= 0 && wave == 3) {
            can_exit = 1; system("color 0A");
            printf("\n [SUCCESS]: VIRUS PURGED. PRESS ENTER TO EXIT.\n");
            getchar(); getchar(); break;
        }

        printf(" Admin@OS.Kill >> ");
        if (scanf("%s", cmd) != 1) continue;

        if (strcmp(cmd, "cd") == 0) {
            scanf("%s", input);
            int found = 0;
            if (strcmp(input, "..") == 0) { current_pos = -1; strcpy(loc_display, "C:/Root"); found = 1; }
            else {
                for (int i = 0; i < folder_limit; i++) {
                    if (strcmp(input, folders[i]) == 0) {
                        current_pos = i; sprintf(loc_display, "C:/Root/%s", folders[i]);
                        found = 1; break;
                    }
                }
            }
            if (!found) strcpy(log_msg, "Error: Node not found.");
            else strcpy(log_msg, "Moving...");
        }

        else if (strcmp(cmd, "kill") == 0 && wave == 1) {
            if (current_pos == virus_pos) {
                vHP -= 50; Beep(1000, 200);
                if (vHP <= 0) {
                    wave = 2; vHP = 100; folder_limit = 8; // เพิ่มแค่ตรงนี้เป็น 8
                    for(int i = 4; i < 8; i++) CreateDirectory(folders[i], NULL);
                    clearFiles(folders, 8);
                    virus_pos = rand() % 8; key_pos = rand() % 8; secret_code = 1000 + rand() % 8999;
                    createHiddenFile(folders[virus_pos], "GHOST_FILE.txt", "WAVE_2_CORE");
                    char code_str[20]; sprintf(code_str, "KEY:%d", secret_code);
                    createHiddenFile(folders[key_pos], "SECRET_KEY.txt", code_str);
                    strcpy(log_msg, "Wave 2: Search for the Secret Key.");
                } else {
                    virus_pos = rand() % 4; clearFiles(folders, 4);
                    createHiddenFile(folders[virus_pos], "GHOST_FILE.txt", "RELOCATED");
                    strcpy(log_msg, "Virus escaped!");
                }
            } else { pHP -= 20; strcpy(log_msg, "MISS! HP -20"); }
        }

        else if (strcmp(cmd, "attack") == 0) {
            if (wave == 2) {
                int guess; if (scanf("%d", &guess) != 1) { fflush(stdin); continue; }
                if (current_pos == virus_pos && guess == secret_code) {
                    vHP -= 50; Beep(1200, 200);
                    if (vHP <= 0) {
                        wave = 3; vHP = 150; 
                        // ** ไม่เพิ่ม folder_limit (คงที่ไว้ที่ 8) **
                        clearFiles(folders, 8);
                        q_index = rand() % 3; virus_pos = rand() % 8;
                        createHiddenFile(folders[virus_pos], "GHOST_FILE.txt", questions[q_index]);
                        createHiddenFile(folders[virus_pos], "ANSWER_A.txt", ans_a[q_index]);
                        createHiddenFile(folders[virus_pos], "ANSWER_B.txt", ans_b[q_index]);
                        strcpy(log_msg, "FINAL WAVE: Use Data Structure knowledge!");
                    } else {
                        virus_pos = rand() % 8; key_pos = rand() % 8; secret_code = 1000 + rand() % 8999;
                        clearFiles(folders, 8);
                        createHiddenFile(folders[virus_pos], "GHOST_FILE.txt", "REGEN");
                        char code_str[20]; sprintf(code_str, "CODE:%d", secret_code);
                        createHiddenFile(folders[key_pos], "SECRET_KEY.txt", code_str);
                        strcpy(log_msg, "Hit! Virus moved.");
                    }
                } else { pHP -= 20; strcpy(log_msg, "FAIL! HP -20"); }
            }
            else if (wave == 3) {
                char choice[10]; scanf("%s", choice);
                if (current_pos == virus_pos) {
                    if (_stricmp(choice, correct[q_index]) == 0) {
                        vHP -= 50; Beep(1500, 300);
                        if (vHP > 0) {
                            q_index = rand() % 3; virus_pos = rand() % 8;
                            clearFiles(folders, 8);
                            createHiddenFile(folders[virus_pos], "GHOST_FILE.txt", questions[q_index]);
                            createHiddenFile(folders[virus_pos], "ANSWER_A.txt", ans_a[q_index]);
                            createHiddenFile(folders[virus_pos], "ANSWER_B.txt", ans_b[q_index]);
                            strcpy(log_msg, "Correct! But it mutated!");
                        }
                    } else { pHP -= 30; strcpy(log_msg, "WRONG! Critical Damage! -30 HP"); }
                } else { strcpy(log_msg, "No virus in this folder."); }
            }
        }
    }
    clearFiles(folders, 8);
    return 0;
}
