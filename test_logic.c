#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <time.h>

int can_exit = 0;

BOOL WINAPI ConsoleHandler(DWORD ctrlType) {
    if (!can_exit) {
        Beep(400, 300);
        return TRUE; 
    }
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
    char p[100];
    sprintf(p, "%s/%s", folder, filename);
    FILE *f = fopen(p, "w");
    if(f) {
        fprintf(f, "%s", content);
        fclose(f);
    }
}

void clearFiles(char* folders[], int count) {
    char p1[100], p2[100];
    for(int i = 0; i < count; i++) {
        sprintf(p1, "%s/GHOST_FILE.txt", folders[i]); remove(p1);
        sprintf(p2, "%s/SECRET_KEY.txt", folders[i]); remove(p2);
    }
}

void drawUI(char* loc, int wave, int vHP, int pHP, char* msg) {
    system("cls");
    if (vHP > 0) {
        system("color 0C");
        printf("!!! [CRITICAL SYSTEM LOCKDOWN] !!!\n");
    } else {
        system("color 0A");
        printf(">>> [SYSTEM RECOVERED] <<<\n");
    }
    
    printf("WAVE: %d  |  VIRUS HP: [%d]  |  PLAYER HP: [%d]\n", wave, vHP, pHP);
    printf("LOCATION: %s\n", loc);
    printf("----------------------------------------------------\n");
    
    // --- ส่วนวิธีเล่น (Manual) ---
    printf("[HOW TO PLAY]\n");
    printf(" 1. Use 'cd [folder]' to navigate the Tree structure.\n");
    printf(" 2. Use 'cd ..' to go back to Root.\n");
    if (wave == 1) {
        printf(" 3. Find 'GHOST_FILE.txt' and use 'kill' to eliminate it.\n");
    } else {
        printf(" 3. Find 'GHOST_FILE.txt' AND 'SECRET_KEY.txt'.\n");
        printf(" 4. Use 'attack [code]' using the secret key to damage the core.\n");
    }
    printf("----------------------------------------------------\n");
    printf("LOG: %s\n", msg);
    printf("----------------------------------------------------\n");
}

int main() {
    srand(time(NULL));
    forceLockWindow();
    SetConsoleCtrlHandler(ConsoleHandler, TRUE);

    char cmd[50], input[50], loc_display[50] = "C:/Root", log_msg[200];
    char* folders[] = {"System", "Users", "Database", "Temp", "Config", "Logs", "Drivers", "Backup"};
    
    int wave = 1, vHP = 100, pHP = 100;
    int folder_limit = 4; 
    int current_pos = -1, virus_pos, key_pos, secret_code;

    for(int i = 0; i < 4; i++) CreateDirectory(folders[i], NULL);
    clearFiles(folders, 4);

    virus_pos = rand() % folder_limit;
    createHiddenFile(folders[virus_pos], "GHOST_FILE.txt", "ROOT_ACCESS_DENIED");
    strcpy(log_msg, "Virus detected in 4 sectors. System locked.");

    while (1) {
        drawUI(loc_display, wave, vHP, pHP, log_msg);
        
        if (pHP <= 0) {
            can_exit = 1;
            system("color 4F");
            printf("\n[FATAL ERROR]: PLAYER HP EXHAUSTED. SYSTEM DESTROYED.\n");
            Sleep(3000);
            exit(0);
        }

        if (vHP <= 0 && wave == 2) {
            can_exit = 1; 
            printf("\n[SUCCESS]: Malware deleted. Press Enter to restore OS...");
            getchar(); getchar(); 
            break; 
        }

        printf("\nAdmin@OS.Kill >> ");
        if (scanf("%s", cmd) != 1) continue;

        if (strcmp(cmd, "cd") == 0) {
            scanf("%s", input);
            int found = 0;
            if (strcmp(input, "..") == 0) {
                current_pos = -1; strcpy(loc_display, "C:/Root"); found = 1;
            } else {
                for (int i = 0; i < folder_limit; i++) {
                    if (strcmp(input, folders[i]) == 0) {
                        current_pos = i; sprintf(loc_display, "C:/Root/%s", folders[i]);
                        found = 1; break;
                    }
                }
            }
            if (!found) strcpy(log_msg, "ACCESS DENIED: Folder locked or non-existent.");
            else strcpy(log_msg, "Moving through directory tree...");
        }

        else if (strcmp(cmd, "kill") == 0 && wave == 1) {
            if (current_pos == virus_pos) {
                vHP -= 50; Beep(1000, 200);
                char p[100]; sprintf(p, "%s/GHOST_FILE.txt", folders[virus_pos]); remove(p);

                if (vHP <= 0) {
                    wave = 2; vHP = 100; folder_limit = 8;
                    for(int i = 4; i < 8; i++) CreateDirectory(folders[i], NULL);
                    clearFiles(folders, 8); 
                    virus_pos = rand() % 8; key_pos = rand() % 8;
                    secret_code = 1000 + rand() % 9000;
                    createHiddenFile(folders[virus_pos], "GHOST_FILE.txt", "CORE_v2");
                    char code_str[20]; sprintf(code_str, "KEY:%d", secret_code);
                    createHiddenFile(folders[key_pos], "SECRET_KEY.txt", code_str);
                    strcpy(log_msg, "WAVE 2: Area expanded to 8 folders. Find the code!");
                } else {
                    virus_pos = rand() % 4;
                    createHiddenFile(folders[virus_pos], "GHOST_FILE.txt", "STILL_HERE");
                    strcpy(log_msg, "Virus relocated! Check other folders.");
                }
            } else {
                pHP -= 20; Beep(200, 600);
                strcpy(log_msg, "ERROR: Target not found. Feedback damage: -20 HP.");
            }
        }

        else if (strcmp(cmd, "attack") == 0 && wave == 2) {
            int guess;
            if (scanf("%d", &guess) != 1) { fflush(stdin); continue; }

            if (current_pos == virus_pos) {
                if (guess == secret_code) {
                    vHP -= 50; Beep(1500, 300);
                    char p1[100], p2[100];
                    sprintf(p1, "%s/GHOST_FILE.txt", folders[virus_pos]); remove(p1);
                    sprintf(p2, "%s/SECRET_KEY.txt", folders[key_pos]); remove(p2);
                    if (vHP <= 0) strcpy(log_msg, "FINAL BLOW! SYSTEM STABILIZED.");
                    else {
                        virus_pos = rand() % 8; key_pos = rand() % 8;
                        secret_code = 1000 + rand() % 9000;
                        createHiddenFile(folders[virus_pos], "GHOST_FILE.txt", "CORE_REGEN");
                        char code_str[20]; sprintf(code_str, "NEW_KEY:%d", secret_code);
                        createHiddenFile(folders[key_pos], "SECRET_KEY.txt", code_str);
                        strcpy(log_msg, "HIT! Virus and Code moved to new locations!");
                    }
                } else {
                    pHP -= 20; Beep(200, 600);
                    strcpy(log_msg, "WRONG CODE: Firewall backfired! -20 HP.");
                }
            } else {
                pHP -= 20; Beep(200, 600);
                strcpy(log_msg, "ATTACK FAILED: Target is in another folder! -20 HP.");
            }
        }
    }
    return 0;
}