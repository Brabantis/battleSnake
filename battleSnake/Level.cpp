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
        battlefield[i].setTileCode(WALKABLE);
    }
    
    // A fence on the outside
    for (int i = 0; i<MAP_WIDTH; i++) {
        for (int j = 0; j<MAP_HEIGHT; j++) {
            if (j == 0 || j == MAP_HEIGHT-1 || i == 0 || i == MAP_WIDTH-1) {
                battlefield[i + MAP_WIDTH * j].setTileCode(WALL);
            }
        }
    }
}

Event Level::getEventFromCode(int code) {
    switch (code) {
        case WALL:     // Once again, totally temporary
            return GAME_LOST;
            break;
        case ENEMY:
            return EAT_ENEMY;
            break;
        default:
            return ERR_LEVEL;
            break;
    };
}

Tile Level::getTile(int x, int y) {
    return battlefield[x + MAP_WIDTH * y];
}

void Level::giveCodeToTile(int x, int y, Code tileCode) {
    battlefield[x + MAP_WIDTH * y].setTileCode(tileCode);
}

void Level::printMap() {    // LA SCRIVE RUOTATA, VORSICHT. Scambiare i, j, width e height invece sputtana qualcosa.
    for (int i = 0; i<MAP_WIDTH; i++) {
        for (int j = 0; j<MAP_HEIGHT; j++) {
            cout << (battlefield[i + MAP_WIDTH * j].partOfWall? 1 : 0);
        }
    }
}

