#include "fileSystem.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void createDirectoryPlatform(const char *path) {
    char cmd[256];
    #ifdef _WIN32
        snprintf(cmd, sizeof(cmd), "mkdir \"%s\" 2>nul", path);
    #else
        snprintf(cmd, sizeof(cmd), "mkdir -p \"%s\"", path);
    #endif
    system(cmd);
}

Folder* create_folder(const char* name) {
    Folder* f = (Folder*)malloc(sizeof(Folder));
    strcpy(f->name, name);
    f->child_count = 0;
    f->has_virus = 0;
    for(int i=0; i<10; i++) f->children[i] = NULL;
    return f;
}

void add_child(Folder* parent, Folder* child) {
    if (parent->child_count < 10) parent->children[parent->child_count++] = child;
}

Folder* find_child(Folder* parent, const char* name) {
    for (int i = 0; i < parent->child_count; i++) {
        if (strcmp(parent->children[i]->name, name) == 0) return parent->children[i];
    }
    return NULL;
}

void clear_virus_flags(Folder* root) {
    for(int i=0; i<root->child_count; i++) root->children[i]->has_virus = 0;
}

void clearFiles(Folder *root) {
    if (!root) return;
    char p[256];
    for (int i = 0; i < root->child_count; i++) {
        snprintf(p, sizeof(p), "%s/GHOST_FILE.txt", root->children[i]->name); remove(p);
        snprintf(p, sizeof(p), "%s/SECRET_KEY.txt", root->children[i]->name); remove(p);
    }
}
