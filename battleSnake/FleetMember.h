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

#include "Spaceship.h"

using namespace std;

class FleetMember : public Spaceship {
private:
    
public:
    FleetMember();
    FleetMember(int ba, int bd, int bh, Characters sprite, location pos = {0, 0});
    string getClass();
    void calculateStats();
};

#endif /* defined(__GiocoLabInf__PartyMember__) */
