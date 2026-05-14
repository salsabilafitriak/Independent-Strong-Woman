#include "history.h"
#include <stdlib.h>

void push_history(StackNode **top, Folder *f) {
    if (!f) return;
    StackNode *newNode = (StackNode*)malloc(sizeof(StackNode));
    if (!newNode) return;
    newNode->folder_ptr = f;
    newNode->next = *top;
    *top = newNode;
}

Folder* pop_history(StackNode **top) {
    if (*top == NULL) return NULL;
    StackNode *temp = *top;
    Folder    *f    = temp->folder_ptr;
    *top = (*top)->next;
    free(temp);
    return f;
}

Folder* peek_history(StackNode *top) {
    if (!top) return NULL;
    return top->folder_ptr;
}

int stack_depth(StackNode *top) {
    int depth = 0;
    while (top) { depth++; top = top->next; }
    return depth;
}

void clear_history(StackNode **top) {
    while (*top != NULL) pop_history(top);
}
