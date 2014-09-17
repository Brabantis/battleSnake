//
//  FleetMember.cpp
//  GiocoLabInf
//
//  Created by Enrico Migliorini on 12/03/14.
//  Copyright (c) 2014 Enrico Migliorini. All rights reserved.
//

#include "FleetMember.h"
#include "Spaceship.h"

FleetMember::FleetMember() {}

FleetMember::FleetMember(int ba, int bd, int bh, Characters sprite, location pos) : Spaceship(ba, bd, bh, sprite, pos) {
    isAllied = true;
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
        default:
            value = "NOT A VALID CLASS";
            break;
    }
    return value;
}

