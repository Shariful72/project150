#include <SDL2/SDL.h>
#include <SDL2/SDL_main.h>
#include <iostream>

const int WINDOW_WIDTH = 750;
const int WINDOW_HEIGHT = 700;
const float ROAD_SPEED = 0.1f; // Slower speed in pixels per frame

// Car class Defines a Car class with 
// public methods for constructing, moving, updating, and rendering the car
class Car {
public:
    Car(SDL_Renderer* renderer, const std::string& filePath, int x, int y);
    ~Car();

    void moveLeft(); // Move the car to the left
    void moveRight(); // Move the car to the right
    void moveUp(); // Move the car up
    void moveDown(); // Move the car down
    void update(); // Update the car's position
    void render(); // Render the car
    
private:
    SDL_Rect carRect; // Rectangle representing the car's position and size
    SDL_Renderer* renderer;
    SDL_Texture* texture; // Texture representing the car's appearance
    int speed; // Speed of the car

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

    SDL_Rect roadRect1 = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};
    SDL_Rect roadRect2 = {0, -WINDOW_HEIGHT, WINDOW_WIDTH, WINDOW_HEIGHT};

    Car car(renderer, "./assests/car80.bmp", WINDOW_WIDTH / 2, WINDOW_HEIGHT - 50);

    float accumulatedMovement = 0.0f; // Accumulator for fractional movement

    bool running = true;
    SDL_Event event;
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
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

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        SDL_RenderCopy(renderer, roadTexture, NULL, &roadRect1);
        SDL_RenderCopy(renderer, roadTexture, NULL, &roadRect2);

        car.render();
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(roadTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
