//
//  Level.cpp
//  GiocoLabInf
//
//  Created by Enrico Migliorini on 19/03/14.
//  Copyright (c) 2014 Enrico Migliorini. All rights reserved.
//

#include "Level.h"

// Of course this is not the final one. I will give him maps from a file.
Level::Level() {
    for (int i = 0; i<MAPWIDTH*MAPHEIGHT; i++) {
        map[i]=WALK;
    }
    // A fence on the outside
    for (int i = 0; i<MAPWIDTH; i++) {
        for (int j = 0; j<MAPHEIGHT; j++) {
            if (j == 0 || j == MAPHEIGHT-1 || i == 0 || i == MAPWIDTH-1) {
                map[i + MAPWIDTH * j] = COLLISION;
            }
        }
    }
}

int Level::getTileCode(int x, int y) {
    return map[(x-1) + MAPWIDTH*(y-1)];
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

void Level::printMap() {
    for (int i = 0; i<MAPHEIGHT; i++) {
        for (int j = 0; j<MAPWIDTH; j++) {
            cout << map[j + MAPWIDTH*i] << ", ";
        }
        cout << endl;
    }
}

void Level::giveCodeToTile(int x, int y, Code tileCode) {
    map[(x-1) + MAPWIDTH * (y-1)] = tileCode;   // Assign a code to a tile
}
