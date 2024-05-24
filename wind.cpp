#include <SDL2/SDL.h>
#include <SDL2/SDL_main.h>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>

// Screen dimensions
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

// Number of falling rectangles
const int NUM_RECT_A = 5;
const int NUM_RECT_C = 5;
const int NUM_RECT_D = 5;
const int NUM_RECT_E = 5;

// Speed of falling rectangles (pixels per frame)
const int RECT_SPEED = 3;

// Function to check if two rectangles are colliding
bool checkCollision(const SDL_Rect& a, const SDL_Rect& b) {
    int leftA = a.x;
    int rightA = a.x + a.w;
    int topA = a.y;
    int bottomA = a.y + a.h;

    int leftB = b.x;
    int rightB = b.x + b.w;
    int topB = b.y;
    int bottomB = b.y + b.h;

    if (bottomA <= topB || topA >= bottomB || rightA <= leftB || leftA >= rightB) {
        return false;
    }

    return true;
}

enum MenuOption {
    NEW_GAME,
    INSTRUCTION,
    QUIT
};

MenuOption showMenu(SDL_Renderer* renderer) {
    bool quit = false;
    SDL_Event e;

    // Load menu background
    SDL_Surface* menuSurface = SDL_LoadBMP("./assests/menu.bmp");
    if (!menuSurface) {
        std::cerr << "Unable to load menu background image! SDL_Error: " << SDL_GetError() << std::endl;
        return QUIT;
    }
    SDL_Texture* menuTexture = SDL_CreateTextureFromSurface(renderer, menuSurface);
    SDL_FreeSurface(menuSurface);
    if (!menuTexture) {
        std::cerr << "Unable to create menu background texture! SDL_Error: " << SDL_GetError() << std::endl;
        return QUIT;
    }

    MenuOption selectedOption = NEW_GAME;
    SDL_Rect newGameRect = { 300, 200, 200, 50 };
    SDL_Rect instructionRect = { 300, 300, 200, 50 };
    SDL_Rect quitRect = { 300, 400, 200, 50 };

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
                selectedOption = QUIT;
            } else if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym) {
                    case SDLK_DOWN:
                        if (selectedOption == NEW_GAME) selectedOption = INSTRUCTION;
                        else if (selectedOption == INSTRUCTION) selectedOption = QUIT;
                        break;
                    case SDLK_UP:
                        if (selectedOption == QUIT) selectedOption = INSTRUCTION;
                        else if (selectedOption == INSTRUCTION) selectedOption = NEW_GAME;
                        break;
                    case SDLK_RETURN:
                        return selectedOption;
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, menuTexture, NULL, NULL);

        // Highlight selected option
        SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF);
        if (selectedOption == NEW_GAME) SDL_RenderDrawRect(renderer, &newGameRect);
        else if (selectedOption == INSTRUCTION) SDL_RenderDrawRect(renderer, &instructionRect);
        else if (selectedOption == QUIT) SDL_RenderDrawRect(renderer, &quitRect);

        SDL_RenderPresent(renderer);
        SDL_Delay(16); // Approximately 60 FPS
    }

    SDL_DestroyTexture(menuTexture);
    return QUIT;
}

void showInstructions(SDL_Renderer* renderer) {
    bool quit = false;
    SDL_Event e;

    // Load instructions image
    SDL_Surface* instructionSurface = SDL_LoadBMP("./assests/instructions.bmp");
    if (!instructionSurface) {
        std::cerr << "Unable to load instructions image! SDL_Error: " << SDL_GetError() << std::endl;
        return;
    }
    SDL_Texture* instructionTexture = SDL_CreateTextureFromSurface(renderer, instructionSurface);
    SDL_FreeSurface(instructionSurface);
    if (!instructionTexture) {
        std::cerr << "Unable to create instructions texture! SDL_Error: " << SDL_GetError() << std::endl;
        return;
    }

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT || (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE)) {
                quit = true;
            }
        }

        SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, instructionTexture, NULL, NULL);
        SDL_RenderPresent(renderer);
        SDL_Delay(16); // Approximately 60 FPS
    }

    SDL_DestroyTexture(instructionTexture);
}

