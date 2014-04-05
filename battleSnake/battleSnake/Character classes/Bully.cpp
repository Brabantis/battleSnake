//
//  Bully.cpp
//  GiocoLabInf
//
//  Created by Enrico Migliorini on 25/03/14.
//  Copyright (c) 2014 Enrico Migliorini. All rights reserved.
//

#include "Bully.h"

Bully::Bully(string plname) : PartyMember(100, 100, 150, plname) {
    sprite = BULLY;
    abilityList[0] = "Sucker Punch";
    abilityList[1] = "Scare";
    abilityList[2] = "Low Kick";
    abilityList[3] = "Trample";
}

void Bully::firstAbility() {
    
}

void Bully::secondAbility() {
    
}

void Bully::thirdAbility() {
    
}

void Bully::fourthAbility() {
    
}