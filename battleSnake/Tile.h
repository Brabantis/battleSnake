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

class Spaceship;

const int TILE_WIDTH = 4;
const int TILE_HEIGHT = 4;

// Should this be here? IDK, but this is one of the most basic classes I have.
enum Event {
    ERR_LEVEL,
    GAME_LOST,
    LEVEL_CLEAR,
    EVE_DEFAULT
};

enum Code {
    EMPTY,
    NOT_WALL,
    WALL,
    ENEMY,
    ALLY,
    NOT_ENEMY,
    NOT_ALLY
};

class Tile {
public:
    // This was a good idea. I just had to change a bit SetTileCode, and now it's smoother and more readable.
    bool occupiedByEnemy;
    bool occupiedByAlly;
    bool partOfWall;
    void setTileCode(Code target, Spaceship* ship = 0); // ship is defaulted to 0 by Level
    
    //void setEnemy(bool how);
    //void setAlly(bool how);
    Spaceship* getEnemy();
    Spaceship* getAlly();
    void setAlly(bool how);
    void setEnemy(bool how);
    bool checkForCollision();
    bool checkForWallCollision();
    
    // Could be useful for collision detecting. Like destroying both the ships.
//private:
    Spaceship* occupyingEnemy;
    Spaceship* occupyingAlly;
};

#endif /* defined(__battleSnake__Tile__) */
