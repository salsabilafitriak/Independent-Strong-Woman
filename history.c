#include "history.h"
#include <stdlib.h>

// Memasukkan folder ke tumpukan (Push)
void push_history(StackNode **top, Folder *f) {
    StackNode *newNode = (StackNode*)malloc(sizeof(StackNode));
    newNode->folder_ptr = f;
    newNode->next = *top;
    *top = newNode;
}

// Mengambil folder terakhir (Pop) untuk Respawn/Back
Folder* pop_history(StackNode **top) {
    if (*top == NULL) return NULL;
    StackNode *temp = *top;
    Folder *f = temp->folder_ptr;
    *top = (*top)->next;
    free(temp);
    return f;
}

// Menghapus semua riwayat (misal saat Game Over)
void clear_history(StackNode **top) {
    while (*top != NULL) {
        pop_history(top);
    }
}
