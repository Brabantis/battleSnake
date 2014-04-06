//
//  Cruiser.h
//  GiocoLabInf
//
//  Created by Enrico Migliorini on 25/03/14.
//  Copyright (c) 2014 Enrico Migliorini. All rights reserved.
//

#ifndef __GiocoLabInf__Cruiser__
#define __GiocoLabInf__Cruiser__

#include <iostream>

#include "FleetMember.h"

class Cruiser : public FleetMember {
private:
public:
    Cruiser() {};
    Cruiser(string plname = "Cruiser");
};

#endif /* defined(__GiocoLabInf__Cruiser__) */
