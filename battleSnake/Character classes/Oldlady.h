//
//  Oldlady.h
//  GiocoLabInf
//
//  Created by Enrico Migliorini on 25/03/14.
//  Copyright (c) 2014 Enrico Migliorini. All rights reserved.
//

#ifndef __GiocoLabInf__Oldlady__
#define __GiocoLabInf__Oldlady__

#include <iostream>

#include "PartyMember.h"

class Oldlady : public PartyMember {
private:
public:
    Oldlady() {};
    Oldlady(string plname = "Hero");
    
    virtual void firstAbility();
    virtual void secondAbility();
    virtual void thirdAbility();
    virtual void fourthAbility();
};

#endif /* defined(__GiocoLabInf__Oldlady__) */
