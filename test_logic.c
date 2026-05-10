#include "test_logic.h"
#include "stack.h"
#include "time_travel.h"
#include "ui.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#ifdef _WIN32
    #include <windows.h>
#else
    #include <unistd.h>
#endif

// Variabel Global
int secret_code = 0;
int quiz_idx = 0;
char *questions[] = {
    "Q1: Binary base?\nA. 10\nB. 2", 
    "Q2: 1+1 Binary?\nA. 10\nB. 2", 
    "Q3: RAM is...?\nA. Volatile\nB. Perm", 
    "Q4: CPU Brain?\nA. ALU\nB. GPU"
};
char *answers[] = {"B", "A", "A", "A"};

Folder* buildTree() {
    createDirectoryPlatform("Root");
    Folder* root = create_folder("Root");
    char *dirs[] = {"System", "Users", "Temp", "Database", "Logs", "Config", "Backup", "Drivers"};
    for (int i = 0; i < 8; i++) {
        createDirectoryPlatform(dirs[i]);
        add_child(root, create_folder(dirs[i]));
    }
    return root;
}

void plantVirus(Folder *root, int wave) {
    clearFiles(root); 
    clear_virus_flags(root);
    
    int r = rand() % root->child_count;
    root->children[r]->has_virus = 1;
    
    char path_v[256]; 
    snprintf(path_v, sizeof(path_v), "%s/GHOST_FILE.txt", root->children[r]->name);
    FILE *fv = fopen(path_v, "w");
    if (!fv) return;

    if (wave == 1) {
        fprintf(fv, "W1 Virus Detected. Execute Kill Protocol.");
    } else if (wave == 2) {
        int s; 
        do { s = rand() % root->child_count; } while (root->children[s]->has_virus);
        secret_code = 1000 + rand() % 9000;
        char ps[256]; 
        snprintf(ps, sizeof(ps), "%s/SECRET_KEY.txt", root->children[s]->name);
        FILE *fs = fopen(ps, "w"); 
        if(fs) { fprintf(fs, "DECRYPTION KEY: %d", secret_code); fclose(fs); }
        fprintf(fv, "W2 Virus Encrypted. Find the Key!");
    } else if (wave == 3) {
        if (quiz_idx < 4) {
            fprintf(fv, "%s", questions[quiz_idx]);
        } else {
            fprintf(fv, "Virus core exposed! Type 'kill' to end this!");
        }
    }
    fclose(fv);
}

void waveTransition(int wave, int type) {
    printf("\n\033[1;33m--- WAVE %d %s ---\033[0m\n", wave, type==0?"STARTED":"FINISHED");
    printf("Press Enter to continue...");
    getchar();
}

void runGameLogic(void) {
    srand((unsigned)time(NULL));
    char line[150], cmd[50], input[50], log_msg[200] = "System Online. Welcome back, Genius!";
    int wave = 1, vHP = 100, pHP = 100;
    
    Folder *fs_root = buildTree();
    Folder *cur_node = fs_root;
    
    waveTransition(1, 0); 
    plantVirus(fs_root, wave);

    while (1) {
        // --- CEK KONDISI MENANG (FAKE ERROR & GENIUS MESSAGE) ---
        if (vHP <= 0 && wave == 3) {
            printf("\n\033[1;31mSegmentation fault (core dumped)\033[0m\n");
            #ifdef _WIN32
                Sleep(2000);
            #else
                sleep(2);
            #endif
            drawVictory(); 
            printf("\n\033[1;32m [!] SYSTEM RECOVERED [!]\033[0m\n");
            printf("\033[1;36m Yey! Selamat, anda memang GENIUS! \033[0m\n");
            printf("\033[1;36m Mission Independent-Strong-Woman: SUCCESS. \033[0m\n\n");
            break; 
        }

        int step = (wave == 2 && cur_node->has_virus) ? 3 : (cur_node->has_virus ? 2 : 1);
        char loc[200]; 
        snprintf(loc, sizeof(loc), "C:/Root/%s", (cur_node == fs_root) ? "" : cur_node->name);
        drawUI(loc, wave, vHP, pHP, log_msg, fs_root, cur_node, step, 0, 0);

        if (pHP <= 0) { drawGameOver(); break; }

        printf(" Admin@OS.Kill >> ");
        if (!fgets(line, sizeof(line), stdin)) continue;
        int args = sscanf(line, "%s %s", cmd, input);
        if (args < 1) continue;

        // --- COMMAND: SAVE / LOAD ---
        if (strcmp(cmd, "save") == 0) {
            int res = execute_save_sequence(atoi(input), wave, pHP, vHP);
            if(res == 1) strcpy(log_msg, "SUCCESS: Slot saved.");
            else if(res == 0) strcpy(log_msg, "DENIED: Slot locked!");
            else strcpy(log_msg, "ERROR: Invalid slot.");
        } 
        else if (strcmp(cmd, "load") == 0) {
            int res = execute_load_sequence(atoi(input), &wave, &pHP, &vHP);
            if(res == 1) {
                cur_node = fs_root; plantVirus(fs_root, wave);
                strcpy(log_msg, "TIME TRAVEL: Timeline recovered.");
            } else strcpy(log_msg, "CRITICAL: Access denied.");
        } 
        // --- COMMAND: KILL (STRICT RANDOM RELOCATION) ---
        else if (strcmp(cmd, "kill") == 0) {
            if (cur_node->has_virus) {
                if (wave == 1) { 
                    vHP -= 50; 
                    if(vHP <= 0){ waveTransition(1,1); wave++; vHP=150; waveTransition(2,0); }
                    else strcpy(log_msg, "CRITICAL HIT! But the virus escaped!"); 
                }
                else if (wave == 2) {
                    if (args >= 2 && atoi(input) == secret_code) { 
                        vHP -= 50; 
                        if(vHP <= 0){ waveTransition(2,1); wave++; vHP=200; waveTransition(3,0); }
                        else strcpy(log_msg, "DECRYPTION SUCCESS! Genius move, but it fled!");
                    } else { 
                        pHP -= 15; strcpy(log_msg, "ACCESS DENIED! Wrong code. Virus escaped!"); 
                    }
                } 
                else if (wave == 3) {
                    if (args >= 2 && strcasecmp(input, answers[quiz_idx]) == 0) {
                        vHP -= 50; strcpy(log_msg, "LOGIC MATCH! Core hit, but it relocated!");
                    } else { 
                        pHP -= 30; strcpy(log_msg, "TERMINAL ERROR! Wrong answer. Virus fled!"); 
                    }
                    quiz_idx++; 
                }
            } else { 
                pHP -= 20; strcpy(log_msg, "SYSTEM MISS! Target folder empty. Virus fled!"); 
            }
            
            // --- NO MERCY ZONE: Virus ALWAYS moves after any kill attempt ---
            plantVirus(fs_root, wave); 
            cur_node = fs_root; 
        } 
        // --- COMMAND: CD ---
        else if (strcmp(cmd, "cd") == 0 && args >= 2) {
            if (strcmp(input, "..") == 0) cur_node = fs_root;
            else {
                Folder *t = find_child(cur_node, input);
                if(t) {
                    cur_node = t;
                    if(cur_node->has_virus) {
                        char p[256]; snprintf(p, sizeof(p), "%s/GHOST_FILE.txt", cur_node->name);
                        FILE *f = fopen(p, "r"); if(f){fgets(log_msg, 200, f); fclose(f);}
                    }
                } else strcpy(log_msg, "ERROR: Path not found.");
            }
        }
    }
}
