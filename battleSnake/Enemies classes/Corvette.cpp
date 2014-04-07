//
//  Corvette.cpp
//  GiocoLabInf
//
//  Created by Enrico Migliorini on 25/03/14.
//  Copyright (c) 2014 Enrico Migliorini. All rights reserved.
//

#include "Corvette.h"

EnCorvette::EnCorvette(string plname) : Enemy(100, 100, 50, plname) {
    sprite = EN_CORVETTE;
}