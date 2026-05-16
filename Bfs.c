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
    if (q->size >= BFS_QUEUE_SIZE) return 0; // Queue penuh
    q->data[q->rear] = f;
    q->rear  = (q->rear + 1) % BFS_QUEUE_SIZE; // Circular array
    q->size++;
    return 1;
}

Folder* bfs_dequeue(BFSQueue *q) {
    if (bfs_queue_empty(q)) return NULL;
    Folder *f = q->data[q->front];
    q->front = (q->front + 1) % BFS_QUEUE_SIZE; // Circular array
    q->size--;
    return f;
}

Folder* bfs_spread_virus(Folder *root, int wave) {
    BFSQueue q;
    bfs_queue_init(&q);
    bfs_enqueue(&q, root);

    Folder *infected[BFS_QUEUE_SIZE];
    int inf_count = 0;

    BFSQueue scan_q;
    bfs_queue_init(&scan_q);
    bfs_enqueue(&scan_q, root);
    while (!bfs_queue_empty(&scan_q)) {
        Folder *cur = bfs_dequeue(&scan_q);
        if (cur->has_virus && inf_count < BFS_QUEUE_SIZE)
            infected[inf_count++] = cur;
        for (int i = 0; i < cur->child_count; i++)
            bfs_enqueue(&scan_q, cur->children[i]);
    }

    if (inf_count == 0) return NULL;

    int spread_limit = (wave == 1) ? 1 : (wave == 2) ? 2 : MAX_CHILDREN;
    Folder *last_spread = NULL;

    for (int i = 0; i < inf_count; i++) {
        Folder *src = infected[i];
        int spread_count = 0;

        for (int j = 0; j < src->child_count && spread_count < spread_limit; j++) {
            Folder *neighbor = src->children[j];
            if (!neighbor->has_virus) {
                neighbor->infection_level++;
                if (neighbor->infection_level >= 2) {
                    neighbor->has_virus = 1; // Fully infected
                    last_spread = neighbor;
                }
                spread_count++;
            }
        }

        for (int k = 0; k < root->child_count && spread_count < spread_limit; k++) {
            Folder *sibling = root->children[k];
            if (!sibling->has_virus && sibling != src) {
                sibling->infection_level++;
                spread_count++;
            }
        }
    }

    return last_spread;
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
