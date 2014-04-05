//
//  Nerd.cpp
//  GiocoLabInf
//
//  Created by Enrico Migliorini on 25/03/14.
//  Copyright (c) 2014 Enrico Migliorini. All rights reserved.
//

#include "Nerd.h"

Nerd::Nerd(string plname) : PartyMember(150, 50, 50, plname) {
    sprite = NERD;
    abilityList[0] = "Calculus 101";
    abilityList[1] = "Newtonian Dynamics";
    abilityList[2] = "Neurotransmission";
    abilityList[3] = "String Theory";
}

void Nerd::firstAbility() {
    
}

void Nerd::secondAbility() {
    
}

void Nerd::thirdAbility() {
    
}

void Nerd::fourthAbility() {
    
}