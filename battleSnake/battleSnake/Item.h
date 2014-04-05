//
//  Item.h
//  GiocoLabInf
//
//  Created by Enrico Migliorini on 25/03/14.
//  Copyright (c) 2014 Enrico Migliorini. All rights reserved.
//

#ifndef __GiocoLabInf__Item__
#define __GiocoLabInf__Item__

#include <iostream>

enum Item_Type {
    WEAPON,
    ARMOR,
    ACCESSORY
};

class Item {
private:
    int atk_modifier, def_modifier, HP_modifier;
    Item_Type category;
    bool available;
public:
    Item(int am, int dm, int hm);
    void switchAvailability();
    int getAtkMod();
    int getDefMod();
    int getHPMod();
};

#endif /* defined(__GiocoLabInf__Item__) */
