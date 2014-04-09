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

FleetMember::FleetMember(int ba, int bd, int bh, string plname, location pos) : Spaceship(ba, bd, bh, pos, plname) {
    // The sprite will be left uninitialized if the FleetMember constructor is used instead of the character class one
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

void FleetMember::move(Direction dest) {
    switch (dest) {
        case NORTH:
            position.y--;
            break;
        case EAST:
            position.x++;
            break;
        case SOUTH:
            position.y++;
            break;
        case WEST:
            position.x--;
            break;
        default:
            break;
    }
    position.orient = dest;
}
