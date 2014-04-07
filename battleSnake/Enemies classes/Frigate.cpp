//
//  Frigate.cpp
//  GiocoLabInf
//
//  Created by Enrico Migliorini on 25/03/14.
//  Copyright (c) 2014 Enrico Migliorini. All rights reserved.
//

#include "Frigate.h"

EnFrigate::EnFrigate(string plname) : Enemy(50, 50, 150, plname) {
    sprite = EN_FRIGATE;
}