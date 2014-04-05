//
//  PartyMember.h
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
#include "Creature.h"
#include "Item.h"

using namespace std;

class PartyMember : public Creature {
private:
    int exp, next_level;
    Item* weapon;
    Item* armor;
    Item* accessory;
    
protected:
    string abilityList[4];
    
public:
    PartyMember();
    PartyMember(int ba, int bd, int bh, string plname = "Hero", location pos = {0, 0, NORTH});
    void levelUp();
    void wonCombat(int xp);
    void move(Direction dest);
    string getClass();
    void equip(Item equip);
    void calculateStats();
    string getAbilityName(int choice);
    
    virtual void firstAbility();
    virtual void secondAbility();
    virtual void thirdAbility();
    virtual void fourthAbility();
    
};

#endif /* defined(__GiocoLabInf__PartyMember__) */
