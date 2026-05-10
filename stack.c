#include <stdio.h>
#include <string.h>
#include "stack.h"

/*
 * Stack — stores the player's action history using LIFO.
 * Data Structure: Stack (Array-based implementation)
 */

static Action stack[MAX_STACK];
static int top = -1;

/* Push a new action string onto the stack */
void pushAction(const char *act) {
    if (top < MAX_STACK - 1) {
        top++;
        strncpy(stack[top].action, act, sizeof(stack[top].action) - 1);
        stack[top].action[sizeof(stack[top].action) - 1] = '\0';
    }
}

/* Pop (remove) the top action from the stack */
void popAction(void) {
    if (top >= 0) top--;
}

/* Return number of items currently in the stack */
int getStackSize(void) {
    return top + 1;
}

/* Return the action string at index i (0 = bottom) */
const char *getAction(int i) {
    if (i >= 0 && i <= top)
        return stack[i].action;
    return "";
}
