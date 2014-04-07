//
//  Corvette.h
//  GiocoLabInf
//
//  Created by Enrico Migliorini on 25/03/14.
//  Copyright (c) 2014 Enrico Migliorini. All rights reserved.
//

#ifndef __GiocoLabInf__EnCorvette__
#define __GiocoLabInf__EnCorvette__

#include <iostream>

#include "Enemy.h"

class EnCorvette : public Enemy {
private:
public:
    EnCorvette() {};
    EnCorvette(string plname = "EnCorvette");
};

#endif /* defined(__GiocoLabInf__EnCorvette__) */
