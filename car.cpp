//
//  Car.cpp
//  Game
//
//  Created by Pepe Becker on 3/8/15.
//  Copyright (c) 2015 Pepe Apps. All rights reserved.
////
//  Car.h
//  Game
//
//  Created by Pepe Becker on 3/8/15.
//  Copyright (c) 2015 Pepe Apps. All rights reserved.
//

#ifndef __Game__Car__
#define __Game__Car__

#include <SDL2/SDL.h>
#include <SDL2_mixer/SDL_mixer.h>

enum CarColor {
    CarColorRed,
    CarColorBlue,
    CarColorGreen,
    CarColorBrown,
    CarColorBlack,
};

class Car {
public:
    SDL_Texture *texture;
    SDL_Rect rect;
    Car(int x, int y, SDL_Renderer *renderer, CarColor color);
    ~Car();
    void Draw(SDL_Renderer *renderer);
    void SetSpeed(float speed);
    void SetDirection(int dir);
    void SwitchLane();
    void Update();
private:
    int rotation = 0;
    int direction = 1;
    int speed = 0;
    int lane = 1;
    int defaultLanePos = 0;
    bool switchingLane = false;
};

#endif /* defined(__Game__Car__) */

Car::Car(int x, int y, SDL_Renderer *renderer, CarColor color) {
    this->rect.x = x;
    this->rect.y = y;
    this->rect.w = 64;
    this->rect.h = 64;
    this->defaultLanePos = x;
    
    const char *path = NULL;
    
    if (color == CarColorBlack)
        path = "./assests/Car-Red_1.bmp";
    if (color == CarColorBlue)
        path = "./assests/Car-Blue_1.bmp";
    if (color == CarColorRed)
        path = "./assests/Car-Brown_1.bmp";
    if (color == CarColorGreen)
        path = "./assests/Car-Green_1.bmp";
    if (color == CarColorBrown)
        path = "./assests/Car-Black_1.bmp";
    
    SDL_Surface *surface = IMG_Load(path);
    this->texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
}

void Car::Draw(SDL_Renderer *renderer) {
    SDL_RenderCopyEx(renderer, this->texture, NULL, &this->rect, rotation, NULL, SDL_FLIP_NONE);
}

void Car::SetDirection(int dir) {
    if (dir > 0) {
        direction = 1;
        rotation = 0;
    }
    if (dir < 0) {
        direction = -1;
        rotation = 180;
    }
}

void Car::SetSpeed(float speed) {
    this->speed = speed;
}

void Car::SwitchLane() {
    if (lane == 0) {
        lane = 1;
    } else {
        lane = 0;
    }
    switchingLane = true;
}

void Car::Update() {
    this->rect.y -= speed * direction;
    
    if (switchingLane) {
        if (lane == 1 && this->rect.x < defaultLanePos) {
            this->rect.x += 2;
            rotation = 10;
        }
        else if (lane == 0 && this->rect.x > defaultLanePos - 64) {
            this->rect.x -= 2;
            rotation = -10;
        }
        else {
            rotation = 0;
            switchingLane = false;
        }
    }
}

Car::~Car() {
    SDL_DestroyTexture(this->texture);
}