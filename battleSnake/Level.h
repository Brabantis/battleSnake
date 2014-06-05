//
//  Level.h
//  GiocoLabInf
//
//  Created by Enrico Migliorini on 19/03/14.
//  Copyright (c) 2014 Enrico Migliorini. All rights reserved.
//

//==================================================//
//                                                  //
//   Level stores a map and info about environment  //
//                                                  //
//==================================================//

#ifndef __GiocoLabInf__Level__
#define __GiocoLabInf__Level__

#include <iostream>

#include "Graphics.h"
#include "Tile.h"

const int MAP_HEIGHT = SCREEN_HEIGHT/TILE_HEIGHT;
const int MAP_WIDTH = SCREEN_WIDTH/TILE_WIDTH;
const int dimension = MAP_HEIGHT*MAP_WIDTH;

class Level {
private:
    // Mapping the map: 0 for empty, 1 for wall, 2 for walkable, 3 for trigger
    Tile battlefield[dimension];
public:
    Level();    // to instantiate level from map
    Tile getTile(int x, int y);
    Event getEventFromCode(int code);
    void printMap(Graphics graph);
    void giveCodeToTile(int x, int y, Code tileCode, Spaceship* ship = 0);
};


#endif /* defined(__GiocoLabInf__Level__) */
