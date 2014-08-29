//
//  Laser.cpp
//  battleSnake
//
//  Created by Enrico Migliorini on 16/04/14.
//  Copyright (c) 2014 Enrico Migliorini. All rights reserved.
//

#include "Laser.h"

Laser::Laser(int dmg, int x, int y, double ang, bool sBA, OtherSprites part) {
    power = dmg;
    gX = x;
    gY = y;
    width = LASERWIDTH;
    height = LASERHEIGHT;
    angle = ang;    // The angle is calculated from the vertical. Check the rotation center. Also, it is in RADIANS
    sprite = part;  // Having to give the sprites mean that the same ship can shoot different lasers
    if (sBA) {
        speed = 20;
    }
    else{
        speed = 9;
    }
    turnsInLife = 0;
    shotByAlly = sBA;
    destroyCounter = 3;
    exploding = false;
    markedForDeletion = false;
}

bool Laser::isHittingWall(Level* currLevel) {
    bool result = false;
    if (gX <= SPRITE_WIDTH/TILE_WIDTH || gY <= SPRITE_HEIGHT/TILE_HEIGHT || gX >= SCREEN_WIDTH-SPRITE_WIDTH/TILE_WIDTH || gY >= SCREEN_HEIGHT-SPRITE_HEIGHT/TILE_HEIGHT ) {
        result = true;
    }
    return result;
}

void Laser::drawOnScreen(Graphics* graph) {  // Float values allow me to move at the speed I need
    SDL_Rect dst = {static_cast<int>(gX), static_cast<int>(gY), height, width};
    // I think that some pain to me came from having put height before width. Damn.
    SDL_Point corner = {0, 0};
    SDL_RenderCopyEx(graph->getRenderer(), graph->getOtherSprite(sprite), 0, &dst, -(angle/pi*180), &corner, SDL_FLIP_NONE);
    // Angle reversed because i'm using reverse coords
}

void Laser::travel() {
    gX += (speed * cos(angle));
    gY -= (speed * sin(angle));
    turnsInLife++;
    // since travel is called each turn, it if good for making lasers explode without creating other functions, setters and getters.
    if (exploding) {
        explode();
    }
}

void Laser::explode() {
    // This makes the laser become cosmetic only
    if (destroyCounter == 3) {
        exploding = true;
        sprite = LASER_EXPLODE;
        speed = 0;
        width = 15;
        height = 15;
        power = 0;
    }
    destroyCounter--;
    if (destroyCounter == 0) {
        markedForDeletion = true;
    }
}

void Laser::setAngle(double targetAngle) {
    angle = targetAngle;
}

void Laser::setSpeed(int targetSpeed) {
    speed = targetSpeed;
}

int Laser::getTurnsInLife() {
    return turnsInLife;
}

int Laser::getSpeed() {
    return speed;
}

double Laser::getAngle() {
    return angle;
}

float Laser::getgX() {
    return gX;
}

float Laser::getgY() {
    return gY;
}

float Laser::getCentergX() {
    float retX = (gX + ((width/2)*cos(angle)) + ((height/2)*sin(angle)));
    if (retX < 1) {
        retX = 1;
    }
    if (retX > 199) {
        retX = 199;
    }
    return retX;
}

float Laser::getCentergY() {
    float retY = (gY - ((width/2)*sin(angle)) + ((height/2)*cos(angle)));
    if (retY < 1) {
        retY = 1;
    }
    if (retY > 149) {
        retY = 149;
    }
    return retY;
}

int Laser::getPower() {
    return power;
}

bool Laser::isSBA() {
    return shotByAlly;
}

bool Laser::isMFD() {
    return markedForDeletion;
}