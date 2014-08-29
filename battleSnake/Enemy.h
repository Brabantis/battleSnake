//
//  Enemy.h
//  battleSnake
//
//  Created by Enrico Migliorini on 06/04/14.
//  Copyright (c) 2014 Enrico Migliorini. All rights reserved.
//

#ifndef __battleSnake__Enemy__
#define __battleSnake__Enemy__

#include "Spaceship.h"

using namespace std;

// Enemy is used to create any of the enemies
class Enemy : public Spaceship {
private:
public:
    Enemy();
    Enemy(int ba, int bd, int bh, Characters spritesrc, location pos = {0, 0});
    string getClass();
    void drawEnemyOnScene(Graphics* graph);
    // If I ever am to make the enemy move, I shall need to avoid him bouncing into walls by means of something like FleetMember::isColliding
};

#endif /* defined(__battleSnake__Enemy__) */
