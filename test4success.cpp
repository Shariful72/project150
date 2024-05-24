#include <SDL2/SDL.h>
#include <SDL2/SDL_main.h>
#include <iostream>

// Screen dimensions
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const float ROAD_SPEED = 0.06f;
const float CAR_SPEED = 0.1f;

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
    SDL_Rect rectA = { 500, 100, 90, 90 };
    SDL_Rect rectB = { 200, 150, 100, 100 };

//FOR CAR
        //SDL_Rect rectA1 = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
        //SDL_Rect rectA2 = {0, -SCREEN_HEIGHT, SCREEN_WIDTH, SCREEN_HEIGHT};

    SDL_Rect roadRect1 = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
    SDL_Rect roadRect2 = {0, -SCREEN_HEIGHT, SCREEN_WIDTH, SCREEN_HEIGHT};

    SDL_Rect rectA1 = {700, 0, 90,90};
    SDL_Rect rectA2 = {500, -SCREEN_HEIGHT,90 ,90};
    SDL_Rect rectA3 = {200, -SCREEN_HEIGHT, 90,90};
    SDL_Rect rectA4 = {100, 0, 90,90};
    SDL_Rect rectA5 = {500, -SCREEN_HEIGHT,90 ,90};
    SDL_Rect rectA6 = {500, 0,90 ,90};
    

float accumulatedMovement = 0.05f;
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


         accumulatedMovement += ROAD_SPEED;
        if (accumulatedMovement >= 1.0f) {
            int pixelsToMove = static_cast<int>(accumulatedMovement);
            roadRect1.y += pixelsToMove;
            roadRect2.y += pixelsToMove;
            accumulatedMovement -= pixelsToMove;

            if (roadRect1.y >= SCREEN_HEIGHT) {
                roadRect1.y = roadRect2.y - SCREEN_HEIGHT;
            }
            if (roadRect2.y >= SCREEN_HEIGHT) {
                roadRect2.y = roadRect1.y - SCREEN_HEIGHT;
            }
        }

                 accumulatedMovement += CAR_SPEED;
        if (accumulatedMovement >= 1.0f) {
            int pixelsToMove = static_cast<int>(accumulatedMovement);
            rectA1.y += pixelsToMove;
            rectA2.y += pixelsToMove;
            rectA3.y += pixelsToMove;
            rectA4.y += pixelsToMove;
            rectA5.y += pixelsToMove;
            rectA6.y += pixelsToMove;
            accumulatedMovement -= pixelsToMove;

            if (rectA1.y >= SCREEN_HEIGHT) {
                rectA1.y = rectA2.y - SCREEN_HEIGHT;
            }
            if (rectA2.y >= SCREEN_HEIGHT) {
                rectA2.y = rectA1.y - SCREEN_HEIGHT;
            }
            if (rectA3.y >= SCREEN_HEIGHT) {
                rectA3.y = rectA4.y - 500;
            }
            if (rectA4.y >= SCREEN_HEIGHT) {
                rectA4.y = rectA3.y -500;
            }
             if (rectA5.y >= SCREEN_HEIGHT) {
                rectA5.y = rectA6.y - 200;
            }
            if (rectA6.y >= SCREEN_HEIGHT) {
                rectA6.y = rectA5.y -300;
            }
        }

        // Clear screen
        SDL_SetRenderDrawColor(renderer, 0, 0,0,255);
        SDL_RenderClear(renderer);

//for road
        SDL_RenderCopy(renderer, roadTexture, NULL, &roadRect1);
        SDL_RenderCopy(renderer, roadTexture, NULL, &roadRect2);
        
        SDL_RenderCopy(renderer, textureA, NULL, &rectA1);
        SDL_RenderCopy(renderer, textureA, NULL, &rectA2);
        SDL_RenderCopy(renderer, textureA, NULL, &rectA3);
        SDL_RenderCopy(renderer, textureA, NULL, &rectA4);
        SDL_RenderCopy(renderer, textureA, NULL, &rectA5);
        SDL_RenderCopy(renderer, textureA, NULL, &rectA6);

        //SDL_RenderCopy(renderer, roadTexture, NULL, NULL);
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