void runGame(SDL_Renderer* renderer) {
    // Load background image
    SDL_Surface* bgSurface = SDL_LoadBMP("./assests/bg.bmp");
    if (!bgSurface) {
        std::cerr << "Unable to load background image! SDL_Error: " << SDL_GetError() << std::endl;
        return;
    }
    SDL_Texture* bgTexture = SDL_CreateTextureFromSurface(renderer, bgSurface);
    SDL_FreeSurface(bgSurface);
    if (!bgTexture) {
        std::cerr << "Unable to create background texture! SDL_Error: " << SDL_GetError() << std::endl;
        return;
    }

    // Load images for moving objects
    SDL_Surface* surfaceA = SDL_LoadBMP("./assests/downA.bmp");
    SDL_Surface* surfaceC = SDL_LoadBMP("./assests/downC.bmp");
    SDL_Surface* surfaceD = SDL_LoadBMP("./assests/downD.bmp");
    SDL_Surface* surfaceE = SDL_LoadBMP("./assests/downE.bmp");
    SDL_Surface* surfaceB = SDL_LoadBMP("./assests/downB.bmp");

    if (!surfaceA || !surfaceC || !surfaceD || !surfaceE || !surfaceB) {
        std::cerr << "Unable to load image! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_DestroyTexture(bgTexture);
        return;
    }

    // Create textures from surfaces
    SDL_Texture* textureA = SDL_CreateTextureFromSurface(renderer, surfaceA);
    SDL_Texture* textureC = SDL_CreateTextureFromSurface(renderer, surfaceC);
    SDL_Texture* textureD = SDL_CreateTextureFromSurface(renderer, surfaceD);
    SDL_Texture* textureE = SDL_CreateTextureFromSurface(renderer, surfaceE);
    SDL_Texture* textureB = SDL_CreateTextureFromSurface(renderer, surfaceB);

    SDL_FreeSurface(surfaceA);
    SDL_FreeSurface(surfaceC);
    SDL_FreeSurface(surfaceD);
    SDL_FreeSurface(surfaceE);
    SDL_FreeSurface(surfaceB);

    if (!textureA || !textureC || !textureD || !textureE || !textureB) {
        std::cerr << "Unable to create texture! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_DestroyTexture(bgTexture);
        return;
    }

    // Define rectangles for the falling images
    std::vector<SDL_Rect> rectAs(NUM_RECT_A);
    std::vector<SDL_Rect> rectCs(NUM_RECT_C);
    std::vector<SDL_Rect> rectDs(NUM_RECT_D);
    std::vector<SDL_Rect> rectEs(NUM_RECT_E);

    for (int i = 0; i < NUM_RECT_A; ++i) {
        rectAs[i] = { rand() % (SCREEN_WIDTH - 100), rand() % (SCREEN_HEIGHT / 2), 50, 100 };
    }
    for (int i = 0; i < NUM_RECT_C; ++i) {
        rectCs[i] = { rand() % (SCREEN_WIDTH - 100), rand() % (SCREEN_HEIGHT / 2), 50, 100 };
    }
    for (int i = 0; i < NUM_RECT_D; ++i) {
        rectDs[i] = { rand() % (SCREEN_WIDTH - 100), rand() % (SCREEN_HEIGHT / 2), 50, 100 };
    }
    for (int i = 0; i < NUM_RECT_E; ++i) {
        rectEs[i] = { rand() % (SCREEN_WIDTH - 100), rand() % (SCREEN_HEIGHT / 2), 50, 100 };
    }

    // Define rectangle for the controlled image
    SDL_Rect rectB = { 350, 500, 50, 100 };

    // Main loop flag
    bool quit = false;
    SDL_Event e;

    // While application is running
    while (!quit) {
        // Handle events on queue
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            } else if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym) {
                    case SDLK_UP:
                        rectB.y -= 100;
                        break;
                    case SDLK_DOWN:
                        rectB.y += 100;
                        break;
                    case SDLK_LEFT:
                        rectB.x -= 50;
                        break;
                    case SDLK_RIGHT:
                        rectB.x += 50;
                        break;
                }
            }
        }

        // Move falling rectangles
        for (auto& rectA : rectAs) {
            rectA.y += RECT_SPEED;
            if (rectA.y > SCREEN_HEIGHT) {
                rectA.y = 0;
                rectA.x = rand() % (SCREEN_WIDTH - rectA.w);
            }
        }
        for (auto& rectC : rectCs) {
            rectC.y += RECT_SPEED;
            if (rectC.y > SCREEN_HEIGHT) {
                rectC.y = 0;
                rectC.x = rand() % (SCREEN_WIDTH - rectC.w);
            }
        }
        for (auto& rectD : rectDs) {
            rectD.y += RECT_SPEED;
            if (rectD.y > SCREEN_HEIGHT) {
                rectD.y = 0;
                rectD.x = rand() % (SCREEN_WIDTH - rectD.w);
            }
        }
        for (auto& rectE : rectEs) {
            rectE.y += RECT_SPEED;
            if (rectE.y > SCREEN_HEIGHT) {
                rectE.y = 0;
                rectE.x = rand() % (SCREEN_WIDTH - rectE.w);
            }
        }

        // Clear screen
        SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear(renderer);

        // Render background
        SDL_RenderCopy(renderer, bgTexture, NULL, NULL);

        // Render falling rectangles
        for (const auto& rectA : rectAs) {
            SDL_RenderCopy(renderer, textureA, NULL, &rectA);
        }
        for (const auto& rectC : rectCs) {
            SDL_RenderCopy(renderer, textureC, NULL, &rectC);
        }
        for (const auto& rectD : rectDs) {
            SDL_RenderCopy(renderer, textureD, NULL, &rectD);
        }
        for (const auto& rectE : rectEs) {
            SDL_RenderCopy(renderer, textureE, NULL, &rectE);
        }

        // Render controlled rectangle
        SDL_RenderCopy(renderer, textureB, NULL, &rectB);

        // Check for collision
        for (const auto& rectA : rectAs) {
            if (checkCollision(rectA, rectB)) {
                std::cout << "Collision detected!" << std::endl;
                quit = true;
            }
        }
        for (const auto& rectC : rectCs) {
            if (checkCollision(rectC, rectB)) {
                std::cout << "Collision detected!" << std::endl;
                quit = true;
            }
        }
        for (const auto& rectD : rectDs) {
            if (checkCollision(rectD, rectB)) {
                std::cout << "Collision detected!" << std::endl;
                quit = true;
            }
        }
        for (const auto& rectE : rectEs) {
            if (checkCollision(rectE, rectB)) {
                std::cout << "Collision detected!" << std::endl;
                quit = true;
            }
        }

        // Update screen
        SDL_RenderPresent(renderer);

        // Add a small delay to control the frame rate
        SDL_Delay(16); // Approximately 60 FPS
    }

    // Free resources and close SDL
    SDL_DestroyTexture(textureA);
    SDL_DestroyTexture(textureC);
    SDL_DestroyTexture(textureD);
    SDL_DestroyTexture(textureE);
    SDL_DestroyTexture(textureB);
    SDL_DestroyTexture(bgTexture);
}

int main(int argc, char* args[]) {
    srand(static_cast<unsigned int>(time(0)));

    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    // Create window
    SDL_Window* window = SDL_CreateWindow("SDL Collision Detection",
                                          SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                          SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    // Create renderer
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    bool running = true;
    while (running) {
        MenuOption selectedOption = showMenu(renderer);
        switch (selectedOption) {
            case NEW_GAME:
                runGame(renderer);
                break;
            case INSTRUCTION:
                showInstructions(renderer);
                break;
            case QUIT:
                running = false;
                break;
        }
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
