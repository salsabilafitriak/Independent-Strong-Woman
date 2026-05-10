#ifndef STACK_H
#define STACK_H

#define MAX_STACK 100

typedef struct {
    char action[100];
} Action;

void pushAction(const char* act);
void popAction();
int getStackSize();
const char* getAction(int i);

#endif
