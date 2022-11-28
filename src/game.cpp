
#include "include/game.hpp"
#include "include/util.hpp"

Game::Game() {
    int randomIndex_0 = rand()%7, randomIndex_1 = rand()%7;
    for (int i = 0; i < 4; i++) {
        Tetromino[i][0] = Tetrominos[randomIndex_0][i][0];
        Tetromino[i][1] = Tetrominos[randomIndex_0][i][1];
        NextTetromino[i][0] = Tetrominos[randomIndex_1][i][0];
        NextTetromino[i][1] = Tetrominos[randomIndex_1][i][1];
    }

    for (int i = 0; i < HORIZONTAL_GRID; i++) {
        for (int j = 0; j < VERTICAL_GRID; j++) {
            SDL_Rect *rect = new SDL_Rect{(i * TILE), (j * TILE), TILE, TILE};
            Grid[i][j] = rect;
        }
    }  

    GameWindow = SDL_CreateWindow("Tetris Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, (TILE * HORIZONTAL_GRID * 2), (TILE * VERTICAL_GRID), 0);
    GameRenderer = SDL_CreateRenderer(GameWindow, -1, SDL_RENDERER_ACCELERATED);
    SDL_SetRenderDrawBlendMode(GameRenderer, SDL_BLENDMODE_BLEND);

    font = TTF_OpenFont("../fonts/UbuntuMono-Regular.ttf", 64);
    if (!font) std::cout << "Failed to load font: " << TTF_GetError() << std::endl;
    SDL_Surface *titleTextSurface = TTF_RenderText_Solid(font, "TETRIS", {255, 255, 255});
    titleTextTexture = SDL_CreateTextureFromSurface(GameRenderer, titleTextSurface);
    titleTextRect = {350, 20, titleTextSurface->w, titleTextSurface->h};
    SDL_FreeSurface(titleTextSurface);

    font = TTF_OpenFont("../fonts/UbuntuMono-Regular.ttf", 30);
    SDL_Surface *scoreTextSurface = TTF_RenderText_Solid(font, ("SCORE - " + std::to_string(score)).c_str(), {255, 255, 255});
    scoreTextTexture = SDL_CreateTextureFromSurface(GameRenderer, scoreTextSurface);
    scoreTextRect = {350, 420, scoreTextSurface->w, scoreTextSurface->h};
    SDL_FreeSurface(scoreTextSurface);
    
    SDL_Surface *numLinesTextSurface = TTF_RenderText_Solid(font, ("LINES - " + std::to_string(lines)).c_str(), {255, 255, 255});
    numLinesTextTexture = SDL_CreateTextureFromSurface(GameRenderer, numLinesTextSurface);
    numLinesTextRect = {350, 460, numLinesTextSurface->w, numLinesTextSurface->h};
    SDL_FreeSurface(numLinesTextSurface);

    SDL_Surface *levelTextSurface = TTF_RenderText_Solid(font, ("LEVEL - " + std::to_string(level)).c_str(), {255, 255, 255});
    levelTextTexture = SDL_CreateTextureFromSurface(GameRenderer, levelTextSurface);
    levelTextRect = {350, 500, levelTextSurface->w, levelTextSurface->h};
    SDL_FreeSurface(levelTextSurface);

    DrawGameMenu();
}

void Game::GameReset() {
    for (int x = 0; x < 10; x++) {
        for (int y = 0; y < 20; y++) {
            TetrominoCoveredGrid[x][y] = false;
        }
    }

    level = 1, lines = 0, score = 0;
    int randomIndex_0 = rand()%7, randomIndex_1 = rand()%7;
    for (int i = 0; i < 4; i++) {
        Tetromino[i][0] = Tetrominos[randomIndex_0][i][0];
        Tetromino[i][1] = Tetrominos[randomIndex_0][i][1];
        NextTetromino[i][0] = Tetrominos[randomIndex_1][i][0];
        NextTetromino[i][1] = Tetrominos[randomIndex_1][i][1];
    }

    scoreTextRect.x     = 350, scoreTextRect.y      = 420;
    numLinesTextRect.x  = 350, numLinesTextRect.y   = 460;
    levelTextRect.x     = 350, levelTextRect.y      = 500;

    font = TTF_OpenFont("../fonts/UbuntuMono-Regular.ttf", 30);
    SDL_Surface *scoreTextSurface = TTF_RenderText_Solid(font, ("SCORE - " + std::to_string(score)).c_str(), {255, 255, 255});
    scoreTextTexture = SDL_CreateTextureFromSurface(GameRenderer, scoreTextSurface);
    scoreTextRect = {350, 420, scoreTextSurface->w, scoreTextSurface->h};
    SDL_FreeSurface(scoreTextSurface);
    
    SDL_Surface *numLinesTextSurface = TTF_RenderText_Solid(font, ("LINES - " + std::to_string(lines)).c_str(), {255, 255, 255});
    numLinesTextTexture = SDL_CreateTextureFromSurface(GameRenderer, numLinesTextSurface);
    numLinesTextRect = {350, 460, numLinesTextSurface->w, numLinesTextSurface->h};
    SDL_FreeSurface(numLinesTextSurface);

    SDL_Surface *levelTextSurface = TTF_RenderText_Solid(font, ("LEVEL - " + std::to_string(level)).c_str(), {255, 255, 255});
    levelTextTexture = SDL_CreateTextureFromSurface(GameRenderer, levelTextSurface);
    levelTextRect = {350, 500, levelTextSurface->w, levelTextSurface->h};
    SDL_FreeSurface(levelTextSurface);
}

void Game::DrawGameMenu() {
    font = TTF_OpenFont("../fonts/UbuntuMono-Regular.ttf", 150);
    SDL_Surface *firstTitleTextSurface = TTF_RenderText_Solid(font, "TETRIS", {255, 255, 255});
    SDL_Texture *firstTitleTextTextTexture = SDL_CreateTextureFromSurface(GameRenderer, firstTitleTextSurface);
    SDL_Rect firstTitleTextRect = {100, 100, int(firstTitleTextSurface->w/1.12), firstTitleTextSurface->h};
    SDL_FreeSurface(firstTitleTextSurface);

    font = TTF_OpenFont("../fonts/UbuntuMono-Regular.ttf", 40);
    SDL_Surface *playGameTextSurface = TTF_RenderText_Solid(font, "PRESS ENTER TO PLAY GAME", {255, 255, 255});
    playGameTextTexture = SDL_CreateTextureFromSurface(GameRenderer, playGameTextSurface);
    playGameTextRect = {100, 400, int(playGameTextSurface->w/1.2), playGameTextSurface->h};
    SDL_FreeSurface(playGameTextSurface);

    SDL_SetRenderDrawColor(GameRenderer, 20, 10, 20, 255);
    SDL_Rect *rect = new SDL_Rect{(2 * TILE), (2 * TILE), TILE * 16, TILE * 16};
    SDL_RenderFillRect(GameRenderer, rect);
    SDL_RenderCopy(GameRenderer, firstTitleTextTextTexture, NULL, &firstTitleTextRect);
    SDL_RenderCopy(GameRenderer, playGameTextTexture, NULL, &playGameTextRect);
    SDL_RenderPresent(GameRenderer);
    SDL_Delay(500);

    bool loop = true;
    while (loop) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                loop = false;
                GameLoop = false;
            }
            else if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_KP_ENTER) loop = false;
            }
        }
    }
}

