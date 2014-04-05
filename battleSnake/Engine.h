//
//  Engine.h
//  GiocoLabInf
//
//  Created by Enrico Migliorini on 17/03/14.
//  Copyright (c) 2014 Enrico Migliorini. All rights reserved.
//

//==================================================//
//                                                  //
//  Engine works with the team and the narration    //
//                                                  //
//==================================================//

#ifndef __GiocoLabInf__Engine__
#define __GiocoLabInf__Engine__

#include <iostream>
#include <string>
#include <vector>
#include <map>

#include "PartyMember.h"
#include "Graphics.h"
#include "Level.h"

using namespace std;

class Engine {
private:
    string narration[3];    // the narrated parts
    const int MAX_TEAMSIZE = 4;
    void addTeamMember(Characters choice);
    Characters convertIntsToCharacter(int toConvert);    // cast is dumb, need int for cin
    map<string, Item> inventory;
    Direction moveBuffer[3];
    Event lastTriggered;
    
    vector<PartyMember> team;
    
public:
    Engine();
    void teamBuilder(Screens &lastDisplayed, bool &isLMD, bool &isND, Graphics graph);
    void printTeamStats();
    void moveTeamOnMap(Direction dest, Level currLevel);   // This must check the value of the destination tile, move the character both on the screen and on the map.
    void startLevel(Level currentLevel);
    void endLevel();
    
    void startBattle(Event battle);
    void endBattle(Event battle);
    void battleCommands();
    void setLastEvent(Event last);
    Event getLastEvent();
    void specialEvent(Event triggered);
    
    void obtainEquipment();
    void switchEquipment();
    void narrate(); // Different dialogues for different team leaders
    
    int start();
    
    //Here I write some graphic functions that have a huge benefit from being here, namely access to the inventory, the members and remove the need to include engine in Graphics. Graphics will just need to include the most basic functions.
    //Draws the whole team
    void drawTeam(Graphics graph);
    //Writes the selection of items. I will need many resources such as this. setView must be called on menu before this one.
    void printEquipMenu(SDL_Renderer* renderer, TTF_Font* font);
    //Writes the status menu.
    void printTeamStats(SDL_Renderer* renderer, TTF_Font* font);
    //Writes battle info. Alerts and narration just need to be printed on screen, sometimes by events.
    void printBattleInfo(SDL_Renderer* renderer, TTF_Font* font);
};

#endif /* defined(__GiocoLabInf__Engine__) */
