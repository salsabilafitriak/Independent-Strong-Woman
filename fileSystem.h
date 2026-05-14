#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#define MAX_CHILDREN 10
#define MAX_NAME_LEN 50
#define MAX_PATH_LEN 256

typedef struct Folder {
    char name[MAX_NAME_LEN];
    struct Folder *children[MAX_CHILDREN];
    int child_count;
    int has_virus;          
    int is_scanned;        
    int infection_level;    
} Folder;

Folder* create_folder(const char* name);
void    add_child(Folder* parent, Folder* child);
Folder* find_child(Folder* parent, const char* name);
void    clear_virus_flags(Folder* root);
void    clear_scan_flags(Folder* root);
void    clearFiles(Folder* root);
void    free_tree(Folder* root);

void createDirectoryPlatform(const char* path);
void createDirectoryNested(const char* parent, const char* child);

#endif
