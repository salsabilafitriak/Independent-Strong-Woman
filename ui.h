#ifndef UI_H
#define UI_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

void drawUI(SDL_Renderer* r, TTF_Font* font);
void handleMouseClick(int x, int y);
void attackVirus();   // NEW

#endif