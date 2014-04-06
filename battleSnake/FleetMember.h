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
    Item* weapon;
    Item* armor;
    Item* accessory;
    
public:
    FleetMember();
    FleetMember(int ba, int bd, int bh, string plname = "Hero", location pos = {0, 0, NORTH});
    void move(Direction dest);
    string getClass();
    void equip(Item equip);
    void calculateStats();
};

#endif /* defined(__GiocoLabInf__PartyMember__) */
