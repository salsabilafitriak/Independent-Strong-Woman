#include <stdio.h>
#include <string.h>
#include "stack.h"

#define MAX_STACK 100

char historyStack[MAX_STACK][100];
int top = -1;

void pushAction(const char* action) {

    if (top < MAX_STACK - 1) {
        top++;
        strcpy(historyStack[top], action);
    }
}

void showHistory() {

    printf("\n========== ACTION HISTORY ==========\n");

    for (int i = top; i >= 0; i--) {
        printf("%d. %s\n", i + 1, historyStack[i]);
    }

    printf("====================================\n");
}