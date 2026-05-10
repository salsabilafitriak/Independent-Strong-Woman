#include "time_travel.h"
#include <stdio.h>

static int save_used[4] = {0, 0, 0, 0};
static int load_used[4] = {0, 0, 0, 0};

void saveGame(int slot, int wave, int pHP, int vHP) {
    char filename[20];
    snprintf(filename, sizeof(filename), "save_slot_%d.txt", slot);
    FILE *f = fopen(filename, "w");
    if (f) {
        fprintf(f, "%d %d %d", wave, pHP, vHP);
        fclose(f);
    }
}

int loadGame(int slot, int *wave, int *pHP, int *vHP) {
    char filename[20];
    snprintf(filename, sizeof(filename), "save_slot_%d.txt", slot);
    FILE *f = fopen(filename, "r");
    if (f) {
        fscanf(f, "%d %d %d", wave, pHP, vHP);
        fclose(f);
        return 1;
    }
    return 0;
}

int execute_save_sequence(int slot, int wave, int pHP, int vHP) {
    if (slot < 1 || slot > 3) return -1; 
    if (save_used[slot]) return 0;
    saveGame(slot, wave, pHP, vHP);
    save_used[slot] = 1;
    return 1;
}

int execute_load_sequence(int slot, int *wave, int *pHP, int *vHP) {
    if (slot < 1 || slot > 3) return -1;
    if (load_used[slot]) return 0;
    if (loadGame(slot, wave, pHP, vHP)) {
        load_used[slot] = 1;
        return 1;
    }
    return -2;
}
