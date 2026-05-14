#include "test_logic.h"
#include "fileSystem.h"
#include "history.h"
#include "Bfs.h"
#include "time_travel.h"
#include "ui.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#ifdef _WIN32
    #include <windows.h>
    #define SLEEP(s) Sleep((s)*1000)
    #define strcasecmp _stricmp
#else
    #include <unistd.h>
    #define SLEEP(s) sleep(s)
#endif

static int secret_code = 0;
static int quiz_idx    = 0;

static char *questions[] = {
    "Q1: Which data structure uses LIFO?\n    A. Stack\n    B. Queue",
    "Q2: Top-most node in a Tree is called?\n    A. Leaf\n    B. Root",
    "Q3: Best structure for folder hierarchy?\n    A. Linked List\n    B. Tree",
    "Q4: Time complexity to access Array by index?\n    A. O(1)\n    B. O(n)"
};
static char *answers[] = {"A", "B", "B", "A"};
static int TOTAL_QUESTIONS = 4;

static char last_virus_lvl1[MAX_NAME_LEN] = "";
static char last_virus_lvl2[MAX_NAME_LEN] = "";


Folder* buildTree(void) {
    createDirectoryPlatform("Root");
    Folder *root = create_folder("Root");

    const char *lvl1[] = {"System", "Users", "Database", "Network"};
    const char *lvl2[][2] = {
        {"Kernel",   "Services"},
        {"Admin",    "Guest"},
        {"Records",  "Cache"},
        {"Firewall", "Proxy"}
    };

    for (int i = 0; i < 4; i++) {
        createDirectoryPlatform(lvl1[i]);
        Folder *parent = create_folder(lvl1[i]);

        for (int j = 0; j < 2; j++) {
            createDirectoryNested(lvl1[i], lvl2[i][j]);
            Folder *child = create_folder(lvl2[i][j]);
            add_child(parent, child);
        }
        add_child(root, parent);
    }
    return root;
}

void plantVirus(Folder *root, int wave) {
    clearFiles(root);
    clear_virus_flags(root);

    int r1, r2;
    Folder *lvl1, *virus_node;

    int attempts = 0;
    do {
        r1 = rand() % root->child_count;
        lvl1 = root->children[r1];
        r2 = rand() % lvl1->child_count;
        virus_node = lvl1->children[r2];
        attempts++;
    } while (attempts < 20 &&
             strcmp(lvl1->name, last_virus_lvl1) == 0 &&
             strcmp(virus_node->name, last_virus_lvl2) == 0);

    strncpy(last_virus_lvl1, lvl1->name, MAX_NAME_LEN - 1);
    strncpy(last_virus_lvl2, virus_node->name, MAX_NAME_LEN - 1);

    virus_node->has_virus = 1;
    virus_node->infection_level = 3;

    if (wave >= 2) {
        bfs_spread_virus(root, wave);
    }

    char path_v[MAX_PATH_LEN];
    snprintf(path_v, sizeof(path_v), "%s/%s/GHOST_FILE.txt", lvl1->name, virus_node->name);

    FILE *fv = fopen(path_v, "w");
    if (!fv) return;

    if (wave == 1) {
        fprintf(fv, "W1 Virus Detected. Execute Kill Protocol.");
    } else if (wave == 2) {
        Folder *safe = NULL;
        for (int i = 0; i < root->child_count; i++) {
            if (!root->children[i]->has_virus) { safe = root->children[i]; break; }
        }
        if (safe) {
            secret_code = 1000 + rand() % 9000;
            char ps[MAX_PATH_LEN];
            snprintf(ps, sizeof(ps), "%s/SECRET_KEY.txt", safe->name);
            FILE *fs = fopen(ps, "w");
            if (fs) { fprintf(fs, "DECRYPTION KEY: %d", secret_code); fclose(fs); }
        }
        fprintf(fv, "W2 Virus Encrypted. Find the Key!");
    } else if (wave == 3) {
        if (quiz_idx < TOTAL_QUESTIONS)
            fprintf(fv, "%s", questions[quiz_idx]);
        else
            fprintf(fv, "Virus core exposed! Type 'kill' to end this!");
    }
    fclose(fv);
}

void buildPath(Folder *root, Folder *cur, char *out) {
    if (cur == root) { snprintf(out, MAX_PATH_LEN, "C:/Root/"); return; }
    for (int i = 0; i < root->child_count; i++) {
        if (root->children[i] == cur) {
            snprintf(out, MAX_PATH_LEN, "C:/Root/%s/", cur->name);
            return;
        }
        for (int j = 0; j < root->children[i]->child_count; j++) {
            if (root->children[i]->children[j] == cur) {
                snprintf(out, MAX_PATH_LEN, "C:/Root/%s/%s/",
                         root->children[i]->name, cur->name);
                return;
            }
        }
    }
    snprintf(out, MAX_PATH_LEN, "C:/Root/?/");
}

