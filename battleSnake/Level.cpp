//
//  Level.cpp
//  GiocoLabInf
//
//  Created by Enrico Migliorini on 19/03/14.
//  Copyright (c) 2014 Enrico Migliorini. All rights reserved.
//

#include "Level.h"

Level::Level() {
    // TODO al momento non aggiunge x e y alle tiles
    for (int i = 0; i<MAP_WIDTH*MAP_HEIGHT; i++) {
        battlefield[i].setTileCode(WALK);
    }
    
    // A fence on the outside
    for (int i = 0; i<MAP_WIDTH; i++) {
        for (int j = 0; j<MAP_HEIGHT; j++) {
            if (j == 0 || j == MAP_HEIGHT-1 || i == 0 || i == MAP_WIDTH-1) {
                battlefield[i + MAP_WIDTH * j].setTileCode(COLLISION);
            }
        }
    }
}

Event Level::getEventFromCode(int code) {
    switch (code) {
        case COLLISION:     // Once again, totally temporary
            return GAME_LOST;
            break;
        case ENEMY_HERE:
            return EAT_ENEMY;
            break;
        default:
            return ERR_LEVEL;
            break;
    };
}

int Level::getTileCode(int x, int y) {
    return battlefield[x + MAP_WIDTH * y].getTileCode();
}

void Level::giveCodeToTile(int x, int y, Code tileCode) {
    battlefield[x + MAP_WIDTH * y].setTileCode(tileCode);
}



