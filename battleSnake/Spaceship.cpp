//
//  Spaceship.cpp
//  GiocoLabInf
//
//  Created by Enrico Migliorini on 12/03/14.
//  Copyright (c) 2014 Enrico Migliorini. All rights reserved.

#include "Spaceship.h"

Spaceship::Spaceship(int ba, int bd, int bh, Characters spritesrc, location pos) {
    atk = ba;
    def = bd;
    hp = bh;
    sprite = spritesrc;
    // CAREFUL!!! Here we are using SDL coordinate system, which is flipped around the x axis.
    position = pos;
}

// I'll need to check in the game loop if the enemy is still alive.
void Spaceship::takeDamage(int damage) {
    // ANIMATE DAMAGE
    double percentage = 100/((double)def + 100);    // LoL calculation
    damage *= percentage;
    hp -= damage;
}

Laser Spaceship::shootLaser(int xdest, int ydest){
    // Shoots a MOTHERFRIGGIN' LAZERBEAM to a tile. If it collides with an enemy, it damages it. Else, it goes till it hits a wall.
    double angle = 0;
    if (xdest != position.x) {
        double cosfact = (ydest - position.y);
        double sinfact = (xdest - position.x);
        angle = atan(cosfact / sinfact);
        if (sinfact <= 0) {
            angle -= pi;
        }
    }
    else {
        if (ydest > position.y) {
            angle = pi/2;
        }
        else {
            angle = -pi/2;
        }
    }
    Laser tmp(atk, (position.x-1) * TILE_WIDTH + 20, (position.y-1) * TILE_HEIGHT + 20, -angle, LASER_BASIC);
    return tmp;
}

void Spaceship::move(Direction dest) {
    switch (dest) {
        case NORTH:
            position.y--;
            break;
        case EAST:
            position.x++;
            break;
        case SOUTH:
            position.y++;
            break;
        case WEST:
            position.x--;
            break;
        default:
            break;
    }
    position.orient = dest;
}

void Spaceship::explode() {
    // KABLEW!!! HAHAHAHAHAHA
}

// WARNING WARNING CODE BLUE
// Drawing functions destroy rendered characters unless they are refreshed.
// This one looks like it's working, and it could even apply to battle scenes.
void Spaceship::drawOnScene(Graphics graph) {
    int rotation = 0;
    switch (position.orient) {
        case NORTH:
            rotation = 0;
            break;
        case EAST:
            rotation = 90;
            break;
        case SOUTH:
            rotation = 180;
            break;
        case WEST:
            rotation = 270;
            break;
        default:
            break;
    }
    // To make them only move by one tile at the time. TODO: check if I should rather move them by 2, 5 or so 
    SDL_Rect renderZone = {(position.x-1) * TILE_WIDTH, (position.y-1) * TILE_HEIGHT, SPRITE_WIDTH, SPRITE_HEIGHT};
    SDL_RenderCopyEx(graph.getRenderer(), graph.getSprite(sprite), 0, &renderZone, rotation, 0, SDL_FLIP_NONE);
}
// x-1 and y-1 because we are working on an array

int Spaceship::getAtk() {
    return atk;
}

int Spaceship::getDef() {
    return def;
}

int Spaceship::getHP() {
    return hp;
}

int Spaceship::getX() {
    return position.x;
}

int Spaceship::getY() {
    return position.y;
}

Direction Spaceship::getDirection() {
    return position.orient;
}

void Spaceship::setX(int value) {
    position.x = value;
}

void Spaceship::setY(int value) {
    position.y = value;
}

void Spaceship::setDirection(Direction facing) {
    position.orient = facing;
}

Characters Spaceship::getSprite() {
    return sprite;
}
