#include <stdio.h>
#include <string.h>
#include "stack.h"

static Action stack[MAX_STACK];
static int top = -1;

void pushAction(const char* act) {
    if (top < MAX_STACK - 1) {
        top++;
        strcpy(stack[top].action, act);
    }
}

void popAction() {
    if (top >= 0) top--;
}

int getStackSize() {
    return top + 1;
}

const char* getAction(int i) {
    if (i >= 0 && i <= top)
        return stack[i].action;
    return "";
}