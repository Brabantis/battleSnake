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

// This way I don't need to override the method for friends and foes
Laser* Spaceship::shootLaser(int xdest, int ydest, OtherSprites sprt){
    // Shoots a MOTHERFRIGGIN' LAZERBEAM to a tile. If it collides with an enemy, it damages it. Else, it goes till it hits a wall.
    double angle = 0;
    if (xdest != position.x) {
        double sinfact = (ydest - position.y - SPRITE_HEIGHT/2);
        double cosfact = (xdest - position.x - SPRITE_WIDTH/2);
        angle = atan2(sinfact, cosfact);
    }
    else {
        if (ydest > position.y) {
            angle = pi/2;
        }
        else {
            angle = -pi/2;
        }
    }
    Laser* tmp = new Laser(atk, (position.x-1) + SPRITE_WIDTH/2, (position.y-1) + SPRITE_HEIGHT/2, -angle, isAllied, sprt);
    return tmp;
}

Laser* Spaceship::shootLaser(double angle, OtherSprites sprt) {
    Laser* tmp = new Laser(atk, (position.x-1) + SPRITE_WIDTH/2, (position.y-1) + SPRITE_HEIGHT/2, angle, isAllied, sprt);
    return tmp;
}

void Spaceship::teleport(int dX, int dY) {
    position.x = dX;
    position.y = dY;
}

void Spaceship::explode() {
    // KABLEW!!! HAHAHAHAHAHA
}

void Spaceship::drawOnScene(Graphics* graph) {
    // To make them only move by one tile at the time.
    SDL_Rect renderZone = {static_cast<int>((position.x-1)), static_cast<int>((position.y-1)), SPRITE_WIDTH, SPRITE_HEIGHT};
    SDL_RenderCopy(graph->getRenderer(), graph->getSprite(sprite), 0, &renderZone);
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

double Spaceship::getX() {
    return position.x;
}

double Spaceship::getY() {
    return position.y;
}

double Spaceship::getCenterX() {
    return (position.x) + 20;
}

double Spaceship::getCenterY() {
    return (position.y) + 20;
}

void Spaceship::setX(int value) {
    position.x = value;
}

void Spaceship::setY(int value) {
    position.y = value;
}

Characters Spaceship::getSprite() {
    return sprite;
}

void Spaceship::setSprite(Characters spritesrc) {
    sprite = spritesrc;
}
