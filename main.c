#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include "ui.h"
#include "stack.h"

int main(int argc, char *argv[]) {

    // ===== INIT SDL =====
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("SDL Error: %s\n", SDL_GetError());
        return 1;
    }

    if (TTF_Init() != 0) {
        printf("TTF Error: %s\n", TTF_GetError());
        return 1;
    }

    // ===== WINDOW =====
    SDL_Window* window = SDL_CreateWindow(
        "Virus System Game",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        1000, 600,
        SDL_WINDOW_SHOWN
    );

    if (!window) {
        printf("Window Error: %s\n", SDL_GetError());
        return 1;
    }

    // ===== RENDERER =====
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    if (!renderer) {
        printf("Renderer Error: %s\n", SDL_GetError());
        return 1;
    }

    // ===== FONT =====
    TTF_Font* font = TTF_OpenFont("assets/arial.ttf", 18);
    if (!font) {
        printf("Font Error: %s\n", TTF_GetError());
        return 1;
    }

    // ===== GAME LOOP =====
    int running = 1;
    SDL_Event e;

    while (running) {

        while (SDL_PollEvent(&e)) {

            if (e.type == SDL_QUIT) {
                running = 0;
            }

            // ===== KEYBOARD =====
            if (e.type == SDL_KEYDOWN) {

    switch (e.key.keysym.sym) {

        case SDLK_k:
            attackVirus();   // ✅ THIS IS THE NEW LINE
            break;

        case SDLK_u:
            popAction();
            break;

        case SDLK_t:
            pushAction("TIME TRAVEL");
            break;

        case SDLK_m:
            pushAction("MOVE (KEY)");
            break;
    }
}

            // ===== MOUSE CLICK =====
            if (e.type == SDL_MOUSEBUTTONDOWN) {
                handleMouseClick(e.button.x, e.button.y);
            }
        }

        // ===== DRAW UI =====
        drawUI(renderer, font);

        SDL_Delay(16); // ~60 FPS
    }

    // ===== CLEANUP =====
    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    TTF_Quit();
    SDL_Quit();

    return 0;
}