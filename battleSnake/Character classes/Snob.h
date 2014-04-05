//
//  Snob.h
//  GiocoLabInf
//
//  Created by Enrico Migliorini on 25/03/14.
//  Copyright (c) 2014 Enrico Migliorini. All rights reserved.
//

#ifndef __GiocoLabInf__Snob__
#define __GiocoLabInf__Snob__

#include <iostream>

#include "PartyMember.h"

class Snob : public PartyMember {
private:
public:
    Snob() {};
    Snob(string plname = "Hero");
    
    virtual void firstAbility();
    virtual void secondAbility();
    virtual void thirdAbility();
    virtual void fourthAbility();
};

#endif /* defined(__GiocoLabInf__Snob__) */
