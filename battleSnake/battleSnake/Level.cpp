//
//  Level.cpp
//  GiocoLabInf
//
//  Created by Enrico Migliorini on 19/03/14.
//  Copyright (c) 2014 Enrico Migliorini. All rights reserved.
//

#include "Level.h"

// Of course this is not the final one. I will give him maps from a file.
Level::Level(string name) {
    for (int i = 0; i<MAPWIDTH*MAPWIDTH; i++) {
        map[i]=2;
    }
    // To check that the functions work
    map[8 + MAPWIDTH*6] = 3;    // 9,7
    map[5 + MAPWIDTH*2] = 4;    // 6,3
    // A fence on the outside
    for (int i = 0; i<MAPWIDTH; i++) {
        map[i + MAPWIDTH*0] = 1;
        map[0 + MAPWIDTH*i] = 1;
        map[-1 + MAPWIDTH*(i+1)] = 1;
        map[i + MAPWIDTH*(MAPWIDTH-1)] = 1;
    }
}

int Level::getTileCode(int x, int y) {
    return map[(x-1) + MAPWIDTH*(y-1)];
}

Event Level::getEventFromCode(int code) {
    switch (code) {
        case 3:     // Once again, totally temporary
            return LEV_CLOSURE;
            break;
        case 4:
            return LEV_START;
            break;
        default:
            return ERR_LEVEL;
            break;
    };
}

void Level::printMap() {
    for (int i = 0; i<MAPWIDTH; i++) {
        for (int j = 0; j<MAPWIDTH; j++) {
            cout << map[j + MAPWIDTH*i] << ", ";
        }
        cout << endl;
    }
}
