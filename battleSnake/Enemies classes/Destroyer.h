//
//  Destroyer.h
//  GiocoLabInf
//
//  Created by Enrico Migliorini on 25/03/14.
//  Copyright (c) 2014 Enrico Migliorini. All rights reserved.
//

#ifndef __GiocoLabInf__EnDestroyer__
#define __GiocoLabInf__EnDestroyer__

#include <iostream>

#include "Enemy.h"

class EnDestroyer: public Enemy {
private:
public:
    EnDestroyer() {};
    EnDestroyer(string plname = "EnDestroyer");
};


#endif /* defined(__GiocoLabInf__EnDestroyer__) */
