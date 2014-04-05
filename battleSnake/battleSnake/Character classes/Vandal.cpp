//
//  Vandal.cpp
//  GiocoLabInf
//
//  Created by Enrico Migliorini on 25/03/14.
//  Copyright (c) 2014 Enrico Migliorini. All rights reserved.
//

#include "Vandal.h"

Vandal::Vandal(string plname) : PartyMember(150, 50, 50, plname) {
    sprite = VANDAL;
    abilityList[0] = "Vandalize";
    abilityList[1] = "Back Alley";
    abilityList[2] = "Ambush";
    abilityList[3] = "Street Smart";
}

void Vandal::firstAbility() {
    
}

void Vandal::secondAbility() {
    
}

void Vandal::thirdAbility() {
    
}

void Vandal::fourthAbility() {
    
}