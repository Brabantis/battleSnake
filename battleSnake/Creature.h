//
//  Creature.h
//  GiocoLabInf
//
//  Created by Enrico Migliorini on 12/03/14.
//  Copyright (c) 2014 Enrico Migliorini. All rights reserved.
//


//==================================================//
//                                                  //
//  Creature is the base class for every creature   //
//                                                  //
//==================================================//

#ifndef __GiocoLabInf__Creature__
#define __GiocoLabInf__Creature__

#include "Graphics.h"

#include <iostream>
#include <string>

using namespace std;

enum power {NONE, WEAK, MEDIUM, STRONG};

struct location {
    int x, y; // coordinates divided by 10 to move on tiles
    Direction orient;
};

class Creature {
protected:
    int base_atk, base_def, base_hp;
    int atk, def, hp;
    int max_atk, max_def, max_hp;
    int level;
    bool poisoned, stunned;
    location position;
    Characters sprite;
    string name;
public:             //Solo le funzioni base per tutti
    
    Creature() {}                                          // Default
    Creature(int ba, int bd, int bh, int lev, location pos = {0, 0, SOUTH}, string crname = "Mook");
    //~Creature();                                          // Default destructor is OK
    void attack(Creature enemy, power strength);
    void takeDamage(int damage);
    void getPoisoned();
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
