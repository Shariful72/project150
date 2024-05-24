#include <SDL2/SDL.h>
#include <SDL2/SDL_main.h>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <SDL2/SDL_rect.h>

const int WINDOW_WIDTH = 850;
const int WINDOW_HEIGHT = 700;
const float ROAD_SPEED = 0.08f; // Slower speed in pixels per frame

bool collision(const SDL_Rect& car, const SDL_Rect& enemyCar) {
    if (car.y + car.h <= enemyCar.y)
        return false;
    if (car.y >= enemyCar.y + enemyCar.h)
        return false;
    if (car.x + car.w <= enemyCar.x)
        return false;
    if (car.x >= enemyCar.x + enemyCar.w)
        return false;
    
    return true;
}

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
    const SDL_Rect& getRect() const { return carRect; }

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
    if (carRect.x > 210) {
        carRect.x -= speed;
    }
}

void Car::moveRight() {
    if (carRect.x < 630) {
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

class EnemyCar {
public:
    EnemyCar(SDL_Renderer* renderer, const std::string& filePath, int x, int y);
    ~EnemyCar();

    void update();
    void render();
    bool isOffScreen() const;
    void reset();
    const SDL_Rect& getRect() const { return carRect; }

private:
    SDL_Rect carRect;
    SDL_Renderer* renderer;
    SDL_Texture* texture;
    float speed;
    float accumulatedMovement;
    SDL_Texture* loadTexture(const std::string& filePath);
};

EnemyCar::EnemyCar(SDL_Renderer* renderer, const std::string& filePath, int x, int y)
    : renderer(renderer), speed(0.05f), accumulatedMovement(0.0f) {
    texture = loadTexture(filePath);
    if (texture == NULL) {
        std::cerr << "Failed to load enemy car texture: " << SDL_GetError() << std::endl;
    }
    SDL_QueryTexture(texture, NULL, NULL, &carRect.w, &carRect.h);
    carRect.x = x;
    carRect.y = y;
}

EnemyCar::~EnemyCar() {
    SDL_DestroyTexture(texture);
}

void EnemyCar::update() {
    accumulatedMovement += speed;
    if (accumulatedMovement >= 1.0f) {
        int pixelsToMove = static_cast<int>(accumulatedMovement);
        carRect.y += pixelsToMove;
        accumulatedMovement -= pixelsToMove;

        if (carRect.y > WINDOW_HEIGHT) {
            reset();
        }
    }
}

void EnemyCar::render() {
    SDL_RenderCopy(renderer, texture, NULL, &carRect);
}

bool EnemyCar::isOffScreen() const {
    return carRect.y > WINDOW_HEIGHT;
}

void EnemyCar::reset() {
    carRect.y = -carRect.h;
    carRect.x = (std::rand() % (800 - carRect.w));
}

SDL_Texture* EnemyCar::loadTexture(const std::string& filePath) {
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

    Car car(renderer, "./assests/Car-Blue_1.bmp", WINDOW_WIDTH / 2, WINDOW_HEIGHT - 150);

    std::vector<EnemyCar> enemyCars;
    std::srand(static_cast<unsigned>(std::time(0)));
    for (int i = 0; i < 2; ++i) {
        int x = (std::rand() % (900));
        enemyCars.emplace_back(renderer, "./assests/Car-Blue_1.bmp", x, -(std::rand() % WINDOW_HEIGHT));
        int y = (std::rand() % (500));
        enemyCars.emplace_back(renderer, "./assests/Car-Blue_1.bmp", y, -(std::rand() % WINDOW_HEIGHT));
        int z = (std::rand() % (300));
        enemyCars.emplace_back(renderer, "./assests/Car-Blue_1.bmp", z, -(std::rand() % WINDOW_HEIGHT));
        int a = (std::rand() % (400));
        enemyCars.emplace_back(renderer, "./assests/Car-Blue_1.bmp", a, -(std::rand() % WINDOW_HEIGHT));
    }

    float accumulatedMovement = 0.0f;

    bool running = true;
    bool gameover = false;
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

        if (!gameover) {
            car.update();
            for (auto& enemyCar : enemyCars) {
                enemyCar.update();
                if (collision(car.getRect(), enemyCar.getRect())) {
                    gameover = true;
                }
            }

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
            for (auto& enemyCar : enemyCars) {
                enemyCar.render();
            }
            SDL_RenderPresent(renderer);
        } else {
            // Render Game Over screen or logic
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Red background for Game Over
            SDL_RenderClear(renderer);
            SDL_RenderPresent(renderer);
        }
    }

    SDL_DestroyTexture(roadTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
