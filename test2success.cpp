#include <SDL2/SDL.h>
#include <SDL2/SDL_main.h>
#include <iostream>

// Screen dimensions
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const float ROAD_SPEED = 0.1f;

// Function to check if two rectangles are colliding
bool checkCollision(const SDL_Rect& a, const SDL_Rect& b) {
    // The sides of the rectangles
    int leftA = a.x;
    int rightA = a.x + a.w;
    int topA = a.y;
    int bottomA = a.y + a.h;

    int leftB = b.x;
    int rightB = b.x + b.w;
    int topB = b.y;
    int bottomB = b.y + b.h;

    // If any of the sides from A are outside of B
    if (bottomA <= topB) {
        return false;
    }
    if (topA >= bottomB) {
        return false;
    }
    if (rightA <= leftB) {
        return false;
    }
    if (leftA >= rightB) {
        return false;
    }

    // If none of the sides from A are outside B
    return true;
}

int main(int argc, char* args[]) {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    // Create window
    SDL_Window* window = SDL_CreateWindow("SDL Collision Detection",
                                          SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                          SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window==NULL) {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    // Create renderer
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer==NULL) {
        std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }


     SDL_Surface* roadSurface = SDL_LoadBMP("./assests/bg.bmp");
    if (roadSurface == NULL) {
        std::cerr << "Failed to load road surface: " << SDL_GetError() << std::endl;
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    SDL_Texture* roadTexture = SDL_CreateTextureFromSurface(renderer, roadSurface);
    SDL_FreeSurface(roadSurface);
    if (roadTexture == NULL) {
        std::cerr << "Failed to create road texture: " << SDL_GetError() << std::endl;
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    // Load image as surface
    SDL_Surface* surfaceA = SDL_LoadBMP("./assests/down.bmp");
    SDL_Surface* surfaceB = SDL_LoadBMP("./assests/down.bmp");
    if (!surfaceA || !surfaceB) {
        std::cerr << "Unable to load image! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Create texture from surface
    SDL_Texture* textureA = SDL_CreateTextureFromSurface(renderer, surfaceA);
    SDL_Texture* textureB = SDL_CreateTextureFromSurface(renderer, surfaceB);
    SDL_FreeSurface(surfaceA);
    SDL_FreeSurface(surfaceB);
    if (!textureA || !textureB) {
        std::cerr << "Unable to create texture! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_DestroyRenderer(renderer);
        SDL_DestroyTexture(roadTexture);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Define rectangles for the images
    SDL_Rect rectA = { 500, 100, 50, 50 };
    SDL_Rect rectB = { 200, 150, 100, 100 };

    // Main loop flag
    bool quit = false;
    SDL_Event e;

    // While application is running
    while (!quit) {
        // Handle events on queue
        while (SDL_PollEvent(&e) != 0) {
            // User requests quit
            if (e.type == SDL_QUIT) {
                quit = true;
            } else if (e.type == SDL_KEYDOWN) {
                // Adjust the position of rectB based on key press
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

        // Clear screen
        SDL_SetRenderDrawColor(renderer, 0, 0,0,255);
        SDL_RenderClear(renderer);

        SDL_RenderCopy(renderer, roadTexture, NULL, NULL);
        // Render texture to screen
        SDL_RenderCopy(renderer, textureA, NULL, &rectA);
        SDL_RenderCopy(renderer, textureB, NULL, &rectB);

        // Check for collision
        if (checkCollision(rectA, rectB)) {
            std::cout << "Collision detected!" << std::endl;
            quit = true;
        }

        // Update screen
        SDL_RenderPresent(renderer);
    }

    // Free resources and close SDL
    SDL_DestroyTexture(roadTexture);
    SDL_DestroyTexture(textureA);
    SDL_DestroyTexture(textureB);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
