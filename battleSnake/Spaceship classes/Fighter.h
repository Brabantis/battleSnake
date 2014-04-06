//
//  Fighter.h
//  GiocoLabInf
//
//  Created by Enrico Migliorini on 25/03/14.
//  Copyright (c) 2014 Enrico Migliorini. All rights reserved.
//

#ifndef __GiocoLabInf__Fighter__
#define __GiocoLabInf__Fighter__

#include <iostream>

#include "FleetMember.h"

class Fighter : public FleetMember {
private:
public:
    Fighter() {};
    Fighter(string plname = "Fighter");
};

#endif /* defined(__GiocoLabInf__Fighter__) */
