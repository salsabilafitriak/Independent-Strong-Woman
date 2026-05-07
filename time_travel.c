#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "time_travel.h"

GameState history[MAX_HISTORY]; 
int current_version = 0;

Folder* create_folder(char* name) {
    Folder* new_folder = (Folder*)malloc(sizeof(Folder));
    strcpy(new_folder->name, name);
    new_folder->child_count = 0;
    for(int i = 0; i < MAX_CHILDREN; i++) new_folder->children[i] = NULL;
    return new_folder;
}

Folder* copy_tree(Folder* root) {
    if (root == NULL) return NULL;
    
    Folder* new_node = create_folder(root->name);
    new_node->child_count = root->child_count;
    for (int i = 0; i < root->child_count; i++) {
        new_node->children[i] = copy_tree(root->children[i]);
    }
    return new_node;
}

void save_checkpoint(int hp, char* loc, Folder* root) {
    if (current_version < MAX_HISTORY) {
        history[current_version].hp = hp;
        strcpy(history[current_version].current_location, loc);
        history[current_version].folder_snapshot = copy_tree(root);
        printf(">> Checkpoint %d saved: [HP: %d | Loc: %s]\n", current_version, hp, loc);
        current_version++;
    }
}

void time_travel(int version, int* current_hp, char* current_loc, Folder** current_root) {
    if (version >= 0 && version < current_version) {
        printf("\n--- Performing Time Travel to Version %d ---\n", version);
        *current_hp = history[version].hp;
        strcpy(current_loc, history[version].current_location);
        *current_root = copy_tree(history[version].folder_snapshot);
        printf("System State Successfully Restored!\n");
    } else {
        printf("Invalid Version!\n");
    }
}