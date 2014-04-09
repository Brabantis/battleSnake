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
    string name;
    
    void shootLaser(int xtarget, int ytarget);
    void takeDamage(int damage);
    void calculateStats();
    void explode();
    
public:             //Solo le funzioni base per tutti
    
    Spaceship() {}                                          // Default
    Spaceship(int ba, int bd, int bh, location pos = {0, 0, SOUTH}, string crname = "Mook");
    
    // to draw on scene, duh
    void drawOnScene(Graphics graph);
    
    // the getters
    string getName();
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
