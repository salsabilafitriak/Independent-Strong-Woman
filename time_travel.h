#ifndef TIME_TRAVEL_H
#define TIME_TRAVEL_H

#define MAX_HISTORY 5
#define MAX_CHILDREN 10

typedef struct Folder {
    char name[50];
    struct Folder* children[MAX_CHILDREN];
    int child_count;
} Folder;

typedef struct {
    int hp;
    char current_location[50];
    Folder* folder_snapshot;
} GameState;

Folder* create_folder(char* name);
Folder* copy_tree(Folder* root);
void save_checkpoint(int hp, char* loc, Folder* root);
void time_travel(int version, int* current_hp, char* current_loc, Folder** current_root);

#endif
