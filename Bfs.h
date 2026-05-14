#ifndef BFS_H
#define BFS_H

#include "fileSystem.h"

#define BFS_QUEUE_SIZE 64  

typedef struct {
    Folder *data[BFS_QUEUE_SIZE];  
    int front;                      
    int rear;                       
    int size;                       
} BFSQueue;

void    bfs_queue_init(BFSQueue *q);
int     bfs_queue_empty(BFSQueue *q);
int     bfs_enqueue(BFSQueue *q, Folder *f);
Folder* bfs_dequeue(BFSQueue *q);

int bfs_scan(Folder *start, Folder *root, char *result_log);

Folder* bfs_spread_virus(Folder *root, int wave);

Folder* bfs_find_virus(Folder *root);

#endif