void Game::Runloop() {
    while (GameLoop) {
        SDL_RenderClear(GameRenderer);
        while (SDL_PollEvent(&event)) ProceesInput();

        // Draw Grid
        SDL_SetRenderDrawColor(GameRenderer, 200, 200, 200, 255);
        for (int i = 0; i < HORIZONTAL_GRID; i++) {
            for (int j = 0; j < VERTICAL_GRID; j++) {
                SDL_RenderDrawRect(GameRenderer, Grid[i][j]);
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
        SDL_SetRenderDrawColor(GameRenderer, 255, 25, 109, 255);
        for (int i = 0; i < 4; i++) {
            SDL_Rect rect = {0, 0, TILE - 1, TILE - 1};
            rect.x = (NextTetromino[i][0] * TILE) + 15 * TILE;
            rect.y = (NextTetromino[i][1] * TILE) + 8  * TILE;
            SDL_RenderFillRect(GameRenderer, &rect);
        }

        SDL_SetRenderDrawColor(GameRenderer, 0, 0, 0, 255);
        SDL_RenderCopy(GameRenderer, titleTextTexture, NULL, &titleTextRect);
        SDL_RenderCopy(GameRenderer, scoreTextTexture, NULL, &scoreTextRect);
        SDL_RenderCopy(GameRenderer, levelTextTexture, NULL, &levelTextRect);
        SDL_RenderCopy(GameRenderer, numLinesTextTexture, NULL, &numLinesTextRect);
        SDL_RenderPresent(GameRenderer);
        SDL_Delay(delayTime);

        UpdateGame();
        if (GameOver) {
            DrawGameOver();
            if (GameLoop) {
                GameReset();
            }
        }

    }
}

void Game::DrawGameOver() {
    for (int j = 0; j < VERTICAL_GRID; j++) {
        SDL_Delay(10);
        for (int i = 0; i < HORIZONTAL_GRID; i++) {
            SDL_SetRenderDrawColor(GameRenderer, rand()%255, rand()%255, rand()%255, 255);
            SDL_RenderFillRect(GameRenderer, Grid[i][j]);
            SDL_RenderPresent(GameRenderer);
            SDL_Delay(5);
        }
    }

    font = TTF_OpenFont("../fonts/UbuntuMono-Regular.ttf", 80);
    SDL_Surface *gameOverTextSurface = TTF_RenderText_Solid(font, "GAME OVER", {255, 255, 255});
    gameOverTextTexture = SDL_CreateTextureFromSurface(GameRenderer, gameOverTextSurface);
    gameOverTextRect = {120, 100, gameOverTextSurface->w, gameOverTextSurface->h};
    SDL_FreeSurface(gameOverTextSurface);

    SDL_SetRenderDrawColor(GameRenderer, 30, 30, 30, 170);
    SDL_Rect *rect = new SDL_Rect{0, 0, TILE * 30, TILE * 20};
    SDL_RenderFillRect(GameRenderer, rect);

    SDL_SetRenderDrawColor(GameRenderer, 20, 10, 20, 255);
    rect = new SDL_Rect{(2 * TILE), (2 * TILE), TILE * 16, TILE * 16};
    SDL_RenderFillRect(GameRenderer, rect);

    scoreTextRect.x     = 220, scoreTextRect.y      = 220;
    numLinesTextRect.x  = 220, numLinesTextRect.y   = 270;
    levelTextRect.x     = 220, levelTextRect.y      = 320;

    SDL_RenderCopy(GameRenderer, scoreTextTexture, NULL, &scoreTextRect);
    SDL_RenderCopy(GameRenderer, levelTextTexture, NULL, &levelTextRect);
    SDL_RenderCopy(GameRenderer, playGameTextTexture, NULL, &playGameTextRect);
    SDL_RenderCopy(GameRenderer, numLinesTextTexture, NULL, &numLinesTextRect);
    SDL_RenderCopy(GameRenderer, gameOverTextTexture, NULL, &gameOverTextRect);
    SDL_RenderPresent(GameRenderer);

    bool loop = true;
    while (loop) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                loop = false, GameLoop = false;
            }
            else if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_KP_ENTER) {
                    loop = false, GameLoop = true, GameOver = false;
                    
                }
            }
        }
    }
}

