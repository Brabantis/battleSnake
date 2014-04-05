//
//  Hippie.h
//  GiocoLabInf
//
//  Created by Enrico Migliorini on 25/03/14.
//  Copyright (c) 2014 Enrico Migliorini. All rights reserved.
//

#ifndef __GiocoLabInf__Hippie__
#define __GiocoLabInf__Hippie__

#include <iostream>

#include "PartyMember.h"

class Hippie : public PartyMember {
private:
public:
    Hippie() {};
    Hippie(string plname = "Hero");
    
    virtual void firstAbility();
    virtual void secondAbility();
    virtual void thirdAbility();
    virtual void fourthAbility();
};

#endif /* defined(__GiocoLabInf__Hippie__) */
