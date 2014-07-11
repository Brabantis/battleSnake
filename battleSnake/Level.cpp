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
        battlefield[i].setTileCode(EMPTY);
    }
    
    // A fence on the outside
    for (int i = 0; i<MAP_WIDTH; i++) {
        for (int j = 0; j<MAP_HEIGHT; j++) {
            if (j == 0 || j == 1 || j == MAP_HEIGHT-1 || j == MAP_HEIGHT-2 || i == 0 || i == 1 || i == MAP_WIDTH-1 || i == MAP_WIDTH-2) {
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
            return KILL_ENEMY;
            break;
        default:
            return ERR_LEVEL;
            break;
    };
}

Tile Level::getTile(int x, int y) {
    return battlefield[x + MAP_WIDTH * y];
}

void Level::giveCodeToTile(int x, int y, Code tileCode, Spaceship* ship) {
    battlefield[x + MAP_WIDTH * y].setTileCode(tileCode, ship);
}

void Level::printMap(Graphics* graph) {    // LA SCRIVE RUOTATA, VORSICHT.
    // Graphic of the map
    
    for (int j = 0; j<MAP_HEIGHT; j++) {
        for (int i = 0; i<MAP_WIDTH; i++) {
            int sym = 0;
            if (battlefield[i + MAP_WIDTH * j].partOfWall) {
                sym = 1;
            }
            else if (battlefield[i + MAP_WIDTH * j].occupiedByAlly) {
                sym = 2;
            }
            else if (battlefield[i + MAP_WIDTH * j].occupiedByEnemy) {
                sym = 3;
            }
            SDL_Rect fillRect = {i*4, j*4, 4, 4};
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
    for (int i = 0; i<10; i++) {
        for (int j = 0; j<10; j++) {
            if (getTile(x + i, y + j).checkForCollision()) {
                return true;
            }
        }
    }
    return false;
}
