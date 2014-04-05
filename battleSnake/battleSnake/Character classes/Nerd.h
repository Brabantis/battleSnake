//
//  Nerd.h
//  GiocoLabInf
//
//  Created by Enrico Migliorini on 25/03/14.
//  Copyright (c) 2014 Enrico Migliorini. All rights reserved.
//

#ifndef __GiocoLabInf__Nerd__
#define __GiocoLabInf__Nerd__

#include <iostream>

#include "PartyMember.h"

class Nerd : public PartyMember {
private:
public:
    Nerd() {};
    Nerd(string plname = "Hero");
    
    virtual void firstAbility();
    virtual void secondAbility();
    virtual void thirdAbility();
    virtual void fourthAbility();
};

#endif /* defined(__GiocoLabInf__Nerd__) */
