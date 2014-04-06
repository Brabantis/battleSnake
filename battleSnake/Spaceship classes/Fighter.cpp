//
//  Fighter.cpp
//  GiocoLabInf
//
//  Created by Enrico Migliorini on 25/03/14.
//  Copyright (c) 2014 Enrico Migliorini. All rights reserved.
//

#include "Fighter.h"

Fighter::Fighter(string plname) : FleetMember(75, 75, 100, plname) {
    sprite = FIGHTER;
}