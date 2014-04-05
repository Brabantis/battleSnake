//
//  Creature.cpp
//  GiocoLabInf
//
//  Created by Enrico Migliorini on 12/03/14.
//  Copyright (c) 2014 Enrico Migliorini. All rights reserved.

#include "Creature.h"

Creature::Creature(int ba, int bd, int bh, int lev, location pos, string crname) {
    base_atk = ba;
    base_def = bd;
    base_hp = bh;
    atk = base_atk;
    def = base_def;
    hp = base_hp;
    level = lev;
    poisoned = false;
    stunned = false;
    
    // CAREFUL!!! Here we are using SDL coordinate system, which is flipped around the x axis.
    position = pos;
    name = crname;
}

// I'll need to check in the game loop if the enemy is still alive.
void Creature::takeDamage(int damage) {
    // ANIMATE DAMAGE
    double percentage = 100/((double)def + 100);    // LoL calculation
    damage *= percentage;
    hp -= damage;
    cout << name << " subisce " << damage << " danni; HP rimanenti = " << hp << endl;
}

void Creature::getPoisoned() {
    poisoned = true;
}

void Creature::attack(Creature enemy, power strength) {
    // ATTACK ANIMATION
    enemy.takeDamage(atk*0.5*strength);
}

// WARNING WARNING CODE BLUE
// Drawing functions destroy rendered characters unless they are refreshed.
// This one looks like it's working, and it could even apply to battle scenes.
void Creature::drawOnScene(Graphics graph) {
    SDL_Rect renderZone = {(position.x-1) * SPRITE_WIDTH, (position.y-1) * SPRITE_HEIGHT, SPRITE_WIDTH, SPRITE_HEIGHT};
    SDL_RenderCopy(graph.getRenderer(), graph.getSprites(sprite, position.orient), 0, &renderZone);
}
// x-1 and y-1 because we are working on an array

string Creature::getName() {
    return name;
}

int Creature::getAtk() {
    return atk;
}

int Creature::getDef() {
    return def;
}

int Creature::getHP() {
    return hp;
}

int Creature::getX() {
    return position.x;
}

int Creature::getY() {
    return position.y;
}

Direction Creature::getDirection() {
    return position.orient;
}

void Creature::setX(int value) {
    position.x = value;
}

void Creature::setY(int value) {
    position.y = value;
}

void Creature::setDirection(Direction facing) {
    position.orient = facing;
}

Characters Creature::getSprite() {
    return sprite;
}
