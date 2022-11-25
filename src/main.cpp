
#include <cstdlib>
#include <iostream>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "include/game.hpp"

int main(int argc, char *argv[]) {
    srand(time(0));

    TTF_Init();
    SDL_Init(SDL_INIT_EVERYTHING);

    Game Tetris = Game();
    Tetris.Runloop();

    return 0;
}

