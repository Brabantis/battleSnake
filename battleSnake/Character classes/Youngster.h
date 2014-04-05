//
//  Youngster.h
//  GiocoLabInf
//
//  Created by Enrico Migliorini on 25/03/14.
//  Copyright (c) 2014 Enrico Migliorini. All rights reserved.
//

#ifndef __GiocoLabInf__Youngster__
#define __GiocoLabInf__Youngster__

#include <iostream>

#include "PartyMember.h"

class Youngster : public PartyMember {
private:
public:
    Youngster() {};
    Youngster(string plname = "Hero");
    
    // It will be responsibility of the Engine::printBattleInfo function to draw these abilities names on screen.
    // These all use no parameters, or I'll have trouble with Engine when I need to call them. They will use SDL events to check the target(s)
    // Passive? Priority low
    
    virtual void firstAbility();
    virtual void secondAbility();
    virtual void thirdAbility();
    virtual void fourthAbility();
};

#endif /* defined(__GiocoLabInf__Youngster__) */
