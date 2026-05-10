#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "time_travel.h"

/*
 * Time Travel System — saves and restores game states.
 * Data Structure: Array of GameState structs (acts as a versioned snapshot store).
 *                 Folder tree snapshots use a recursive Tree (Folder*).
 */

GameState history[MAX_HISTORY];
int current_version = 0;   /* next free slot; also doubles as count of saves made */

/* ── Tree helpers ── */

/* Allocate and initialise a new folder node */
Folder *create_folder(char *name) {
    Folder *node = (Folder *)malloc(sizeof(Folder));
    if (!node) { perror("malloc"); exit(1); }
    strncpy(node->name, name, sizeof(node->name) - 1);
    node->name[sizeof(node->name) - 1] = '\0';
    node->child_count = 0;
    for (int i = 0; i < MAX_CHILDREN; i++) node->children[i] = NULL;
    return node;
}

/* Deep-copy a folder tree (recursive) */
Folder *copy_tree(Folder *root) {
    if (root == NULL) return NULL;
    Folder *copy = create_folder(root->name);
    copy->child_count = root->child_count;
    for (int i = 0; i < root->child_count; i++)
        copy->children[i] = copy_tree(root->children[i]);
    return copy;
}

/* Free a folder tree to avoid memory leaks (recursive) */
void free_tree(Folder *root) {
    if (root == NULL) return;
    for (int i = 0; i < root->child_count; i++)
        free_tree(root->children[i]);
    free(root);
}

/* ── Checkpoint API ── */

/*
 * save_checkpoint — records the current game state into the history array.
 * Limited to MAX_HISTORY (5) slots; the caller also enforces a 3-save cap.
 */
void save_checkpoint(int hp, char *loc, Folder *root) {
    if (current_version >= MAX_HISTORY) {
        printf(">> Cannot save: history array is full (%d/%d).\n",
               current_version, MAX_HISTORY);
        return;
    }

    history[current_version].hp = hp;
    strncpy(history[current_version].current_location, loc,
            sizeof(history[current_version].current_location) - 1);
    history[current_version].current_location[
        sizeof(history[current_version].current_location) - 1] = '\0';

    /* If a folder tree is provided, deep-copy it; otherwise set to NULL */
    if (root != NULL) {
        history[current_version].folder_snapshot = copy_tree(root);
    } else {
        history[current_version].folder_snapshot = NULL;
    }

    printf(">> Checkpoint %d saved: [HP: %d | Loc: %s]\n",
           current_version + 1, hp, loc);

    current_version++;
}

/*
 * time_travel — restores a previously saved state.
 * 'version' is 1-based (matching what the player types: "travel 1").
 */
void time_travel(int version, int *current_hp, char *current_loc,
                 Folder **current_root) {
    int index = version - 1;   /* convert to 0-based index */

    if (index < 0 || index >= current_version) {
        printf(">> Invalid checkpoint number. Valid range: 1 to %d.\n",
               current_version);
        return;
    }

    printf("\n--- Restoring to Checkpoint %d ---\n", version);

    *current_hp = history[index].hp;
    strncpy(current_loc, history[index].current_location,
            49);   /* caller's buffer is 50 bytes */
    current_loc[49] = '\0';

    /* Restore folder tree snapshot if both pointers are valid */
    if (current_root != NULL && history[index].folder_snapshot != NULL) {
        if (*current_root != NULL) free_tree(*current_root);
        *current_root = copy_tree(history[index].folder_snapshot);
    }

    printf(">> State restored: [HP: %d | Loc: %s]\n", *current_hp, current_loc);
}
