#include "Bfs.h"
#include <stdio.h>
#include <string.h>

void bfs_queue_init(BFSQueue *q) {
    q->front = 0;
    q->rear  = 0;
    q->size  = 0;
}

int bfs_queue_empty(BFSQueue *q) {
    return q->size == 0;
}

int bfs_enqueue(BFSQueue *q, Folder *f) {
    if (q->size >= BFS_QUEUE_SIZE) return 0; 
    q->data[q->rear] = f;
    q->rear  = (q->rear + 1) % BFS_QUEUE_SIZE; 
    q->size++;
    return 1;
}

Folder* bfs_dequeue(BFSQueue *q) {
    if (bfs_queue_empty(q)) return NULL;
    Folder *f = q->data[q->front];
    q->front = (q->front + 1) % BFS_QUEUE_SIZE; 
    q->size--;
    return f;
}

Folder* bfs_find_virus(Folder *root) {
    BFSQueue q;
    bfs_queue_init(&q);
    bfs_enqueue(&q, root);

    while (!bfs_queue_empty(&q)) {
        Folder *cur = bfs_dequeue(&q);
        if (cur->has_virus) return cur;
        for (int i = 0; i < cur->child_count; i++)
            bfs_enqueue(&q, cur->children[i]);
    }
    return NULL;
}
