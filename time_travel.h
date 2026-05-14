#ifndef TIME_TRAVEL_H
#define TIME_TRAVEL_H


#define MAX_SLOTS 3

void saveGame(int slot, int wave, int pHP, int vHP);
int  loadGame(int slot, int *wave, int *pHP, int *vHP);
int  execute_save_sequence(int slot, int wave, int pHP, int vHP);
int  execute_load_sequence(int slot, int *wave, int *pHP, int *vHP);
void reset_slot_usage(void);

#endif
