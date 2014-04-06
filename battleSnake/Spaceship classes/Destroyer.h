//
//  Destroyer.h
//  GiocoLabInf
//
//  Created by Enrico Migliorini on 25/03/14.
//  Copyright (c) 2014 Enrico Migliorini. All rights reserved.
//

#ifndef __GiocoLabInf__Destroyer__
#define __GiocoLabInf__Destroyer__

#include <iostream>

#include "FleetMember.h"

class Destroyer: public FleetMember {
private:
public:
    Destroyer() {};
    Destroyer(string plname = "Destroyer");
};


#endif /* defined(__GiocoLabInf__Destroyer__) */
