//
//  Laser.h
//  battleSnake
//
//  Created by Enrico Migliorini on 16/04/14.
//  Copyright (c) 2014 Enrico Migliorini. All rights reserved.
//

//==================================================//
//                                                  //
//   Lasers are fired from ships to enemies         //
//                                                  //
//==================================================//


#ifndef __battleSnake__Laser__
#define __battleSnake__Laser__

#include <cmath>

#include "Graphics.h"
#include "Level.h"

const int LASERWIDTH = 5;
const int LASERHEIGHT = 20;

class Laser {
private:
    bool shotByAlly;
    OtherSprites sprite;
    int power;
    float gX, gY;       // Graphics coordinates of the collision center (on top)
    double angle;       // I will build the direction moving according to the sin and cos of the angle
    int turnsInLife;    // To make them turn after a while
    int speed;
public:
    int TEMPlifeCounter = 0;
    Laser(int power, int gX, int gY, double angle, bool sBA, OtherSprites part = LASER_ALLIED);
    bool isHittingTarget(Level* currLevel, Spaceship* &target);
    bool isHittingWall(Level* currLevel);
    bool isHittingPlayerHitbox(Level* currLevel);
    
    void travel();
    void drawOnScreen(Graphics* graph);
    
    int getX();
    int getY();
    int getPower();
    double getAngle();
    int getSpeed();
    int getTurnsInLife();
    bool isSBA();
    void setAngle(double targetAngle);
    void setSpeed(int targetSpeed);
};

#endif /* defined(__battleSnake__Laser__) */