void Game::ProceesInput() {
    if (event.type == SDL_QUIT) GameLoop = false;
    else if (event.type == SDL_KEYDOWN) {
        if (event.key.keysym.sym == SDLK_ESCAPE) {
            GameLoop = false;
        } else if (event.key.keysym.sym == SDLK_SPACE) {
            RotateTetromino();
        } else if (event.key.keysym.sym == SDLK_DOWN) {
            while(CheckBottomBorder()) TetrominoCoord[1]++;
        } else if (event.key.keysym.sym == SDLK_RIGHT) {
            if (CheckHorizontalBorder(1)) TetrominoCoord[0]++;
        } else if (event.key.keysym.sym == SDLK_LEFT) {
            if (CheckHorizontalBorder(-1)) TetrominoCoord[0]--;
        } 
    }
}

void Game::RotateTetromino() {
    for (int i = 0; i < 4; i++) {
        int X = Tetromino[i][1] + TetrominoCoord[0];
        int Y = (Tetromino[i][0] * (-1)) + TetrominoCoord[1];
        if (TetrominoCoveredGrid[X][Y] || X >= 10 || X < 0 || Y >= 20) return;
    }

    for (int i = 0; i < 4; i++) {
        int tmpCoord = Tetromino[i][0];
        Tetromino[i][0] = Tetromino[i][1];
        Tetromino[i][1] = tmpCoord * (-1);
    }
}

