
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <vector>

#include <SDL2/SDL.h>

#define TILE            30
#define VERTICAL_GRID   20
#define HORIZONTAL_GRID 10

void SetUpGame();
void ProceesInput();
void UpdateGame();
void RotateTetromino();
void GetTetrominoDown();
void ClearLine();
bool CheckBottomBorder();
bool CheckHorizontalBorder(int move);

bool GameLoop;
int  TetrominoCoord[] = {5, -2};
bool TetrominoCoveredGrid[10][20] = {false};

SDL_Event event;
SDL_Rect Grid[10][20];

int Tetromino[4][2];
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

    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window *GameWindow = SDL_CreateWindow("Tetris Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, TILE * HORIZONTAL_GRID * 3, (TILE * VERTICAL_GRID), 0);
    SDL_Renderer *GameRenderer = SDL_CreateRenderer(GameWindow, -1, SDL_RENDERER_ACCELERATED);

    SetUpGame(); 
    while (GameLoop) {
        SDL_RenderClear(GameRenderer);
        while (SDL_PollEvent(&event)) {
            ProceesInput();
        }

        SDL_SetRenderDrawColor(GameRenderer, 200, 200, 200, 255);
        for (int i = 0; i < HORIZONTAL_GRID; i++) {
            for (int j = 0; j < VERTICAL_GRID; j++) {
                SDL_RenderDrawRect(GameRenderer, &Grid[i][j]);
            }
        }
        

        SDL_SetRenderDrawColor(GameRenderer, 255, 255, 0, 255);
        for (int x = 0; x < 10; x++) {
            for (int y = 0; y < 20; y++) {
                if (TetrominoCoveredGrid[x][y]) {
                    SDL_Rect rect;
                    rect.x = TILE * x;
                    rect.y = TILE * y;
                    rect.w = TILE - 1;
                    rect.h = TILE - 1;
                    SDL_RenderFillRect(GameRenderer, &rect);
                }
            }
        }

        SDL_SetRenderDrawColor(GameRenderer, 0, 255, 0, 255);
        for (int i = 0; i < 4; i++) {
            SDL_Rect rect;
            rect.x = (Tetromino[i][0] * TILE) + (TetrominoCoord[0] * TILE);
            rect.y = (Tetromino[i][1] * TILE) + (TetrominoCoord[1] * TILE);
            rect.w = TILE - 1;
            rect.h = TILE - 1;
            SDL_RenderFillRect(GameRenderer, &rect);
        }

        SDL_SetRenderDrawColor(GameRenderer, 0, 0, 0, 255);
        SDL_RenderPresent(GameRenderer);
        SDL_Delay(200);

        UpdateGame();
    }

    SDL_DestroyRenderer(GameRenderer);
    SDL_DestroyWindow(GameWindow);
    SDL_Quit();

    return 0;
}

void ProceesInput() {
    if (event.type == SDL_QUIT) 
        GameLoop = false;

    else if (event.type == SDL_KEYDOWN) {
        if (event.key.keysym.sym == SDLK_ESCAPE) GameLoop = false;

        else if (event.key.keysym.sym == SDLK_RIGHT) {
            if (CheckHorizontalBorder(1)) TetrominoCoord[0]++;
        } 
        
        else if (event.key.keysym.sym == SDLK_LEFT) {
            if (CheckHorizontalBorder(-1)) TetrominoCoord[0]--;
        } 
        
        else if (event.key.keysym.sym == SDLK_DOWN) {
            GetTetrominoDown();
        } 
        
        else if (event.key.keysym.sym == SDLK_UP || event.key.keysym.sym == SDLK_SPACE) {
            RotateTetromino();
        } 
        
    }
}

void SetUpGame() {
    GameLoop = true;

    int RandomIndex = rand()%7;
    for (int i = 0; i < 4; i++) {
        Tetromino[i][0] = Tetrominos[RandomIndex][i][0];
        Tetromino[i][1] = Tetrominos[RandomIndex][i][1];
    }

    for (int x = 0; x < 10; x++) {
        for (int y = 0; y < 20; y++) {
            TetrominoCoveredGrid[10][20] = false;
        }
    }

    for (int i = 0; i < HORIZONTAL_GRID; i++) {
        for (int j = 0; j < VERTICAL_GRID; j++) {
            SDL_Rect rect;
            rect.x = (i * TILE);
            rect.y = (j * TILE);
            rect.w = TILE;
            rect.h = TILE;
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
}void GetTetrominoDown();


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

        int RandomIndex = rand()%7;
        for (int i = 0; i < 4; i++) {
            Tetromino[i][0] = Tetrominos[RandomIndex][i][0];
            Tetromino[i][1] = Tetrominos[RandomIndex][i][1];
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
                if (x == 9) ClearLine(y);
                else continue;
            } else break;
        }
    }
}


