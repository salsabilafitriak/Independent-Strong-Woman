#include "fileSystem.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void createDirectoryPlatform(const char *path) {
    char cmd[MAX_PATH_LEN];
#ifdef _WIN32
    snprintf(cmd, sizeof(cmd), "mkdir \"%s\" 2>nul", path);
#else
    snprintf(cmd, sizeof(cmd), "mkdir -p \"%s\"", path);
#endif
    system(cmd);
}

void createDirectoryNested(const char *parent, const char *child) {
    char path[MAX_PATH_LEN];
    snprintf(path, sizeof(path), "%s/%s", parent, child);
    createDirectoryPlatform(path);
}

Folder* create_folder(const char* name) {
    Folder* f = (Folder*)malloc(sizeof(Folder));
    if (!f) return NULL;
    strncpy(f->name, name, MAX_NAME_LEN - 1);
    f->name[MAX_NAME_LEN - 1] = '\0';
    f->child_count    = 0;
    f->has_virus      = 0;
    f->infection_level = 0;
    for (int i = 0; i < MAX_CHILDREN; i++) f->children[i] = NULL;
    return f;
}

void add_child(Folder* parent, Folder* child) {
    if (!parent || !child) return;
    if (parent->child_count < MAX_CHILDREN)
        parent->children[parent->child_count++] = child;
}

Folder* find_child(Folder* parent, const char* name) {
    if (!parent) return NULL;
    for (int i = 0; i < parent->child_count; i++) {
        if (strcmp(parent->children[i]->name, name) == 0)
            return parent->children[i];
    }
    return NULL;
}

void clear_virus_flags(Folder* root) {
    if (!root) return;
    root->has_virus = 0;
    root->infection_level = 0;
    for (int i = 0; i < root->child_count; i++) {
        root->children[i]->has_virus = 0;
        root->children[i]->infection_level = 0;
        // Level 2 (grandchildren)
        for (int j = 0; j < root->children[i]->child_count; j++) {
            root->children[i]->children[j]->has_virus = 0;
            root->children[i]->children[j]->infection_level = 0;
        }
    }
}

void clearFiles(Folder* root) {
    if (!root) return;
    char p[MAX_PATH_LEN];

    for (int i = 0; i < root->child_count; i++) {
        Folder *lvl1 = root->children[i];
        snprintf(p, sizeof(p), "%s/GHOST_FILE.txt", lvl1->name); remove(p);
        snprintf(p, sizeof(p), "%s/SECRET_KEY.txt", lvl1->name); remove(p);

        for (int j = 0; j < lvl1->child_count; j++) {
            Folder *lvl2 = lvl1->children[j];
            snprintf(p, sizeof(p), "%s/%s/GHOST_FILE.txt", lvl1->name, lvl2->name); remove(p);
            snprintf(p, sizeof(p), "%s/%s/SECRET_KEY.txt", lvl1->name, lvl2->name); remove(p);
        }
    }
}

void free_tree(Folder* root) {
    if (!root) return;
    for (int i = 0; i < root->child_count; i++)
        free_tree(root->children[i]);
    free(root);
}
