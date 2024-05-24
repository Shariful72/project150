#include <SDL2/SDL.h>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <string>
#include <sstream>
#include <SDL2/SDL_stbfont.h> // Include SDL_stbfont

// Screen dimensions
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

// Number of falling rectangles
const int NUM_RECT_A = 5;

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

// Function to render text on the screen using SDL_stbfont
void renderText(SDL_Renderer* renderer, STB_Font* font, const std::string& text, int x, int y, SDL_Color color) {
    SDL_Color oldColor;
    SDL_GetRenderDrawColor(renderer, &oldColor.r, &oldColor.g, &oldColor.b, &oldColor.a);
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    STB_Font_Print(font, x, y, text.c_str());
    SDL_SetRenderDrawColor(renderer, oldColor.r, oldColor.g, oldColor.b, oldColor.a);
}

enum MenuOption {
    MENU_OPTION_NEW_GAME,
    MENU_OPTION_INSTRUCTIONS,
    MENU_OPTION_QUIT,
    MENU_OPTION_NONE
};

MenuOption showMenu(SDL_Renderer* renderer, STB_Font* font) {
    bool menuRunning = true;
    SDL_Event e;

    SDL_Surface* menuSurface = SDL_LoadBMP("./assests/Resume.bmp");
    SDL_Texture* menuTexture = SDL_CreateTextureFromSurface(renderer, menuSurface);
    SDL_FreeSurface(menuSurface);

    while (menuRunning) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                return MENU_OPTION_QUIT;
            } else if (e.type == SDL_MOUSEBUTTONDOWN) {
                int x = e.button.x;
                int y = e.button.y;

                if (x >= 300 && x <= 500) {
                    if (y >= 200 && y <= 250) {
                        return MENU_OPTION_NEW_GAME;
                    } else if (y >= 300 && y <= 350) {
                        return MENU_OPTION_INSTRUCTIONS;
                    } else if (y >= 400 && y <= 450) {
                        return MENU_OPTION_QUIT;
                    }
                }
            }
        }

        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, menuTexture, NULL, NULL);
        SDL_RenderPresent(renderer);
    }

    return MENU_OPTION_NONE;
}

void showInstructions(SDL_Renderer* renderer) {
    SDL_Surface* instructionSurface = SDL_LoadBMP("./assests/Easy.bmp");
    SDL_Texture* instructionTexture = SDL_CreateTextureFromSurface(renderer, instructionSurface);
    SDL_FreeSurface(instructionSurface);

    bool instructionsRunning = true;
    SDL_Event e;
    while (instructionsRunning) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT || e.type == SDL_MOUSEBUTTONDOWN || e.type == SDL_KEYDOWN) {
                instructionsRunning = false;
            }
        }

        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, instructionTexture, NULL, NULL);
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(instructionTexture);
}

std::string to_string(int value) {
    std::ostringstream os;
    os << value;
    return os.str();
}

