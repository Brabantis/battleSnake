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

#include <iostream>
#include <string>

using namespace std;

struct location {
    int x, y; // coordinates divided by 10 to move on tiles
    Direction orient;
};

class Spaceship {
protected:
    int atk, def, hp;
    location position;
    Characters sprite;
    
    void calculateStats();
    void explode();
    
public:             //Solo le funzioni base per tutti
    
    Spaceship() {}                                          // Default
    Spaceship(int ba, int bd, int bh, Characters spritesrc, location pos = {0, 0, SOUTH});
    
    void move(Direction dest, Level &lvl);
    
    // to draw on scene, duh
    void drawOnScene(Graphics graph);
    
    Laser shootLaser(int xdest, int ydest);
    void takeDamage(int damage);
    
    // the getters
    int getAtk();
    int getDef();
    int getHP();
    int getX();
    int getY();
    Direction getDirection();
    Characters getSprite();
    
    // the setters
    void setX(int value);
    void setY(int value);
    void setDirection(Direction next);
};

#endif /* defined(__GiocoLabInf__Creature__) */
