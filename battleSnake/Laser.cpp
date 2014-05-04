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

bool Laser::isHittingEnemy(Level currLevel) {
    bool result = false;
    // This way it works. Must be black magic.
    int tmpX = gX/SPRITE_WIDTH;
    int tmpY = gY/SPRITE_HEIGHT;
    if (currLevel.getTileCode((tmpX+1), (tmpY+1)) == ENEMY_HERE) {
        result = true;
    }
    return result;
}

bool Laser::isHittingWall(Level currLevel) {
    bool result = false;
    // Check if this works or places it in a weird place
    int tmpX = gX/SPRITE_WIDTH;
    int tmpY = gY/SPRITE_HEIGHT;
    if (currLevel.getTileCode(tmpX+1, tmpY+1) == COLLISION) {
        result = true;
    }
    return result;
}

void Laser::drawOnScreen(Graphics graph) {
    SDL_Rect dst = {gX, gY, LASERHEIGHT, LASERWIDTH};
    // Check formulas for finding the rendering area
    SDL_RenderCopyEx(graph.getRenderer(), graph.getOtherSprite(sprite), 0, &dst, -(angle/pi*180), 0, SDL_FLIP_NONE);
    // Angle reversed because i'm using reverse coords
}

void Laser::travel() {
    gX += 20 * cos(angle);
    gY -= 20 * sin(angle);
}

double Laser::getAngle() {
    return angle;
}
