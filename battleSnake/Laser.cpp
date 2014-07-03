//
//  Laser.cpp
//  battleSnake
//
//  Created by Enrico Migliorini on 16/04/14.
//  Copyright (c) 2014 Enrico Migliorini. All rights reserved.
//

#include "Laser.h"

Laser::Laser(int dmg, int x, int y, double ang, OtherSprites part) {
    power = dmg;
    gX = x;
    gY = y;
    angle = ang;    // The angle is calculated from the vertical. Check the rotation center
    sprite = part;
}

bool Laser::isHittingEnemy(Level currLevel, Spaceship* &target) {
    bool result = false;
    // This way it works. Must be black magic.
    int tmpX = gX/TILE_WIDTH;
    int tmpY = gY/TILE_HEIGHT;
    if (currLevel.getTile((tmpX+1), (tmpY+1)).occupiedByEnemy == true) {
        result = true;
    }
    target = currLevel.getTile((tmpX+1), (tmpY+1)).occupyingEnemy;
    return result;
}

bool Laser::isHittingWall(Level currLevel) {
    bool result = false;
    // Check if this works or places it in a weird place
    int tmpX = gX/TILE_WIDTH;
    int tmpY = gY/TILE_HEIGHT;
    if (currLevel.getTile((tmpX+1), (tmpY+1)).partOfWall == true) {
        result = true;
    }
    return result;
}

void Laser::drawOnScreen(Graphics &graph) {  // Float values allow me to move at the speed I need
    SDL_Rect dst = {static_cast<int>(gX), static_cast<int>(gY), LASERHEIGHT, LASERWIDTH};
    // Check formulas for finding the rendering area
    SDL_RenderCopyEx(graph.getRenderer(), graph.getOtherSprite(sprite), 0, &dst, -(angle/pi*180), 0, SDL_FLIP_NONE);
    // Angle reversed because i'm using reverse coords
}

void Laser::travel() {
    gX += (2 * cos(angle));
    gY -= (2 * sin(angle));
}

double Laser::getAngle() {
    return angle;
}

int Laser::getX() {
    return gX/TILE_WIDTH;
}

int Laser::getY() {
    return gY/TILE_HEIGHT;
}

int Laser::getPower() {
    return power;
}