void runGame(SDL_Renderer* renderer, STB_Font* font) {
    // Load background image
    SDL_Surface* bgSurface = SDL_LoadBMP("./assests/bg.bmp");
    SDL_Texture* bgTexture = SDL_CreateTextureFromSurface(renderer, bgSurface);
    SDL_FreeSurface(bgSurface);

    // Load images for moving objects
    SDL_Surface* surfaceA = SDL_LoadBMP("./assests/down.bmp");
    SDL_Surface* surfaceB = SDL_LoadBMP("./assests/down.bmp");
    SDL_Surface* surfaceC = SDL_LoadBMP("./assests/down.bmp");
    SDL_Surface* surfaceD = SDL_LoadBMP("./assests/down.bmp");
    SDL_Surface* surfaceE = SDL_LoadBMP("./assests/down.bmp");

    SDL_Texture* textureA = SDL_CreateTextureFromSurface(renderer, surfaceA);
    SDL_Texture* textureB = SDL_CreateTextureFromSurface(renderer, surfaceB);
    SDL_Texture* textureC = SDL_CreateTextureFromSurface(renderer, surfaceC);
    SDL_Texture* textureD = SDL_CreateTextureFromSurface(renderer, surfaceD);
    SDL_Texture* textureE = SDL_CreateTextureFromSurface(renderer, surfaceE);

    SDL_FreeSurface(surfaceA);
    SDL_FreeSurface(surfaceB);
    SDL_FreeSurface(surfaceC);
    SDL_FreeSurface(surfaceD);
    SDL_FreeSurface(surfaceE);

    // Define rectangles for the falling images
    std::vector<SDL_Rect> rectAs(NUM_RECT_A);
    for (int i = 0; i < NUM_RECT_A; ++i) {
        rectAs[i] = { rand() % (SCREEN_WIDTH - 100), rand() % (SCREEN_HEIGHT / 2), 50, 100 };
    }

    // Define rectangles for other falling images
    std::vector<SDL_Rect> rectCs(NUM_RECT_A), rectDs(NUM_RECT_A), rectEs(NUM_RECT_A);
    for (int i = 0; i < NUM_RECT_A; ++i) {
        rectCs[i] = { rand() % (SCREEN_WIDTH - 100), rand() % (SCREEN_HEIGHT / 2), 50, 100 };
        rectDs[i] = { rand() % (SCREEN_WIDTH - 100), rand() % (SCREEN_HEIGHT / 2), 50, 100 };
        rectEs[i] = { rand() % (SCREEN_WIDTH - 100), rand() % (SCREEN_HEIGHT / 2), 50, 100 };
    }

    // Define rectangle for the controlled image
    SDL_Rect rectB = { 350, 500, 50, 100 };

    // Main loop flag
    bool quit = false;
    SDL_Event e;

    Uint32 startTime = SDL_GetTicks();
    Uint32 score = 0;

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
        auto moveRects = [](std::vector<SDL_Rect>& rects) {
            for (auto& rect : rects) {
                rect.y += RECT_SPEED;
                if (rect.y > SCREEN_HEIGHT) {
                    rect.y = 0;
                    rect.x = rand() % (SCREEN_WIDTH - rect.w);
                }
            }
        };

        moveRects(rectAs);
        moveRects(rectCs);
        moveRects(rectDs);
        moveRects(rectEs);

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
        auto checkCollisionWithRects = [&rectB](const std::vector<SDL_Rect>& rects) -> bool {
            for (const auto& rect : rects) {
                if (checkCollision(rect, rectB)) {
                    return true;
                }
            }
            return false;
        };

        if (checkCollisionWithRects(rectAs) || checkCollisionWithRects(rectCs) || checkCollisionWithRects(rectDs) || checkCollisionWithRects(rectEs)) {
            quit = true;
            std::cout << "Collision detected!" << std::endl;
        }

        // Calculate score
        score = (SDL_GetTicks() - startTime) / 1000;

        // Render score
        SDL_Color textColor = { 255, 255, 255, 255 };
        renderText(renderer, font, "Score: " + to_string(score), 650, 10, textColor);

        // Update screen
        SDL_RenderPresent(renderer);

        // Add a small delay to control the frame rate
        SDL_Delay(16); // Approximately 60 FPS
    }

    // Game over message
    SDL_RenderClear(renderer);
    renderText(renderer, font, "GAME OVER", SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 - 50, { 255, 0, 0, 255 });
    renderText(renderer, font, "Score: " + to_string(score), SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2, { 255, 0, 0, 255 });
    SDL_RenderPresent(renderer);
    SDL_Delay(3000);

    // Free resources
    SDL_DestroyTexture(textureA);
    SDL_DestroyTexture(textureB);
    SDL_DestroyTexture(textureC);
    SDL_DestroyTexture(textureD);
    SDL_DestroyTexture(textureE);
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

    // Load font
    STB_Font* font = STB_Font_Load(renderer, "./assests/Herculanum (1).ttf", 24);
    if (!font) {
        std::cerr << "Failed to load font!" << std::endl;
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    bool running = true;
    while (running) {
        MenuOption selectedOption = showMenu(renderer, font);
        switch (selectedOption) {
            case MENU_OPTION_NEW_GAME:
                runGame(renderer, font);
                break;
            case MENU_OPTION_INSTRUCTIONS:
                showInstructions(renderer);
                break;
            case MENU_OPTION_QUIT:
                running = false;
                break;
            default:
                break;
        }
    }

    // Cleanup
    STB_Font_Destroy(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
