//
//  PartyMember.cpp
//  GiocoLabInf
//
//  Created by Enrico Migliorini on 12/03/14.
//  Copyright (c) 2014 Enrico Migliorini. All rights reserved.
//

#include "PartyMember.h"
#include "Creature.h"
#include "Item.h"

PartyMember::PartyMember() {
}

PartyMember::PartyMember(int ba, int bd, int bh, string plname, location pos) : Creature(ba, bd, bh, 1, pos, plname) {
    level = 1;
    exp = 0;
    next_level = 1000;
    // The sprite will be left uninitialized if the PartyMember constructor is used instead of the character class one
}

void PartyMember::levelUp() {
    level += 1;         // LevelUp = 20% increase
    base_atk *= 1.2;
    base_def *= 1.2;
    base_hp *= 1.2;
    cout << name << " sale al livello " << level << endl;
}

void PartyMember::wonCombat(int xp) {
    // VICTORY FANFARE
    atk = max_atk;
    def = max_def;
    hp = max_hp;
    cout << "All damage and ailments cured" << endl;
    exp += xp;
    if (exp >= next_level) {
        exp -= next_level;
        next_level += 1000;
        levelUp();
    }
}

string PartyMember::getClass() {
    string value;
    switch (sprite) {
        case YOUNGSTER:
            value = "Youngster";
            break;
        case OLD_LADY:
            value = "Old bag";
            break;
        case HIPPIE:
            value = "Hippie";
            break;
        case BULLY:
            value = "Bully";
            break;
        case VANDAL:
            value = "Vandal";
            break;
        case NERD:
            value = "Nerd";
            break;
        case KARATEKA:
            value = "Karateka";
            break;
        case SNOB:
            value = "Snob";
            break;
        case SPRITES_TOTAL:
            value = "NOT A VALID CLASS";
            break;
    }
    return value;
}

void PartyMember::move(Direction dest) {
    switch (dest) {
        case NORTH:
            position.y -= 1;
            break;
        case EAST:
            position.x += 1;
            break;
        case SOUTH:
            position.y += 1;
            break;
        case WEST:
            position.x -= 1;
            break;
        default:
            break;
    }
    position.orient = dest;
}

string PartyMember::getAbilityName(int choice) {
    return abilityList[choice - 1];
}

void PartyMember::firstAbility() {
    
}

void PartyMember::secondAbility() {
    
}

void PartyMember::thirdAbility() {
    
}

void PartyMember::fourthAbility() {
    
}