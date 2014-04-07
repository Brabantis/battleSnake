//
//  Frigate.h
//  GiocoLabInf
//
//  Created by Enrico Migliorini on 25/03/14.
//  Copyright (c) 2014 Enrico Migliorini. All rights reserved.
//

#ifndef __GiocoLabInf__EnFrigate__
#define __GiocoLabInf__EnFrigate__

#include <iostream>

#include "Enemy.h"

class EnFrigate : public Enemy {
private:
public:
    EnFrigate() {};
    EnFrigate(string plname = "EnFrigate");
};

#endif /* defined(__GiocoLabInf__EnFrigate__) */
