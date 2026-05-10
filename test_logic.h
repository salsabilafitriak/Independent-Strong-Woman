#ifndef TEST_LOGIC_H
#define TEST_LOGIC_H

#ifdef _WIN32
    #include <windows.h>
#endif

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Shared flag: set to 1 when the process is allowed to exit */
extern int can_exit;

/* Platform / Window helpers */
#ifdef _WIN32
    BOOL WINAPI ConsoleHandler(DWORD ctrlType);
#endif
void forceLockWindow(void);

/* File & directory utilities */
void createDirectoryPlatform(const char *path);
void createHiddenFile(char *folder, char *filename, char *content);
void clearFiles(char *folders[], int count);

/* UI */
void drawManual(int wave);
void drawUI(char *loc, int wave, int vHP, int pHP, char *msg);

/* Entry point for the game */
void runGameLogic(void);

#endif /* TEST_LOGIC_H */
