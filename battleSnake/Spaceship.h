//
//  Spaceship.h
//  GiocoLabInf
//
//  Created by Enrico Migliorini on 12/03/14.
//  Copyright (c) 2014 Enrico Migliorini. All rights reserved.
//


//==================================================//
//                                                  //
//  Spaceship is the base class for every creature   //
//                                                  //
//==================================================//

#ifndef __GiocoLabInf__Creature__
#define __GiocoLabInf__Creature__

#include "Graphics.h"
#include "Laser.h"

#include <string>

using namespace std;

struct location {
    double x, y; // coordinates divided by 10 to move on tiles
};

class Spaceship {
protected:
    int atk, def, hp;
    location position;
    Characters sprite;
    
    bool allied;
    
    void calculateStats();
    
public:             // just base funcs
    
    Spaceship() {}
    Spaceship(int ba, int bd, int bh, Characters spritesrc, location pos = {0, 0});
    
    // teleport is the new move
    void teleport(int dX, int dY);
    
    // to draw on scene, duh
    void drawOnScene(Graphics* graph);
    
    // i have to give the sprite, which is better for colourful patterns
    Laser* shootLaser(int xdest, int ydest, OtherSprites sprt);
    Laser* shootLaser(double angle, OtherSprites sprt);
    
    void takeDamage(int damage);
    
    // the getters
    int getAtk();
    int getDef();
    int getHP();
    double getX();
    double getY();
    double getCenterX();
    double getCenterY();
    Characters getSprite();
    bool isAlive();
    bool isAllied();
    
    // the setters
    void setAtk(int value);
    void setX(int value);
    void setY(int value);
    void setSprite(Characters spritesrc);
};

#endif /* defined(__GiocoLabInf__Creature__) */
