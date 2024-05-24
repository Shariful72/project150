#include <string>
#include <SDL2/SDL.h>
#include <iostream>



// Menu class
class Menu {
public:
    Menu(SDL_Renderer* renderer);
    ~Menu();

    void render();
    void handleEvent(SDL_Event& e, bool& running, bool& gameStarted, bool& showInstructions);
    void showInstructions();

private:
    SDL_Renderer* renderer;
    SDL_Rect newGameRect;
    SDL_Rect instructionsRect;
    SDL_Rect quitRect;
    SDL_Texture* newGameTexture;
    SDL_Texture* instructionsTexture;
    SDL_Texture* quitTexture;

    SDL_Texture* loadTexture(const std::string& filePath);
};

Menu::Menu(SDL_Renderer* renderer)
    : renderer(renderer) {
    newGameRect = { 275, 200, 200, 50 };
    instructionsRect = { 275, 300, 200, 50 };
    quitRect = { 275, 400, 200, 50 };

    newGameTexture = loadTexture("./assests/NEW_GAME.bmp");
    instructionsTexture = loadTexture("./assests/HOW_TO_PLAY.bmp");
    quitTexture = loadTexture("./assests/QUIT.bmp");
}

Menu::~Menu() {
    SDL_DestroyTexture(newGameTexture);
    SDL_DestroyTexture(instructionsTexture);
    SDL_DestroyTexture(quitTexture);
}

SDL_Texture* Menu::loadTexture(const std::string& filePath) {
    SDL_Surface* surface = SDL_LoadBMP(filePath.c_str());
    if (surface == NULL) {
        std::cerr << "Unable to load image " << filePath << ": " << SDL_GetError() << std::endl;
        return NULL;
    }

    SDL_Texture* newTexture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    if (newTexture == NULL) {
        std::cerr << "Unable to create texture from surface: " << SDL_GetError() << std::endl;
    }
    return newTexture;
}
void Menu::render() {
    SDL_RenderCopy(renderer, newGameTexture, NULL, &newGameRect);
    SDL_RenderCopy(renderer, instructionsTexture, NULL, &instructionsRect);
    SDL_RenderCopy(renderer, quitTexture, NULL, &quitRect);
}

void Menu::handleEvent(SDL_Event& e, bool& running, bool& gameStarted, bool& showInstructions) {
    if (e.type == SDL_MOUSEBUTTONDOWN) {
        int x, y;
        SDL_GetMouseState(&x, &y);

        if (x >= newGameRect.x && x <= newGameRect.x + newGameRect.w && y >= newGameRect.y && y <= newGameRect.y + newGameRect.h) {
            gameStarted = true;
        } else if (x >= instructionsRect.x && x <= instructionsRect.x + instructionsRect.w && y >= instructionsRect.y && y <= instructionsRect.y + instructionsRect.h) {
            showInstructions = true;
        } else if (x >= quitRect.x && x <= quitRect.x + quitRect.w && y >= quitRect.y && y <= quitRect.y + quitRect.h) {
            running = false;
        }
    }
}
void Menu::showInstructions() {
    SDL_Rect instructionsRect = { 50, 50, WINDOW_WIDTH - 100, WINDOW_HEIGHT - 100 };
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &instructionsRect);

    // Here, you can render instructions text or images to the screen
    // For simplicity, we're not including the actual instructions rendering code

    SDL_RenderPresent(renderer);
    SDL_Delay(5000); // Show instructions for 5 seconds
}