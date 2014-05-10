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

#include <iostream>

#include <cmath>

#include "Graphics.h"
#include "Level.h"

const int LASERWIDTH = 5;
const int LASERHEIGHT = 20;

class Laser {
private:
    OtherSprites sprite;
    int power;
    float gX, gY;     // Graphics coordinates of the collision center (on top)
    double angle;      // I will build the direction moving according to the sin and cos of the angle
public:
    int TEMPlifeCounter = 0;
    Laser(int power, int gX, int gY, double angle, OtherSprites part = LASER_BASIC);
    bool isHittingEnemy(Level currLevel);
    bool isHittingWall(Level currLevel);
    void travel();
    void drawOnScreen(Graphics graph);
    
    double getAngle();
};

#endif /* defined(__battleSnake__Laser__) */
