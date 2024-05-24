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

// Speed of falling rectangles
const float RECT_A_SPEED = 0.05f;

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
    SDL_Surface* bgSurface = SDL_LoadBMP("./assets/bg.bmp");
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
    SDL_Surface* surfaceA = SDL_LoadBMP("imageA.bmp");
    SDL_Surface* surfaceB = SDL_LoadBMP("imageB.bmp");
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
    for (int i = 0; i < NUM_RECT_A; ++i) {
        rectAs[i] = { rand() % (SCREEN_WIDTH - 100), 0, 100, 100 };
    }

    // Define rectangle for the controlled image
    SDL_Rect rectB = { 200, 150, 100, 100 };

    // Main loop flag
    bool quit = false;
    SDL_Event e;
    Uint32 startTime = SDL_GetTicks();

    // While application is running
    while (!quit) {
        // Handle events on queue
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            } else if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym) {
                    case SDLK_UP:
                        rectB.y -= 5;
                        break;
                    case SDLK_DOWN:
                        rectB.y += 5;
                        break;
                    case SDLK_LEFT:
                        rectB.x -= 5;
                        break;
                    case SDLK_RIGHT:
                        rectB.x += 5;
                        break;
                }
            }
        }

        // Calculate elapsed time
        Uint32 elapsedTime = SDL_GetTicks() - startTime;

        // Move falling rectangles
        for (auto& rectA : rectAs) {
            rectA.y += static_cast<int>(RECT_A_SPEED * elapsedTime);
            if (rectA.y > SCREEN_HEIGHT) {
                rectA.y = 0;
                rectA.x = rand() % (SCREEN_WIDTH - rectA.w);
            }
        }

        // Reset start time for the next frame
        startTime = SDL_GetTicks();

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
            }
        }

        // Update screen
        SDL_RenderPresent(renderer);
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
