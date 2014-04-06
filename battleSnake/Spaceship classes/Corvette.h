//
//  Corvette.h
//  GiocoLabInf
//
//  Created by Enrico Migliorini on 25/03/14.
//  Copyright (c) 2014 Enrico Migliorini. All rights reserved.
//

#ifndef __GiocoLabInf__Corvette__
#define __GiocoLabInf__Corvette__

#include <iostream>

#include "FleetMember.h"

class Corvette : public FleetMember {
private:
public:
    Corvette() {};
    Corvette(string plname = "Corvette");
};

#endif /* defined(__GiocoLabInf__Corvette__) */
