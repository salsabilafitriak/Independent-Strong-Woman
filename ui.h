#ifndef UI_H
#define UI_H

#include "fileSystem.h"

void drawUI(char *loc, int wave, int vHP, int pHP, char *msg,
            Folder *root, Folder *current, int step, int depth, char *action);

void drawWaveTransition(int wave, int type);
void drawGameOver(void);
void drawVictory(void);

#endif
