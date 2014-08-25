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
}

bool Laser::isHittingTarget(Level* currLevel, Spaceship* &target) {
    // This is the EVIL
    // MAke it work differently whether or not the laser is shotByAlly
    int tmpX = gX/TILE_WIDTH;
    int tmpY = gY/TILE_HEIGHT;
    Tile* ref = currLevel->getTile((tmpX), (tmpY));
    if (ref == nullptr) {
        cout << "Trouble in iHT at " << tmpX << ", " << tmpY << endl;
        return false;
    }
    if (ref->occupiedByEnemy == true && shotByAlly == true) {
        target = ref->occupyingEnemy;
        return true;
    }
    else if (ref->occupiedByAlly == true && shotByAlly == false) {
        target = ref->occupyingAlly;
        return true;
    }
    target = nullptr;
    return false;
}

bool Laser::isHittingWall(Level* currLevel) {
    bool result = false;
    if (gX <= SPRITE_WIDTH/TILE_WIDTH || gY <= SPRITE_HEIGHT/TILE_HEIGHT || gX >= SCREEN_WIDTH-SPRITE_WIDTH/TILE_WIDTH || gY >= SCREEN_HEIGHT-SPRITE_HEIGHT/TILE_HEIGHT) {
        result = true;
    }
    return result;
}

void Laser::drawOnScreen(Graphics* graph) {  // Float values allow me to move at the speed I need
    SDL_Rect dst = {static_cast<int>(gX), static_cast<int>(gY), LASERHEIGHT, LASERWIDTH};
    // Check formulas for finding the rendering area
    SDL_Point corner = {0, 0};
    SDL_RenderCopyEx(graph->getRenderer(), graph->getOtherSprite(sprite), 0, &dst, -(angle/pi*180), &corner, SDL_FLIP_NONE);
    // Angle reversed because i'm using reverse coords
}

void Laser::travel() {
    gX += (speed * cos(angle));
    gY -= (speed * sin(angle));
    turnsInLife++;
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

int Laser::getPower() {
    return power;
}

bool Laser::isSBA() {
    return shotByAlly;
}
