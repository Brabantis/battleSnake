//
//  Engine.h
//  GiocoLabInf
//
//  Created by Enrico Migliorini on 17/03/14.
//  Copyright (c) 2014 Enrico Migliorini. All rights reserved.
//


//==================================================//
//                                                  //
//  Engine works with the fleet and the narration   //
//                                                  //
//==================================================//

#ifndef __GiocoLabInf__Engine__
#define __GiocoLabInf__Engine__

#include <deque>
#include <map>
#include <cmath>
#include <vector>

#include <stdlib.h>
#include <time.h>

#include "FleetMember.h"
#include "Graphics.h"
#include "Enemy.h"

using namespace std;

enum Pattern {
    MAIN_STAGE,
    BOSS_STAGE,
    TYPHOON,
    TEST_TEST
};

enum Event {
    ERR_LEVEL,
    GAME_LOST,
    MAIN_STAGE_CLEAR,
    BOSS_STAGE_CLEAR,
    EVE_DEFAULT
};

class Engine {
public:
    Engine();
    int start(Pattern patt);
    
    Event getLastEvent();
    void setLastEvent(Event last);
    Graphics* getGraphicsEngine();
    
    void narrate(Screens narrator, int phase);
    
private:
    int FPS;
    
    // To render in case of graphical debug needed.
    /*
    SDL_Rect fillRect1;
    SDL_Rect fillRect2;
    SDL_Rect fillRect3;
    SDL_Rect fillRect4;
    */
    
    Graphics* graphEngine;
    
    int patternTurnsHelper;
    
    int score;
    int kills;
    bool enemyOnScreen;
    
    // If I ever want to implement aiming back, like a powerup
    // bool areLasersAimed;

    Pattern currentPattern;
    
    Event lastTriggered;
    
    vector<Enemy*> enemyFleet;
    vector<Laser*> lasersOnMap;         // I need something made so that I can BOTH access elements in order AND delete one in every position (pop).
    // These are needed for managing multiple movements and KILLS
    FleetMember* protagonist;
        
    bool expiredEnemy0;
    bool expiredEnemy1;
    
    void coordsOfNearestEnemy(int &x, int &y, int index, bool shipSearch);
    void addFleetMember(Characters choice);
    void addEnemyFleetMember(double x, double y, Characters choice, float hM);
    void cleanEnemyFleet();
    void cleanLasers();
    
    void printHealth();
    
    void protagonistShootsAimedLaser();
    void protagonistShootsSingleLaser();
    
    bool laserIsHittingTarget(int index, Spaceship* &target);
    
    void nextMove(int turn, Pattern patt, Graphics* graph);
    
    void moveEnemyOnMap(double directangle, double speed, Spaceship* enemy);
    
    void mainMenu();                    // TODO: Select ship, buy kills

    // The functions that spawn enemies
    Characters intToCharacterConvert(int input);
    int getRandInSpan(int lower, int upper);
    void spawnEnemy(int x, int y, Characters ch, float healthMod);
    int doubleToInt(double input);

    void collectkills(int amount);
    
    void buyEquipment();        // TODO
    void loadEquipment();       // TODO
    
    // Here I write some graphic functions that have a huge benefit from being here, namely access to the inventory, the members and remove the need to include engine in Graphics. Graphics will just need to include the most basic functions.
    // Or, now that I switched the organization, I could give them a graph and let them do their magic. Will think later.
    // Draws the whole fleet
    void drawFleet(Graphics* graph);
    void drawEnemyFleet(Graphics* graph);
    // NOTE: These funcs have not be written. Their purpose is to write on screen dynamic info for the player.
    // Writes the selection of items. setView must be called on menu before this one.
    void printEquipMenu(SDL_Renderer* renderer, TTF_Font* font);
    // Writes the status menu.
    void printFleetStats(SDL_Renderer* renderer, TTF_Font* font);
    // Writes battle info. Alerts and narration just need to be printed on screen, sometimes by events.
    void printBattleInfo(SDL_Renderer* renderer, TTF_Font* font);
};

#endif /* defined(__GiocoLabInf__Engine__) */
