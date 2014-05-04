//
//  Tile.h
//  battleSnake
//
//  Created by Enrico Migliorini on 04/05/14.
//  Copyright (c) 2014 Enrico Migliorini. All rights reserved.
//

//==================================================//
//                                                  //
//          Tiles make up the battlefield           //
//               and store collision                //
//==================================================//

#ifndef __battleSnake__Tile__
#define __battleSnake__Tile__

#include <iostream>

const int TILE_WIDTH = 4;
const int TILE_HEIGHT = 4;

enum Event {
    ERR_LEVEL,
    GAME_LOST,
    EAT_ENEMY,
    EVE_DEFAULT
};

enum Code {
    UNACCESSIBLE,
    NOT_WALK,
    WALK,
    COLLISION,
    ENEMY_HERE,
    SPEC_EVENT_2,
    SPEC_EVENT_3,
};

class Tile {
private:
    Code tileCode;
    int x;
    int y;
public:
    Code getTileCode();
    void setTileCode(Code target);
};

#endif /* defined(__battleSnake__Tile__) */
