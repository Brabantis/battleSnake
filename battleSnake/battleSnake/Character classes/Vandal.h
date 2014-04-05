//
//  Vandal.h
//  GiocoLabInf
//
//  Created by Enrico Migliorini on 25/03/14.
//  Copyright (c) 2014 Enrico Migliorini. All rights reserved.
//

#ifndef __GiocoLabInf__Vandal__
#define __GiocoLabInf__Vandal__

#include <iostream>

#include "PartyMember.h"

class Vandal : public PartyMember {
private:
public:
    Vandal() {};
    Vandal(string plname = "Hero");
    
    virtual void firstAbility();
    virtual void secondAbility();
    virtual void thirdAbility();
    virtual void fourthAbility();
};

#endif /* defined(__GiocoLabInf__Vandal__) */
