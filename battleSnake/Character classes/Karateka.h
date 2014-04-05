//
//  Karateka.h
//  GiocoLabInf
//
//  Created by Enrico Migliorini on 25/03/14.
//  Copyright (c) 2014 Enrico Migliorini. All rights reserved.
//

#ifndef __GiocoLabInf__Karateka__
#define __GiocoLabInf__Karateka__

#include <iostream>

#include "PartyMember.h"

class Karateka : public PartyMember {
private:
public:
    Karateka() {};
    Karateka(string plname = "Hero");
    
    virtual void firstAbility();
    virtual void secondAbility();
    virtual void thirdAbility();
    virtual void fourthAbility();
};

#endif /* defined(__GiocoLabInf__Karateka__) */
