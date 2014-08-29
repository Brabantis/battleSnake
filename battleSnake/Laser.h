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

const int LASERWIDTH = 20;
const int LASERHEIGHT = 5;

class Laser {
private:
    bool shotByAlly;
    OtherSprites sprite;
    int power;
    double gX, gY;       // Graphics coordinates of the collision center (on top)
    int width, height;
    double angle;       // I will build the direction moving according to the sin and cos of the angle
    int turnsInLife;    // To make them turn after a while
    int speed;
    int destroyCounter;
    bool exploding;
    bool markedForDeletion;
public:
    Laser(int power, int gX, int gY, double angle, bool sBA, OtherSprites part = LASER_ALLIED);
    bool isHittingWall();
    
    void travel();
    void explode();
    void drawOnScreen(Graphics* graph);
    
    double getgX();
    double getgY();
    double getCentergX();
    double getCentergY();
    int getPower();
    double getAngle();
    int getSpeed();
    int getTurnsInLife();
    bool isSBA();
    bool isMFD();
    void setAngle(double targetAngle);
    void setSpeed(int targetSpeed);
};

#endif /* defined(__battleSnake__Laser__) */
