//
//  Fighter.cpp
//  GiocoLabInf
//
//  Created by Enrico Migliorini on 25/03/14.
//  Copyright (c) 2014 Enrico Migliorini. All rights reserved.
//

#include "Fighter.h"

EnFighter::EnFighter(string plname) : Enemy(75, 75, 100, plname) {
    sprite = EN_FIGHTER;
}