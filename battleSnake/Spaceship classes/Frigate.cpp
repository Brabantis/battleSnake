//
//  Frigate.cpp
//  GiocoLabInf
//
//  Created by Enrico Migliorini on 25/03/14.
//  Copyright (c) 2014 Enrico Migliorini. All rights reserved.
//

#include "Frigate.h"

Frigate::Frigate(string plname) : FleetMember(50, 50, 150, plname) {
    sprite = FRIGATE;
}