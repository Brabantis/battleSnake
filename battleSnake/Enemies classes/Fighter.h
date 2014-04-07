//
//  Fighter.h
//  GiocoLabInf
//
//  Created by Enrico Migliorini on 25/03/14.
//  Copyright (c) 2014 Enrico Migliorini. All rights reserved.
//

#ifndef __GiocoLabInf__EnFighter__
#define __GiocoLabInf__EnFighter__

#include <iostream>

#include "Enemy.h"

class EnFighter : public Enemy {
private:
public:
    EnFighter() {};
    EnFighter(string plname = "EnFighter");
};

#endif /* defined(__GiocoLabInf__EnFighter__) */
