//
//  Tile.cpp
//  battleSnake
//
//  Created by Enrico Migliorini on 04/05/14.
//  Copyright (c) 2014 Enrico Migliorini. All rights reserved.
//

#include "Tile.h"

void Tile::setTileCode(Code target){
    switch (target) {
        case EMPTY:
            occupiedByAlly = false;
            occupiedByEnemy = false;
            partOfWall = false;
            break;
        case WALKABLE:  // Don't think that I'll use this much, but it's here for completion's sake
            partOfWall = false;
            break;
        case WALL:
            partOfWall = true;
            break;
        case ENEMY:
            occupiedByEnemy = true;
            break;
        case ALLY:
            occupiedByAlly = true;
            break;
        default:
            break;
    };
}

/*void Tile::setEnemy(bool how){
    occupiedByEnemy = how;
}

void Tile::setAlly(bool how){
    occupiedByAlly = how;
}*/

Spaceship* Tile::getEnemy(){
    return occupyingEnemy;
}

Spaceship* Tile::getAlly(){
    return occupyingAlly;
}

bool Tile::checkForCollision(){
    if (occupiedByEnemy && occupiedByAlly)
        return true;
    return false;
}