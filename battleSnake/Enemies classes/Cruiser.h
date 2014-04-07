//
//  Cruiser.h
//  GiocoLabInf
//
//  Created by Enrico Migliorini on 25/03/14.
//  Copyright (c) 2014 Enrico Migliorini. All rights reserved.
//

#ifndef __GiocoLabInf__EnCruiser__
#define __GiocoLabInf__EnCruiser__

#include <iostream>

#include "Enemy.h"

class EnCruiser : public Enemy {
private:
public:
    EnCruiser() {};
    EnCruiser(string plname = "EnCruiser");
};

#endif /* defined(__GiocoLabInf__EnCruiser__) */