void Game::ClearLine(int lineindex) {
    for (int y = lineindex; y > 0; y--) {
        for (int x = 0; x < 10; x++) {
            TetrominoCoveredGrid[x][y] = TetrominoCoveredGrid[x][y-1];
        }
    }
}

bool Game::CheckHorizontalBorder(int move) {
    for (int i = 0; i < 4; i++) {
        int X = Tetromino[i][0] + TetrominoCoord[0] + move;
        if (X < 0 || X >= 10) return false;
    }   return true;
}

bool Game::CheckBottomBorder() {
    for (int i = 0; i < 4; i++) {
        int X = Tetromino[i][0] + TetrominoCoord[0];
        int Y = Tetromino[i][1] + TetrominoCoord[1] + 1;

        if (Y < 0) return true;
        if (TetrominoCoveredGrid[X][Y] == true) return false;
        if (Y >= 20) return false;     
    }   return true;
}
 
void Game::UpdateGame() {
    if (CheckBottomBorder()) TetrominoCoord[1]++;   
    else NewTetrominoSetUp();

    int countline = 0;
    for (int y = 19; y >= 0; y--) {
        for (int x = 0; x < 10; x++) {
            if (y == 0) {
                if (TetrominoCoveredGrid[x][y]) {
                    GameLoop = false, GameOver = true;
                } else continue;
            } 
            
            if (TetrominoCoveredGrid[x][y]) {
                if (x == 9) {
                    ClearLine(y);
                    countLineForLevelChange++, countline++;
                } else continue;
            } else break;
        }
    }

    if (countline != 0) {
        if (countline == 1) score += 40   * level;
        if (countline == 2) score += 100  * level;
        if (countline == 3) score += 300  * level;
        if (countline == 4) score += 1200 * level;

        font = TTF_OpenFont("../fonts/UbuntuMono-Regular.ttf", 30);
        SDL_Surface *scoreTextSurface = TTF_RenderText_Solid(font, ("SCORE - " + std::to_string(score)).c_str(), {255, 255, 255});
        scoreTextTexture = SDL_CreateTextureFromSurface(GameRenderer, scoreTextSurface);
        scoreTextRect = {350, 420, scoreTextSurface->w, scoreTextSurface->h};
        SDL_FreeSurface(scoreTextSurface);
        
        lines += countline;
        SDL_Surface *numLinesTextSurface = TTF_RenderText_Solid(font, ("LINES - " + std::to_string(lines)).c_str(), {255, 255, 255});
        numLinesTextTexture = SDL_CreateTextureFromSurface(GameRenderer, numLinesTextSurface);
        numLinesTextRect = {350, 460, numLinesTextSurface->w, numLinesTextSurface->h};
        SDL_FreeSurface(numLinesTextSurface);
    }

    if (countLineForLevelChange >= 10) {
        delayTime = delayTime - (delayTime * 5/100);
        level++, countLineForLevelChange = 0;
        font = TTF_OpenFont("../fonts/UbuntuMono-Regular.ttf", 30);
        SDL_Surface *levelTextSurface  = TTF_RenderText_Solid(font, ("LEVEL - " + std::to_string(level)).c_str(), {255, 255, 255});
        levelTextTexture = SDL_CreateTextureFromSurface(GameRenderer, levelTextSurface);
        levelTextRect = {350, 500, levelTextSurface->w, levelTextSurface->h};
        SDL_FreeSurface(levelTextSurface);
    }
}

void Game::NewTetrominoSetUp() {
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

Game::~Game() {
    TTF_CloseFont(font);
    SDL_DestroyTexture(titleTextTexture);
    SDL_DestroyRenderer(GameRenderer);
    SDL_DestroyWindow(GameWindow);
    	
    TTF_Quit();
    SDL_Quit();
}