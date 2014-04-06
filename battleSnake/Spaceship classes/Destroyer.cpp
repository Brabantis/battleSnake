//
//  Destroyer.cpp
//  GiocoLabInf
//
//  Created by Enrico Migliorini on 25/03/14.
//  Copyright (c) 2014 Enrico Migliorini. All rights reserved.
//

#include "Destroyer.h"

Destroyer::Destroyer(string plname) : FleetMember(100, 100, 150, plname) {
    sprite = DESTROYER;
}