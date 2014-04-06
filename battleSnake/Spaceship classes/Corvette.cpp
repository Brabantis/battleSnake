//
//  Corvette.cpp
//  GiocoLabInf
//
//  Created by Enrico Migliorini on 25/03/14.
//  Copyright (c) 2014 Enrico Migliorini. All rights reserved.
//

#include "Corvette.h"

Corvette::Corvette(string plname) : FleetMember(100, 100, 50, plname) {
    sprite = CORVETTE;
}