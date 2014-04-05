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

const int MAPWIDTH = 30; // Temporary

enum Event {
    // THIS will be filled with event codes. BAT for BATtles, NAR for NARration.
    ERR_LEVEL,
    BAT_TUTORIAL,
    NAR_TUTORIALEND,
    LEV_START,
    LEV_CLOSURE
    // ...
};

class Level {
private:
    // Mapping the map: 0 for empty, 1 for wall, 2 for walkable, 3 for trigger
    short int map[MAPWIDTH*MAPWIDTH];
    SDL_Texture* graphicMap;
public:
    Level(string name);    // to instantiate level from map, put there characters, then give back control
    int getTileCode(int x, int y);
    Event getEventFromCode(int code);
    void printMap();
    // A method to check for triggers should be here
};


#endif /* defined(__GiocoLabInf__Level__) */
