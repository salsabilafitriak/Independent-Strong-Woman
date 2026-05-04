#include "ui.h"
#include "stack.h"
#include <math.h>

// ===== NODE =====
typedef struct {
    int x, y;
    int hasVirus;
    int virusHP;
} Node;

Node nodes[3] = {
    {500,150, 1, 50},
    {400,260, 1, 50},
    {600,260, 0, 0}
};

int currentNode = 0;

// ===== PLAYER =====
int playerHP = 100;

// ===== TEXT =====
void drawText(SDL_Renderer* r, TTF_Font* font, const char* text, int x, int y) {
    SDL_Color color = {0, 255, 200, 255};
    SDL_Surface* s = TTF_RenderText_Solid(font, text, color);
    if (!s) return;

    SDL_Texture* t = SDL_CreateTextureFromSurface(r, s);
    SDL_Rect rect = {x, y, s->w, s->h};
    SDL_RenderCopy(r, t, NULL, &rect);

    SDL_FreeSurface(s);
    SDL_DestroyTexture(t);
}

// ===== CIRCLE =====
void drawCircle(SDL_Renderer* r, int cx, int cy, int radius) {
    for (int w = 0; w < radius*2; w++) {
        for (int h = 0; h < radius*2; h++) {
            int dx = radius - w;
            int dy = radius - h;
            if ((dx*dx + dy*dy) <= (radius*radius)) {
                SDL_RenderDrawPoint(r, cx + dx, cy + dy);
            }
        }
    }
}

// ===== CLICK =====
void handleMouseClick(int mx, int my) {

    for (int i = 0; i < 3; i++) {

        int dx = mx - nodes[i].x;
        int dy = my - nodes[i].y;

        if ((dx*dx + dy*dy) <= (20*20)) {
            currentNode = i;
            pushAction("MOVE");
        }
    }
}

// ===== ATTACK =====
void attackVirus() {

    Node* n = &nodes[currentNode];

    if (!n->hasVirus) {
        pushAction("NO VIRUS");
        return;
    }

    // player attacks
    n->virusHP -= 20;

    // virus counter-attacks
    playerHP -= 10;

    pushAction("ATTACK");

    if (n->virusHP <= 0) {
        n->hasVirus = 0;
        pushAction("VIRUS KILLED");
    }
}

// ===== DRAW UI =====
void drawUI(SDL_Renderer* r, TTF_Font* font) {

    SDL_SetRenderDrawColor(r, 15, 15, 30, 255);
    SDL_RenderClear(r);

    // ===== PANELS =====
    SDL_SetRenderDrawColor(r, 25, 25, 45, 255);

    SDL_Rect topBar = {0, 0, 1000, 60};
    SDL_RenderFillRect(r, &topBar);

    SDL_Rect leftPanel = {0, 60, 200, 540};
    SDL_RenderFillRect(r, &leftPanel);

    SDL_Rect rightPanel = {750, 60, 250, 540};
    SDL_RenderFillRect(r, &rightPanel);

    // ===== TOP INFO =====
    char hpText[50];
    sprintf(hpText, "HP: %d", playerHP);
    drawText(r, font, hpText, 30, 20);

    // ===== COMMANDS =====
    drawText(r, font, "COMMANDS", 20, 80);
    drawText(r, font, "[Click] Move", 20, 120);
    drawText(r, font, "[K] Attack", 20, 160);
    drawText(r, font, "[U] Undo", 20, 200);

    // ===== TREE =====
    SDL_SetRenderDrawColor(r, 0, 255, 120, 255);

    SDL_RenderDrawLine(r, nodes[0].x, nodes[0].y, nodes[1].x, nodes[1].y);
    SDL_RenderDrawLine(r, nodes[0].x, nodes[0].y, nodes[2].x, nodes[2].y);

    for (int i = 0; i < 3; i++) {

        if (nodes[i].hasVirus) {
            SDL_SetRenderDrawColor(r, 255, 50, 50, 255); // red virus
        } else {
            SDL_SetRenderDrawColor(r, 0, 255, 120, 255);
        }

        drawCircle(r, nodes[i].x, nodes[i].y, 20);
    }

    // current node highlight
    SDL_SetRenderDrawColor(r, 0, 150, 255, 255);
    drawCircle(r, nodes[currentNode].x, nodes[currentNode].y, 10);

    // ===== STACK =====
    drawText(r, font, "HISTORY", 800, 80);

    for (int i = 0; i < getStackSize(); i++) {
        drawText(r, font, getAction(i), 800, 120 + i * 30);
    }

    // ===== WIN / LOSE =====
    int allDead = 1;
    for (int i = 0; i < 3; i++) {
        if (nodes[i].hasVirus) allDead = 0;
    }

    if (allDead) {
        drawText(r, font, "YOU WIN!", 450, 300);
    }

    if (playerHP <= 0) {
        drawText(r, font, "GAME OVER", 450, 300);
    }

    SDL_RenderPresent(r);
}