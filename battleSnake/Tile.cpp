//
//  Tile.cpp
//  battleSnake
//
//  Created by Enrico Migliorini on 04/05/14.
//  Copyright (c) 2014 Enrico Migliorini. All rights reserved.
//

#include "Tile.h"

void Tile::setTileCode(Code target, Spaceship* ship){
    switch (target) {
        case EMPTY:
            occupiedByAlly = false;
            occupiedByEnemy = false;
            partOfWall = false;
            break;
        case NOT_WALL:  // Don't think that I'll use this much, but it's here for completion's sake
            partOfWall = false;
            break;
        case WALL:
            partOfWall = true;
            break;
        case ENEMY:
            occupiedByEnemy = true;
            occupyingEnemy = ship;
            break;
        case ALLY:
            occupiedByAlly = true;
            occupyingAlly = ship;
            break;
        case NOT_ALLY:
            occupiedByAlly = false;
            occupyingAlly = nullptr;
            break;
        case NOT_ENEMY:
            occupiedByEnemy = false;
            occupyingEnemy = nullptr;
            break;
        default:
            break;
    };
}

void Tile::setEnemy(bool how){
    occupiedByEnemy = how;
}

void Tile::setAlly(bool how){
    occupiedByAlly = how;
}

Spaceship* Tile::getEnemy(){
    return occupyingEnemy;
}

Spaceship* Tile::getAlly(){
    return occupyingAlly;
}

// Remember that this checks only for enemies.
bool Tile::checkForCollision(){
    if (occupiedByEnemy && occupiedByAlly)
        // This gives another bug
        return true;
    return false;
}

bool Tile::checkForWallCollision(){
    if (occupiedByAlly && partOfWall)
        return true;
    return false;
}
