//
//  Frigate.h
//  GiocoLabInf
//
//  Created by Enrico Migliorini on 25/03/14.
//  Copyright (c) 2014 Enrico Migliorini. All rights reserved.
//

#ifndef __GiocoLabInf__Frigate__
#define __GiocoLabInf__Frigate__

#include <iostream>

#include "FleetMember.h"

class Frigate : public FleetMember {
private:
public:
    Frigate() {};
    Frigate(string plname = "Frigate");
};

#endif /* defined(__GiocoLabInf__Frigate__) */
