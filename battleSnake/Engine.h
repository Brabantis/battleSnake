//
//  Engine.h
//  GiocoLabInf
//
//  Created by Enrico Migliorini on 17/03/14.
//  Copyright (c) 2014 Enrico Migliorini. All rights reserved.
//

//==================================================//
//                                                  //
//  Engine works with the fleet and the narration    //
//                                                  //
//==================================================//

#ifndef __GiocoLabInf__Engine__
#define __GiocoLabInf__Engine__

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <cmath>

#include <stdlib.h>
#include <time.h>

#include "FleetMember.h"
#include "Graphics.h"
#include "Level.h"
#include "Enemy.h"

using namespace std;

const int DELAYFOURTH = 50;

enum RateOfFire {
    UNUSED_1,
    HIGH,
    MEDIUM,
    UNUSED_2,
    LOW
};

class Engine {
public:
    Engine();
    int start();

private:
    int fleetsize;
    int enemysize;
    int score;
    int parts;
    Level currentLevel;
    
    Event lastTriggered;
    
    map<string, Item> inventory;        // As of now, not implemented
    vector<Direction> moveBuffer;
    vector<FleetMember> fleet;
    vector<Enemy> enemyFleet;   // Using the Enemy class allows me to create any kind of enemy, either alien or human or whatever, I just need to create a new object. I guess i COULD just make a better constructor.
    vector<Laser> lasersOnMap;   // I need something made so that I can BOTH access elements in order AND delete one in every position (pop). If there isn't one, I'll just have to create it myself.
    
    void coordsOfNearestEnemy(int &x, int &y, int index);
    void addFleetMember(Characters choice);
    void addEnemyFleetMember(int x, int y, Characters choice);
    void fleetBuilder(Screens &lastDisplayed, Graphics graph);
    void addLaserToMap();
    
    void printFleetStats();
    void moveFleetOnMap(Direction dest);
    
    void mainMenu();                    // TODO: Selezione nave, acquisto potenziamenti
    void startLevel();
    void endLevel();                    // TODO: finisce la missione dopo un po' di punti.
    
    // The functions that spawn enemies
    Characters intToCharacterConvert(int input);
    Direction intToDirectionConvert(int input);
    int getRandInSpan(int lower, int upper);
    void spawnEnemy(int x, int y, Level current);
    
    void killEnemy();   // TO ADJUST
    void collectParts(int amount);
    
    bool isOccupied(int x, int y, Level curr);
    
    void setLastEvent(Event last);
    Event getLastEvent();
    
    void buyEquipment();        // TODO
    void loadEquipment();       // TODO
    
    // Here I write some graphic functions that have a huge benefit from being here, namely access to the inventory, the members and remove the need to include engine in Graphics. Graphics will just need to include the most basic functions.
    // Or, now that I switched the organization, I could give them a graph and let them do their magic. Will think later.
    // Draws the whole fleet
    void drawFleet(Graphics graph);
    void drawEnemyFleet(Graphics graph);
    //Writes the selection of items. I will need many resources such as this. setView must be called on menu before this one.
    void printEquipMenu(SDL_Renderer* renderer, TTF_Font* font);
    //Writes the status menu.
    void printFleetStats(SDL_Renderer* renderer, TTF_Font* font);
    //Writes battle info. Alerts and narration just need to be printed on screen, sometimes by events.
    void printBattleInfo(SDL_Renderer* renderer, TTF_Font* font);
};

#endif /* defined(__GiocoLabInf__Engine__) */
