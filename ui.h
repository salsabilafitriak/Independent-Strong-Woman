#ifndef UI_H
#define UI_H

#include "stack.h"

// Forward declaration untuk struct Folder
struct Folder;
typedef struct Folder Folder;

void drawUI(char *loc, int wave, int vHP, int pHP, char *msg, Folder *root, Folder *current, int step, int u_lim, int s_lim);
void drawGameOver(void);
void drawVictory(void);

#endif
