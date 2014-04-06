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

const int MAPWIDTH = 20; // Temporary
const int MAPHEIGHT = 15;

// The codes for the tiles
enum Code {
    UNACCESSIBLE,
    NOT_WALK,
    WALK,
    GAME_CLOSE,
    ENEMY_HERE,
    SPEC_EVENT_2,
    SPEC_EVENT_3,
};

enum Event {
    // THIS will be filled with event codes. BAT for BATtles, NAR for NARration.
    ERR_LEVEL,
    LEV_CLOSURE,
    EAT_ENEMY,
    EVE_DEFAULT,
    // ...
};

class Level {
private:
    // Mapping the map: 0 for empty, 1 for wall, 2 for walkable, 3 for trigger
    short int map[MAPWIDTH*MAPWIDTH];
    SDL_Texture* graphicMap;
public:
    Level();    // to instantiate level from map
    int getTileCode(int x, int y);
    Event getEventFromCode(int code);
    void printMap();
    void giveCodeToTile(int x, int y, Code tileCode);
};


#endif /* defined(__GiocoLabInf__Level__) */
