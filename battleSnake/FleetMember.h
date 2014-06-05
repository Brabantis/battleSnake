//
//  FleetMember.h
//  GiocoLabInf
//
//  Created by Enrico Migliorini on 12/03/14.
//  Copyright (c) 2014 Enrico Migliorini. All rights reserved.
//


//==================================================//
//                                                  //
//       PartyMembers can move and level up.        //
//        Their sprites depend on the class         //
//==================================================//

#ifndef __GiocoLabInf__PartyMember__
#define __GiocoLabInf__PartyMember__

#include <iostream>
#include "Spaceship.h"
#include "Item.h"

using namespace std;

class FleetMember : public Spaceship {
private:
    // Find some clever way to use calculateStats just once, changing the atk, def and hp values as the fleetmember is created. The equipment will be selected from a menu.
    Item* weapon;
    Item* armor;
    Item* accessory;
    
public:
    FleetMember();
    FleetMember(int ba, int bd, int bh, Characters sprite, location pos = {0, 0, NORTH});
    string getClass();
    void equip(Item equip);
    void calculateStats();
    bool isEnemyColliding(Level lvl);
};

#endif /* defined(__GiocoLabInf__PartyMember__) */
