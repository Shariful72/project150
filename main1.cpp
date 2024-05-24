#include <SDL2/SDL.h>
#include <SDL2/SDL_main.h>
#include <iostream>
#include <string>


const int WINDOW_WIDTH = 750;
const int WINDOW_HEIGHT = 700;
const float ROAD_SPEED = 0.01f; // Slower speed in pixels per frame




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

// Car class
class Car {
public:
    Car(SDL_Renderer* renderer, const std::string& filePath, int x, int y);
    ~Car();

    void moveLeft();
    void moveRight();
    void moveUp();
    void moveDown();
    void update();
    void render();

private:
    SDL_Rect carRect;
    SDL_Renderer* renderer;
    SDL_Texture* texture;
    int speed;

    SDL_Texture* loadTexture(const std::string& filePath);
};

Car::Car(SDL_Renderer* renderer, const std::string& filePath, int x, int y)
    : renderer(renderer), speed(70) {
    texture = loadTexture(filePath);
    if (texture == NULL) {
        std::cerr << "Failed to load car texture: " << SDL_GetError() << std::endl;
    }
    SDL_QueryTexture(texture, NULL, NULL, &carRect.w, &carRect.h);
    carRect.x = x;
    carRect.y = y;
}

Car::~Car() {
    SDL_DestroyTexture(texture);
}

void Car::moveLeft() {
    if (carRect.x > 140) {
        carRect.x -= speed;
    }
}

void Car::moveRight() {
    if (carRect.x < 560) {
        carRect.x += speed;
    }
}

void Car::moveDown() {
    carRect.y += speed;
}

void Car::moveUp() {
    carRect.y -= speed;
}

void Car::update() {
    if (carRect.x < 0) carRect.x = 0;
    if (carRect.x > WINDOW_WIDTH - carRect.w) carRect.x = WINDOW_WIDTH - carRect.w;
    if (carRect.y < 0) carRect.y = 0;
    if (carRect.y > WINDOW_HEIGHT - carRect.h) carRect.y = WINDOW_HEIGHT - carRect.h;
}

void Car::render() {
    SDL_RenderCopy(renderer, texture, NULL, &carRect);
}

SDL_Texture* Car::loadTexture(const std::string& filePath) {
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



int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return -1;
    }

    SDL_Window* window = SDL_CreateWindow("Car Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == NULL) {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return -1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
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

    SDL_Rect roadRect1 = { 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };
    SDL_Rect roadRect2 = { 0, -WINDOW_HEIGHT, WINDOW_WIDTH, WINDOW_HEIGHT };

    Car car(renderer, "./assests/car80.bmp", WINDOW_WIDTH / 2, WINDOW_HEIGHT - 50);

    Menu menu(renderer);

    float accumulatedMovement = 0.0f;

    bool running = true;
    bool gameStarted = false;
    bool showInstructions = false;
    SDL_Event event;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
    
            
            if (!gameStarted) {
                menu.handleEvent(event, running, gameStarted, showInstructions);
            }
            if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                    case SDLK_LEFT:
                        car.moveLeft();
                        break;
                    case SDLK_RIGHT:
                        car.moveRight();
                        break;
                    case SDLK_UP:
                        car.moveUp();
                        break;
                    case SDLK_DOWN:
                        car.moveDown();
                        break;
        
        
            }
        }
    }


        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        if (gameStarted) {
            car.update();

            accumulatedMovement += ROAD_SPEED;
            if (accumulatedMovement >= 1.0f) {
                int pixelsToMove = static_cast<int>(accumulatedMovement);
                roadRect1.y += pixelsToMove;
                roadRect2.y += pixelsToMove;
                accumulatedMovement -= pixelsToMove;

                if (roadRect1.y >= WINDOW_HEIGHT) {
                    roadRect1.y = roadRect2.y - WINDOW_HEIGHT;
                }
                if (roadRect2.y >= WINDOW_HEIGHT) {
                    roadRect2.y = roadRect1.y - WINDOW_HEIGHT;
                }
            }

            SDL_RenderCopy(renderer, roadTexture, NULL, &roadRect1);
            SDL_RenderCopy(renderer, roadTexture, NULL, &roadRect2);

            car.render();
        } else if (showInstructions) {
            menu.showInstructions();
            showInstructions = false;
        } else {
            menu.render();
        }

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(roadTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
