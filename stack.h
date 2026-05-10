#ifndef STACK_H
#define STACK_H

typedef struct Folder {
    char name[50];
    struct Folder *children[10];
    int child_count;
    int has_virus;
} Folder;

Folder* create_folder(const char* name);
void add_child(Folder* parent, Folder* child);
Folder* find_child(Folder* parent, const char* name);
void clear_virus_flags(Folder* root);
void clearFiles(Folder *root);
void createDirectoryPlatform(const char *path);

#endif
