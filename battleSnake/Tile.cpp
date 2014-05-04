//
//  Tile.cpp
//  battleSnake
//
//  Created by Enrico Migliorini on 04/05/14.
//  Copyright (c) 2014 Enrico Migliorini. All rights reserved.
//

#include "Tile.h"

void Tile::setTileCode(Code target){
    tileCode = target;
}

Code Tile::getTileCode() {
    return tileCode;
}