
#include <cstdlib>
#include <iostream>
#include <vector>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

// #include "include/game.hpp"
#include "include/util.hpp"

void SetUpGame();
void ProceesInput();
void UpdateGame();
void RotateTetromino();
void GetTetrominoDown();
void ClearLine();
bool CheckBottomBorder();
bool CheckHorizontalBorder(int move);

int score = 0;
SDL_Event event;
SDL_Rect Grid[10][20];
SDL_Rect scoreTextRect;

TTF_Font *font;
SDL_Surface *scoreTextSurface;
SDL_Texture *scoreTextTexture;
SDL_Renderer *GameRenderer;


int Tetromino[4][2];
int NextTetromino[4][2];
int Tetrominos[7][4][2] = {
    {{-2, 0}, {-1, 0}, { 0, 0}, { 1, 0}},
    {{ 0,-1}, {-1,-1}, {-1, 0}, { 0, 0}},
    {{-1, 0}, {-1, 1}, { 0, 0}, { 0,-1}},
    {{ 0, 0}, {-1, 0}, { 0, 1}, {-1,-1}},
    {{ 0, 0}, { 0,-1}, { 0, 1}, {-1,-1}},
    {{ 0, 0}, { 0,-1}, { 0, 1}, { 1,-1}},
    {{ 0, 0}, { 0,-1}, { 0, 1}, {-1, 0}} 
};

