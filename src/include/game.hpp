
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
    int delayTime = 200;
    int countLineForLevelChange = 0;
    int level = 1,  lines = 0, score = 0;
    int Tetromino[4][2];
    int NextTetromino[4][2];
    int TetrominoCoord[2] = {5, -2};

    bool GameLoop = true;
    bool GameOver = false;
    bool TetrominoCoveredGrid[10][20] = {false};

    SDL_Event event;
    SDL_Rect *Grid[10][20];
    SDL_Rect scoreTextRect;
    SDL_Rect titleTextRect;
    SDL_Rect levelTextRect;
    SDL_Rect playGameTextRect;
    SDL_Rect numLinesTextRect;
    SDL_Rect gameOverTextRect;

    TTF_Font *font;
    SDL_Window *GameWindow;
    SDL_Texture* titleTextTexture;
    SDL_Texture *scoreTextTexture;
    SDL_Texture *levelTextTexture;
    SDL_Texture *playGameTextTexture;
    SDL_Texture *numLinesTextTexture;
    SDL_Texture *gameOverTextTexture;
    SDL_Renderer *GameRenderer;

    public :
    Game();
    ~Game();

    void DrawGameMenu();
    void Runloop();
    void GameReset();
    void DrawGameOver();
    void ProceesInput();
    void UpdateGame();
    void NewTetrominoSetUp();
    void RotateTetromino();
    void ClearLine(int lineindex);
    bool CheckBottomBorder();
    bool CheckHorizontalBorder(int move);
};

#endif