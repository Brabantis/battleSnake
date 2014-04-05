//
//  Oldlady.cpp
//  GiocoLabInf
//
//  Created by Enrico Migliorini on 25/03/14.
//  Copyright (c) 2014 Enrico Migliorini. All rights reserved.
//

#include "Oldlady.h"

Oldlady::Oldlady(string plname) : PartyMember(100, 100, 50, plname) {
    sprite = OLD_LADY;
    abilityList[0] = "Back In My Time";
    abilityList[1] = "Relic Bag";
    abilityList[2] = "Heavy Cooking";
    abilityList[3] = "Not Dead Yet";
}

void Oldlady::firstAbility() {
    
}

void Oldlady::secondAbility() {
    
}

void Oldlady::thirdAbility() {
    
}

void Oldlady::fourthAbility() {
    
}