int main(int argc, char *argv[]) {
    srand(time(0));

    TTF_Init();
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window *GameWindow = SDL_CreateWindow("Tetris Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, (TILE * HORIZONTAL_GRID * 2.4), (TILE * VERTICAL_GRID), 0);
    GameRenderer = SDL_CreateRenderer(GameWindow, -1, SDL_RENDERER_ACCELERATED);

    font = TTF_OpenFont("../fonts/UbuntuMono-Regular.ttf", 64);
    if (!font) std::cout << "Failed to load font: " << TTF_GetError() << std::endl;

    SDL_Surface* titleTextSurface = TTF_RenderText_Solid(font, "Tetris Game", {255, 255, 255});
    SDL_Texture* titleTextTexture = SDL_CreateTextureFromSurface(GameRenderer, titleTextSurface);

    scoreTextSurface = TTF_RenderText_Solid(font, ("SCORE - " + std::to_string(score)).c_str(), {255, 255, 255});
    scoreTextTexture = SDL_CreateTextureFromSurface(GameRenderer, scoreTextSurface);

    SDL_Rect titleTextRect = {320, 20, titleTextSurface->w, titleTextSurface->h};
    SDL_FreeSurface(titleTextSurface);

    scoreTextRect = {320, 500, scoreTextSurface->w, scoreTextSurface->h};
    SDL_FreeSurface(scoreTextSurface);
  
    SetUpGame(); 
    while (GameLoop) {
        SDL_RenderClear(GameRenderer);
        while (SDL_PollEvent(&event)) ProceesInput();

        // Draw Grid
        SDL_SetRenderDrawColor(GameRenderer, 200, 200, 200, 255);
        for (int i = 0; i < HORIZONTAL_GRID; i++) {
            for (int j = 0; j < VERTICAL_GRID; j++) {
                SDL_RenderDrawRect(GameRenderer, &Grid[i][j]);
            }
        }
        
        // Draw Already Dropped Tetrominos
        SDL_SetRenderDrawColor(GameRenderer, 255, 255, 0, 255);
        for (int x = 0; x < 10; x++) {
            for (int y = 0; y < 20; y++) {
                if (TetrominoCoveredGrid[x][y]) {
                    SDL_Rect rect = {TILE * x, TILE * y, TILE - 1, TILE - 1};
                    SDL_RenderFillRect(GameRenderer, &rect);
                }
            }
        }

        // Draw Dropping Tetromino
        SDL_SetRenderDrawColor(GameRenderer, 0, 255, 0, 255);
        for (int i = 0; i < 4; i++) {
            SDL_Rect rect = {0, 0, TILE - 1, TILE - 1};
            rect.x = (Tetromino[i][0] * TILE) + (TetrominoCoord[0] * TILE);
            rect.y = (Tetromino[i][1] * TILE) + (TetrominoCoord[1] * TILE);
            SDL_RenderFillRect(GameRenderer, &rect);
        }

        // Draw Next Tetromino
        SDL_SetRenderDrawColor(GameRenderer, 255, 255, 0, 255);
        for (int i = 0; i < 4; i++) {
            SDL_Rect rect = {0, 0, TILE - 1, TILE - 1};
            rect.x = (NextTetromino[i][0] * TILE) + 15 * TILE;
            rect.y = (NextTetromino[i][1] * TILE) + 5  * TILE;
            SDL_RenderFillRect(GameRenderer, &rect);
        }

        SDL_SetRenderDrawColor(GameRenderer, 0, 0, 0, 255);
        SDL_RenderCopy(GameRenderer, titleTextTexture, NULL, &titleTextRect);
        SDL_RenderCopy(GameRenderer, scoreTextTexture, NULL, &scoreTextRect);
        SDL_RenderPresent(GameRenderer);
        SDL_Delay(200);

        UpdateGame();
    }

    TTF_CloseFont(font);
    SDL_DestroyTexture(titleTextTexture);
    SDL_DestroyRenderer(GameRenderer);
    SDL_DestroyWindow(GameWindow);
    	
    TTF_Quit();
    SDL_Quit();

    return 0;
}

void ProceesInput() {
    if (event.type == SDL_QUIT) GameLoop = false;

    else if (event.type == SDL_KEYDOWN) {
        if (event.key.keysym.sym == SDLK_ESCAPE) GameLoop = false;
        else if (event.key.keysym.sym == SDLK_DOWN) GetTetrominoDown();
        else if (event.key.keysym.sym == SDLK_UP || event.key.keysym.sym == SDLK_SPACE) RotateTetromino();
        
        else if (event.key.keysym.sym == SDLK_RIGHT) {
            if (CheckHorizontalBorder(1)) TetrominoCoord[0]++;
        } 
        else if (event.key.keysym.sym == SDLK_LEFT) {
            if (CheckHorizontalBorder(-1)) TetrominoCoord[0]--;
        } 
    }
}

void SetUpGame() {
    int RandomIndex = rand()%7;
    for (int i = 0; i < 4; i++) {
        Tetromino[i][0] = Tetrominos[RandomIndex][i][0];
        Tetromino[i][1] = Tetrominos[RandomIndex][i][1];
    }

    RandomIndex = rand()%7;
    for (int i = 0; i < 4; i++) {
        NextTetromino[i][0] = Tetrominos[RandomIndex][i][0];
        NextTetromino[i][1] = Tetrominos[RandomIndex][i][1];
    }

    for (int i = 0; i < HORIZONTAL_GRID; i++) {
        for (int j = 0; j < VERTICAL_GRID; j++) {
            SDL_Rect rect = {(i * TILE), (j * TILE), TILE, TILE};
            Grid[i][j] = rect;
        }
    }
}

void RotateTetromino() {
    for (int i = 0; i < 4; i++) {
        int X = Tetromino[i][1] + TetrominoCoord[0];
        if (X >= 10 || X < 0) return;
        int Y = (Tetromino[i][0] * (-1)) + TetrominoCoord[1];
        if (Y >= 20 ) return;
    }

    for (int i = 0; i < 4; i++) {
        int tmpCoord = Tetromino[i][0];
        Tetromino[i][0] = Tetromino[i][1];
        Tetromino[i][1] = tmpCoord * (-1);
    }
}

void GetTetrominoDown() {
    while(CheckBottomBorder()) {
        TetrominoCoord[1]++;
    }
}

void ClearLine(int y_index) {
    for (int y = y_index; y > 0; y--) {
        for (int x = 0; x < 10; x++) {
            TetrominoCoveredGrid[x][y] = TetrominoCoveredGrid[x][y-1];
        }
    }
}


bool CheckHorizontalBorder(int move) {
    for (int i = 0; i < 4; i++) {
        int X = Tetromino[i][0] + TetrominoCoord[0] + move;
        if (X < 0 || X >= 10) return false;
    }   return true;
}

bool CheckBottomBorder() {
    for (int i = 0; i < 4; i++) {
        int X = Tetromino[i][0] + TetrominoCoord[0];
        int Y = Tetromino[i][1] + TetrominoCoord[1] + 1;

        if (Y < 0) return true;
        if (TetrominoCoveredGrid[X][Y] == true) return false;
        if (Y >= 20) return false;        
    }   return true;
}

void UpdateGame() {
    if (CheckBottomBorder()) {
        TetrominoCoord[1]++;
    }
        
    else {
        for (int i = 0; i < 4; i++) {
            int x = Tetromino[i][0] + TetrominoCoord[0]; 
            int y = Tetromino[i][1] + TetrominoCoord[1];
            TetrominoCoveredGrid[x][y] = true;
        }

        for (int i = 0; i < 4; i++) {
            Tetromino[i][0] = NextTetromino[i][0];
            Tetromino[i][1] = NextTetromino[i][1];
        }

        int RandomIndex = rand()%7;
        for (int i = 0; i < 4; i++) {
            NextTetromino[i][0] = Tetrominos[RandomIndex][i][0];
            NextTetromino[i][1] = Tetrominos[RandomIndex][i][1];
        }

        TetrominoCoord[0] =  5;
        TetrominoCoord[1] = -2;
    }

    for (int y = 19; y >= 0; y--) {
        for (int x = 0; x < 10; x++) {
            if (y == 0) {
                if (TetrominoCoveredGrid[x][y]) GameLoop = false;
                else continue;
            }
                
            if (TetrominoCoveredGrid[x][y]) {
                if (x == 9) {
                    ClearLine(y);
                    score += 100;
                    scoreTextSurface = TTF_RenderText_Solid(font, ("SCORE - " + std::to_string(score)).c_str(), {255, 255, 255});
                    scoreTextTexture = SDL_CreateTextureFromSurface(GameRenderer, scoreTextSurface);

                    scoreTextRect = {320, 500, scoreTextSurface->w, scoreTextSurface->h};
                    SDL_FreeSurface(scoreTextSurface);
                } else continue;
            } else break;
        }
    }
}


