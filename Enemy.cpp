//
//  Enemy.cpp
//  battleSnake
//
//  Created by Enrico Migliorini on 06/04/14.
//  Copyright (c) 2014 Enrico Migliorini. All rights reserved.
//

#include "Enemy.h"

Enemy::Enemy(int ba, int bd, int bh, string plname, location pos) : Spaceship(ba, bd, bh, pos, plname) {
    sprite = EN_FIGHTER;
}

void Enemy::drawEnemyOnScene(Graphics graph) {
    SDL_Rect renderZone = {(position.x-1) * SPRITE_WIDTH, (position.y-1) * SPRITE_HEIGHT, SPRITE_WIDTH, SPRITE_HEIGHT};
    SDL_RenderCopy(graph.getRenderer(), graph.getEnFighterSprite(), 0, &renderZone);
}