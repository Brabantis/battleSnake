//
//  FleetMember.cpp
//  GiocoLabInf
//
//  Created by Enrico Migliorini on 12/03/14.
//  Copyright (c) 2014 Enrico Migliorini. All rights reserved.
//

#include "FleetMember.h"
#include "Spaceship.h"
#include "Item.h"

FleetMember::FleetMember() {}

FleetMember::FleetMember(int ba, int bd, int bh, Characters sprite, location pos) : Spaceship(ba, bd, bh, sprite, pos) {
    armor = 0;
    weapon = 0;
}

string FleetMember::getClass() {
    string value;
    switch (sprite) {
        case FIGHTER:
            value = "Fighter";
            break;
        case CORVETTE:
            value = "Corvette";
            break;
        case FRIGATE:
            value = "Frigate";
            break;
        case DESTROYER:
            value = "Destroyer";
            break;
        case CRUISER:
            value = "Cruiser";
            break;
        default:
            value = "NOT A VALID CLASS";
            break;
    }
    return value;
}

bool FleetMember::isColliding(Level lvl){
    for (int i = 0; i<10; i++) {
        for (int j = 0; j<10; j++) {
            if (lvl.getTile(position.x + i, position.y + j)->checkForCollision()) {
                return true;
            }
        }
    }
    return false;
}

