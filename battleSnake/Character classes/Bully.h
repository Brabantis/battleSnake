//
//  Bully.h
//  GiocoLabInf
//
//  Created by Enrico Migliorini on 25/03/14.
//  Copyright (c) 2014 Enrico Migliorini. All rights reserved.
//

#ifndef __GiocoLabInf__Bully__
#define __GiocoLabInf__Bully__

#include <iostream>

#include "PartyMember.h"

class Bully: public PartyMember {
private:
public:
    Bully() {};
    Bully(string plname = "Hero");
    
    
    virtual void firstAbility();
    virtual void secondAbility();
    virtual void thirdAbility();
    virtual void fourthAbility();
};


#endif /* defined(__GiocoLabInf__Bully__) */
