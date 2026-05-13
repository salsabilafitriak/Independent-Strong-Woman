#ifndef HISTORY_H
#define HISTORY_H

#include "fileSystem.h" 

typedef struct StackNode {
    Folder *folder_ptr;        
    struct StackNode *next;    
} StackNode;

void push_history(StackNode **top, Folder *f);
Folder* pop_history(StackNode **top);
void clear_history(StackNode **top);

#endif