void runGameLogic(void) {
    srand((unsigned)time(NULL));
    reset_slot_usage();

    StackNode *nav_history = NULL;
    char line[150], cmd[50], input[50];
    char log_msg[512] = "System Online. Welcome back, Genius!";
    char action_msg[128] = ""; 
    char loc[MAX_PATH_LEN];

    int wave = 1, vHP = 100, pHP = 100;

    Folder *fs_root  = buildTree();
    Folder *cur_node = fs_root;

    drawWaveTransition(1, 0);
    plantVirus(fs_root, wave);

    while (1) {
        if (vHP <= 0) {
            if (wave < 3) {
                drawWaveTransition(wave + 1, 1);
                wave++;
                vHP = (wave == 2) ? 150 : 200;
                quiz_idx = 0;
                clear_history(&nav_history);
                cur_node = fs_root;
                plantVirus(fs_root, wave);
                drawWaveTransition(wave, 0);
                strcpy(log_msg, "Wave cleared! Virus mutated...");
                continue;
            } else {
                printf("\n\033[1;31mSegmentation fault (core dumped)\033[0m\n");
                SLEEP(2);
                drawVictory();
                printf("\n\033[1;32m [!] SYSTEM RECOVERED [!]\033[0m\n");
                printf("\033[1;36m Congratulations! You truly are a GENIUS!\033[0m\n");
                clear_history(&nav_history);
                free_tree(fs_root);
                break;
            }
        }

        if (pHP <= 0) {
            drawGameOver();
            clear_history(&nav_history);
            free_tree(fs_root);
            break;
        }
        if (wave == 1) {
            snprintf(action_msg, sizeof(action_msg), "ACTION: Find GHOST_FILE.txt and use 'kill'");
        } else if (wave == 2) {
            if (secret_code != 0)
                snprintf(action_msg, sizeof(action_msg), "ACTION: Use 'kill [code]' to decrypt");
            else
                snprintf(action_msg, sizeof(action_msg), "ACTION: Search for SECRET_KEY.txt");
        } else if (wave == 3) {
            snprintf(action_msg, sizeof(action_msg), "ACTION: Answer quiz in GHOST_FILE.txt with 'kill [A/B]'");
        }

        buildPath(fs_root, cur_node, loc);
        int step = (wave == 2 && cur_node->has_virus) ? 3 : (cur_node->has_virus ? 2 : 1);
        
        // Render UI with Action Message
        drawUI(loc, wave, vHP, pHP, log_msg, fs_root, cur_node, step, stack_depth(nav_history), action_msg);

        printf(" Admin@OS.Kill >> ");
        if (!fgets(line, sizeof(line), stdin)) continue;
        int args = sscanf(line, "%49s %49s", cmd, input);
        if (args < 1) continue;

        if (strcmp(cmd, "cd") == 0 && args >= 2) {
            if (strcmp(input, "..") == 0) {
                Folder *prev = pop_history(&nav_history);
                cur_node = prev ? prev : fs_root;
                strcpy(log_msg, "Navigated back.");
            } else {
                Folder *t = find_child(cur_node, input);
                if (t) {
                    push_history(&nav_history, cur_node);
                    cur_node = t;
                    if (cur_node->has_virus) {
                        char p[MAX_PATH_LEN];
                        Folder *par = peek_history(nav_history);
                        if (par && par != fs_root)
                            snprintf(p, sizeof(p), "%s/%s/GHOST_FILE.txt", par->name, cur_node->name);
                        else
                            snprintf(p, sizeof(p), "%s/GHOST_FILE.txt", cur_node->name);
                        FILE *f = fopen(p, "r");
                        if (f) { fgets(log_msg, sizeof(log_msg), f); fclose(f); }
                        else strcpy(log_msg, "!! VIRUS DETECTED !! Type 'kill' to attack!");
                    } else {
                        strcpy(log_msg, "Folder accessed. System clean.");
                    }
                } else strcpy(log_msg, "ERROR: Path not found.");
            }
        }
        else if (strcmp(cmd, "kill") == 0) {
            if (cur_node->has_virus) {
                if (wave == 1) {
                    vHP -= 50;
                    strcpy(log_msg, (vHP > 0) ? "CRITICAL HIT! Virus weakened!" : "VIRUS DESTROYED!");
                } else if (wave == 2) {
                    if (args >= 2 && atoi(input) == secret_code) {
                        vHP -= 50;
                        strcpy(log_msg, (vHP > 0) ? "DECRYPTION SUCCESS! Keep attacking!" : "VIRUS DECRYPTED & DESTROYED!");
                    } else {
                        pHP -= 15;
                        strcpy(log_msg, "ACCESS DENIED! Wrong decryption code.");
                    }
                } else if (wave == 3) {
                    if (args >= 2 && strcasecmp(input, answers[quiz_idx]) == 0) {
                        vHP -= 50;
                        quiz_idx++;
                        strcpy(log_msg, "LOGIC MATCH! Core hit!");
                    } else {
                        pHP -= 30;
                        strcpy(log_msg, "WRONG ANSWER! Virus counterattacked!");
                    }
                }
            } else {
                pHP -= 20;
                strcpy(log_msg, "SYSTEM MISS! No virus here.");
            }
            plantVirus(fs_root, wave);
            Folder *prev = pop_history(&nav_history);
            cur_node = prev ? prev : fs_root;
        }
        else if (strcmp(cmd, "save") == 0 && args >= 2) {
            int res = execute_save_sequence(atoi(input), wave, pHP, vHP);
            if (res == 1) snprintf(log_msg, sizeof(log_msg), "SAVED to slot %s.", input);
            else strcpy(log_msg, "ERROR: Save failed.");
        }
        else if (strcmp(cmd, "load") == 0 && args >= 2) {
            int res = execute_load_sequence(atoi(input), &wave, &pHP, &vHP);
            if (res == 1) {
                cur_node = fs_root;
                clear_history(&nav_history);
                plantVirus(fs_root, wave);
                snprintf(log_msg, sizeof(log_msg), "TIME TRAVEL: Slot %s restored.", input);
            } else strcpy(log_msg, "ERROR: Load failed.");
        }
        else {
            strcpy(log_msg, "UNKNOWN COMMAND. Try: cd, kill, save, load");
        }
    }
}
