#include "time_travel.h"
#include <stdio.h>
#include <string.h>

static int save_used[MAX_SLOTS + 1]; // index 1-3
static int load_used[MAX_SLOTS + 1];

void reset_slot_usage(void) {
    memset(save_used, 0, sizeof(save_used));
    memset(load_used, 0, sizeof(load_used));
}

void saveGame(int slot, int wave, int pHP, int vHP) {
    char filename[30];
    snprintf(filename, sizeof(filename), "save_slot_%d.txt", slot);
    FILE *f = fopen(filename, "w");
    if (f) {
        fprintf(f, "%d %d %d", wave, pHP, vHP);
        fclose(f);
    }
}

int loadGame(int slot, int *wave, int *pHP, int *vHP) {
    char filename[30];
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
    if (slot < 1 || slot > MAX_SLOTS) return -1;
    if (save_used[slot]) return 0;   // Slot sudah dipakai
    saveGame(slot, wave, pHP, vHP);
    save_used[slot] = 1;
    return 1;
}

int execute_load_sequence(int slot, int *wave, int *pHP, int *vHP) {
    if (slot < 1 || slot > MAX_SLOTS) return -1;
    if (load_used[slot]) return 0;
    if (loadGame(slot, wave, pHP, vHP)) {
        load_used[slot] = 1;
        return 1;
    }
    return -2;
}
