//
//  Level.cpp
//  GiocoLabInf
//
//  Created by Enrico Migliorini on 19/03/14.
//  Copyright (c) 2014 Enrico Migliorini. All rights reserved.
//

#include "Level.h"

Level::Level() {
    for (int i = 0; i<MAP_WIDTH*MAP_HEIGHT; i++) {
        Tile* temptile = new Tile();
        temptile->setTileCode(EMPTY);
        temptile->occupyingAlly = nullptr;
        temptile->occupyingEnemy = nullptr;
        battlefield.push_back(temptile);
    }
    // A fence on the outside
    for (int i = 0; i<MAP_WIDTH; i++) {
        for (int j = 0; j<MAP_HEIGHT; j++) {
            if (j == 0 || j == MAP_HEIGHT-1 || i == 0 || i == MAP_WIDTH-1) {
                battlefield[i + MAP_WIDTH * j]->setTileCode(WALL);
            }
        }
    }
}

bool Level::isMapSafe() {
    for (int x = 0; x<MAP_WIDTH; x++) {
        for (int y = 0; y<MAP_HEIGHT; y++) {
            if (battlefield[x + MAP_WIDTH*y] == nullptr) {
                cout << "ERROR AT " << x << ", " << y << endl;
                return false;
            }
        }
    }
    return true;
}

Tile* Level::getTile(int x, int y) {
    return (battlefield[x + MAP_WIDTH * y]);
}

void Level::giveCodeToTile(int x, int y, Code tileCode, Spaceship* ship) {
    battlefield[x + MAP_WIDTH * y]->setTileCode(tileCode, ship);
}

void Level::printMap(Graphics* graph) {    // LA SCRIVE RUOTATA, VORSICHT.
    // Graphic of the map
    
    for (int j = 0; j<MAP_HEIGHT; j++) {
        for (int i = 0; i<MAP_WIDTH; i++) {
            int sym = 0;
            if (battlefield[i + MAP_WIDTH * j]->partOfWall) {
                sym = 1;
            }
            if (battlefield[i + MAP_WIDTH * j]->occupiedByAlly) {
                sym = 2;
            }
            if (battlefield[i + MAP_WIDTH * j]->occupiedByEnemy) {
                sym = 3;
            }
            SDL_Rect fillRect = {i*TILE_WIDTH, j*TILE_HEIGHT, TILE_WIDTH, TILE_HEIGHT};
            switch (sym) {
                case 0:
                    SDL_SetRenderDrawColor(graph->getRenderer(), 0xFF, 0xFF, 0xFF, 0xFF);
                    break;
                case 1:
                    SDL_SetRenderDrawColor(graph->getRenderer(), 0x00, 0x00, 0x00, 0xFF);
                    break;
                case 2:
                    SDL_SetRenderDrawColor(graph->getRenderer(), 0x00, 0xFF, 0x00, 0xFF);
                    break;
                case 3:
                    SDL_SetRenderDrawColor(graph->getRenderer(), 0xFF, 0x00, 0x00, 0xFF);
                    break;
                default:
                    break;
            }
            SDL_RenderFillRect(graph->getRenderer(), &fillRect);
            //cout << sym;
        }
    }
    SDL_RenderPresent(graph->getRenderer());
}

bool Level::thereIsCollision(int x, int y) {
    bool result = false;
    for (int i = 0; i<10; i++) {
        for (int j = 0; j<10; j++) {
            if (getTile(x + i, y + j) == nullptr) {
                // This happens when you're too close to the S or E border. It just means that it shouldn't be here.
                cout << "Trouble in tIC at " << x+i << ", " << y+j << endl;
                result = true;
            }
            if (getTile(x + i, y + j)->checkForCollision()) {
                result = true;
            }
        }
    }
    return result;
}
