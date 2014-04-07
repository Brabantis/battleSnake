//
//  Enemy.h
//  battleSnake
//
//  Created by Enrico Migliorini on 06/04/14.
//  Copyright (c) 2014 Enrico Migliorini. All rights reserved.
//

#ifndef __battleSnake__Enemy__
#define __battleSnake__Enemy__

#include <iostream>
#include <string>
#include "Spaceship.h"

using namespace std;

class Enemy : public Spaceship {
private:
public:
    Enemy() {};
    Enemy(int ba, int bd, int bh, string plname = "Enemy", location pos = {0, 0, NORTH});
    string getClass();
    void drawEnemyOnScene(Graphics graph);
};

#endif /* defined(__battleSnake__Enemy__) */
