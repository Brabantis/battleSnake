//
//  Enemy.cpp
//  battleSnake
//
//  Created by Enrico Migliorini on 06/04/14.
//  Copyright (c) 2014 Enrico Migliorini. All rights reserved.
//

#include "Enemy.h"

Enemy::Enemy() {}

Enemy::Enemy(int ba, int bd, int bh, Characters spritesrc, location pos) : Spaceship(ba, bd, bh, spritesrc, pos) {
}

// Redefine move()