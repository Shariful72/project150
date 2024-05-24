#include <SDL2/SDL.h>
#include <SDL2/SDL_main.h>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>



// Screen dimensions
const int SCREEN_WIDTH = 600;
const int SCREEN_HEIGHT = 600;

// Number of falling rectangles
const int NUM_RECT_A = 4;

// Speed of falling rectangles (pixels per frame)
const int RECT_A_SPEED = 2;

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

    // Load background image
    SDL_Surface* bgSurface = SDL_LoadBMP("./assests/bg.bmp");
    if (!bgSurface) {
        std::cerr << "Unable to load background image! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }
    SDL_Texture* bgTexture = SDL_CreateTextureFromSurface(renderer, bgSurface);
    SDL_FreeSurface(bgSurface);
    if (!bgTexture) {
        std::cerr << "Unable to create background texture! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Load images for moving objects
    SDL_Surface* surfaceA = SDL_LoadBMP("./assests/down.bmp");
    SDL_Surface* surfaceB = SDL_LoadBMP("./assests/down.bmp");
    if (!surfaceA || !surfaceB) {
        std::cerr << "Unable to load image! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_DestroyTexture(bgTexture);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Create textures from surfaces
    SDL_Texture* textureA = SDL_CreateTextureFromSurface(renderer, surfaceA);
    SDL_Texture* textureB = SDL_CreateTextureFromSurface(renderer, surfaceB);
    SDL_FreeSurface(surfaceA);
    SDL_FreeSurface(surfaceB);
    if (!textureA || !textureB) {
        std::cerr << "Unable to create texture! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_DestroyTexture(bgTexture);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Define rectangles for the falling images
    std::vector<SDL_Rect> rectAs(NUM_RECT_A);
    for (int i = 0;i < 4; ++i) {
        for (int j = 0;j <3; ++j){
            for (int j = 0;j <2; ++j){
                for (int j = 0;j <1; ++j){
        rectAs[j] = { rand() % (SCREEN_WIDTH - 400), rand() % (SCREEN_HEIGHT), 50, 100 };
    }
        rectAs[j] = { rand() % (SCREEN_WIDTH - 500), rand() % (SCREEN_HEIGHT), 50, 100 };
    }
        rectAs[j] = { rand() % (SCREEN_WIDTH - 580), rand() % (SCREEN_HEIGHT), 50, 100 };
    }
        rectAs[i] = { rand() % (SCREEN_WIDTH - 7000), rand() % (SCREEN_HEIGHT), 50, 100 };
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
                        rectB.x -= 20;
                        break;
                    case SDLK_RIGHT:
                        rectB.x += 20;
                        break;
                }
            }
        }

        // Move falling rectangles
        for (auto& rectA : rectAs) {
            rectA.y += RECT_A_SPEED;
            if (rectA.y > SCREEN_HEIGHT) {
                rectA.y = 0;
                rectA.x = rand() % (SCREEN_WIDTH - rectA.w);
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

        // Render controlled rectangle
        SDL_RenderCopy(renderer, textureB, NULL, &rectB);

        // Check for collision
        for (const auto& rectA : rectAs) {
            if (checkCollision(rectA, rectB)) {
                std::cout << "Collision detected!" << std::endl;
                //quit=true;
            }
        }

        // Update screen
        SDL_RenderPresent(renderer);

        // Add a small delay to control the frame rate
        SDL_Delay(16); // Approximately 60 FPS
    }

    // Free resources and close SDL
    SDL_DestroyTexture(textureA);
    SDL_DestroyTexture(textureB);
    SDL_DestroyTexture(bgTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
