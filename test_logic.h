#ifndef TEST_LOGIC_H
#define TEST_LOGIC_H

#include "stack.h"

extern int can_exit;

void runGameLogic(void);
void clearAllFiles(Folder *root);
Folder* buildTree(void);

#endif
