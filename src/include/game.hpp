
#ifndef GAME_HPP
#define GAME_HPP

#include <iostream>
#include <algorithm>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#define TILE            30
#define VERTICAL_GRID   20
#define HORIZONTAL_GRID 10

class Game {
    int score = 0;
    int Tetromino[4][2];
    int NextTetromino[4][2];
    int TetrominoCoord[2] = {5, -2};

    bool GameLoop = true;
    bool TetrominoCoveredGrid[10][20] = {false};

    SDL_Event event;
    SDL_Rect Grid[10][20];
    SDL_Rect scoreTextRect;
    SDL_Rect titleTextRect;

    TTF_Font *font;
    SDL_Window *GameWindow;
    SDL_Surface* titleTextSurface;
    SDL_Texture* titleTextTexture;
    SDL_Surface *scoreTextSurface;
    SDL_Texture *scoreTextTexture;
    SDL_Renderer *GameRenderer;

    public :
    Game();
    ~Game();

    void Runloop();
    void ProceesInput();
    void UpdateGame();
    void NewTetrominoSetUp();
    void RotateTetromino();
    void ClearLine(int lineindex);
    bool CheckBottomBorder();
    bool CheckHorizontalBorder(int move);
};

#endif