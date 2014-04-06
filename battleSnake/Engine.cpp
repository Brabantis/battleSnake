//
//  Engine.cpp
//  GiocoLabInf
//
//  Created by Enrico Migliorini on 17/03/14.
//  Copyright (c) 2014 Enrico Migliorini. All rights reserved.
//

#include <new>

#include "Engine.h"

Engine::Engine() {
    teamsize = 0;
    enemysize = 0;
    moveBuffer.push_back(NORTH);
    lastTriggered = EVE_DEFAULT;
}

// in addFleetMember I need some sort of control, to avoid having duplicate characters
void Engine::addFleetMember(Characters choice) {
    FleetMember* player;
    switch (choice) {
        case FIGHTER:
        {
            player = new Fighter("Fighter");
            break;
        }
        case CORVETTE:
        {
            
            player = new Corvette("Corvette");
            break;
        }
        case FRIGATE:
        {
            player = new Frigate("Frigate");
            break;
        }
        case DESTROYER:
        {
            player = new Destroyer("Destroyer");
            break;
        }
        case CRUISER:
        {
            player = new Cruiser("Cruiser");
            break;
        }
        default:
            player = nullptr;
            cerr << "This was never meant to happen! Invalid fleet member!" << endl;
            break;
    }
    fleet.push_back(*player);
    if (teamsize == 0) {
        fleet[teamsize].setX(5);
        fleet[teamsize].setY(5);
    }
    else {
        fleet[teamsize].setX(fleet[teamsize-1].getX());
        fleet[teamsize].setY(fleet[teamsize-1].getY());
        switch (fleet[teamsize-1].getDirection()) {
            case NORTH:
                fleet[teamsize].move(SOUTH);
                fleet[teamsize].setDirection(NORTH);
                break;
            case EAST:
                fleet[teamsize].move(WEST);
                fleet[teamsize].setDirection(EAST);
                break;
            case SOUTH:
                fleet[teamsize].move(NORTH);
                fleet[teamsize].setDirection(SOUTH);
                break;
            case WEST:
                fleet[teamsize].move(EAST);
                fleet[teamsize].setDirection(WEST);
                break;
            default:
                break;
        }
    }
    teamsize++;
    //moveBuffer.push_back(moveBuffer[teamsize-1]);
}

// I don't need a fleetbuilder

void Engine::drawFleet(Graphics graph) {
    for (int i = 0; i<teamsize; i++) {
        fleet[i].drawOnScene(graph);
    }
}

void Engine::drawEnemyFleet(Graphics graph) {
    for (int i = 0; i<enemysize; i++) {
        enemyFleet[i].drawEnemyOnScene(graph);
    }
}

void Engine::printFleetStats() {
    for (int i = 0; i<teamsize; i++) {
        cout << "Member in position " << i+1 << ": a " << fleet[i].getClass() << " called " << fleet[i].getName() << " who has these values for attack, defense and health: " << fleet[i].getAtk() << ", " << fleet[i].getDef() << ", " << fleet[i].getHP() << "." << endl;
    }
}

void Engine::moveFleetOnMap(Direction dest, Level currLevel) {
    int tmpX = fleet[0].getX();
    int tmpY = fleet[0].getY();
    switch (dest) {
        case NORTH:
            tmpY--;
            break;
        case EAST:
            tmpX++;
            break;
        case SOUTH:
            tmpY++;
            break;
        case WEST:
            tmpX--;
            break;
        default:
            cerr << "Invalid destination for team! How is this possible?" << endl;
            break;
    }
    short y = currLevel.getTileCode(tmpX, tmpY);
    switch (y) {
        case 0:
            cerr << "Accessed out of boundary zone! The character walked through walls!" << endl;
            break;
        case 1:
            break;
        case 2:
            fleet[0].move(dest);
            // Works like a charm, adding at the end and changing the buffer. But HOW COME the buffer works if it's always formed by only one element? For now, I'll accept it as a mystery
            for (int i = 1; i<teamsize; i++) {
                fleet[i].move(moveBuffer[i-1]);
            }
            for (int i = teamsize-1; i>0; i--) {
                moveBuffer[i] = moveBuffer[i-1];
            }
            moveBuffer[0] = dest;
            break;
        case 3:         // Different codes for different events
        case 4:
        case 5:
        case 6:
            fleet[0].move(dest);
            for (int i = 1; i<teamsize; i++) {
                fleet[i].move(moveBuffer[i-1]);
            }
            for (int i = teamsize-1; i>0; i--) {
                moveBuffer[i] = moveBuffer[i-1];
            }
            moveBuffer[0] = dest;
            // Commands are repeated, but either writing a function or adding a check boolean or two seems an overkill at this stage
            setLastEvent(currLevel.getEventFromCode(y));
            break;
        default:
            break;
    }
}

int Engine::getRandInSpan(int lower, int upper) {
    if (lower>=upper) {
        return 0;
    }
    int result = rand() % (upper - lower +1) + lower;
    return result;
}

void Engine::spawnEnemy(int x, int y, Level current) {
    // Some permission stuff forbids me from changing here the tile code.
    Enemy spawned(100, 100, 100, "Bob", {x, y, NORTH});
    enemyFleet.push_back(spawned);
    enemysize++;
}

void Engine::eatEnemy() {
    enemyFleet.pop_back();
    enemysize--;
    addFleetMember(FIGHTER);
}

void Engine::setLastEvent(Event lastEvent) {
    lastTriggered = lastEvent;
}

// THIS MIGHT BE USELESS, CHECK LATER
Event Engine::getLastEvent() {
    return lastTriggered;
}

// the main function, transferred in engine. Setting all the graphics stuff as children of Graphics and so on may allow me to make the functions slimmer
int Engine::start()
{
    srand ((int)time(NULL));
    Level currentLevel;
    Graphics graphEngine;
    
    int tmpx = 2, tmpy = 2;
    
    //Start up SDL and create window
	if( !graphEngine.init() )
	{
		cout << "Failed to initialize!" << endl;
	}
	else
	{
		//Load media
		if( !graphEngine.loadMedia())
		{
			cout << "Failed to load media!" << endl;
		}
		else
		{
            // Here go all the things I need to start up the game. Eventually they'll get into a new nice function.
			//Main loop flag
			bool quit = false;
            
			//Event handler
			SDL_Event e;
            
            //Background color
            SDL_SetRenderDrawColor(graphEngine.getRenderer(), 255, 255, 255, 1);
            
            //Reminder of the last displayed
            Screens gLastDisplayed = MAIN_CAMERA;

            // Moving direction
            Direction choice = NORTH;
            
            // Is there an enemy?
            bool enemyOnScreen = false;
            
            // First ship   THIS IS GOING TO BE A FUNCTION
            addFleetMember(FIGHTER);
            
            //First drawing
            SDL_RenderClear(graphEngine.getRenderer());
            graphEngine.setView(MAIN_CAMERA, gLastDisplayed);
            drawFleet(graphEngine);
            SDL_RenderPresent(graphEngine.getRenderer());
            
            // THIS BLOODY THING WAS SENDING MY CLOSE() INTO CONFUSION!!! HOW?
            // returning zero has no effect on this particular bug
            // Attempt #3
            
            /*
             SDL_Rect tempDest = {200, 100, 400, 100};
             Graphics::printTextOnScreen(gRenderer, "third attempt", &tempDest, gFont);
             SDL_RenderPresent(gRenderer);
             */
            
            // The funtion works, apparently. the program does NOT;
            
			while( !quit )
			{
				//Handle events on queue
				while( SDL_PollEvent( &e ) != 0 )
				{
					//User requests quit
					if( e.type == SDL_QUIT )
					{
						quit = true;
					}
                    //User presses a key
					else if( e.type == SDL_KEYDOWN )
					{
						switch( e.key.keysym.sym )
						{
							case SDLK_UP:
                                choice = NORTH;
                                break;
							case SDLK_DOWN:
                                choice = SOUTH;
                                break;
							case SDLK_LEFT:
                                choice = WEST;
                                break;
							case SDLK_RIGHT:
                                choice = EAST;
                            default:
                                break;
						}
                        
                        // APPARENTLY what is not on the renderer is destroyed when you present it. BAD thing, but I just need to use gLastDisplayed and the boolean sisters and redraw everything.
					}
                // Things over this brace are only invoked at the press of a button
				}
                // Here I invoke the functions I need each time
                switch (lastTriggered) {
                    case LEV_CLOSURE:
                        quit = true;
                        break;
                    case EAT_ENEMY: // When I'm on a tile with a trigger, the trigger is fired continuously
                        enemyOnScreen = false;
                        eatEnemy();
                        currentLevel.giveCodeToTile(tmpx, tmpy, WALK);  // If I can, I should include those in eatEnemy();
                        lastTriggered = EVE_DEFAULT;
                        tmpx++;
                        tmpy++;
                        break;
                    default:
                        break;
                }
                
                if (!enemyOnScreen) {
                    tmpx = getRandInSpan(2, 19);
                    tmpy = getRandInSpan(2, 14);
                    spawnEnemy(tmpx, tmpy, currentLevel);
                    currentLevel.giveCodeToTile(tmpx, tmpy, ENEMY_HERE);
                    enemyOnScreen = true;
                }
                
                graphEngine.setView(gLastDisplayed, gLastDisplayed);
                drawFleet(graphEngine);
                drawEnemyFleet(graphEngine);
                moveFleetOnMap(choice, currentLevel);
                SDL_RenderPresent(graphEngine.getRenderer());
                SDL_Delay(150);
			}   // Here ends the main loop
		}
	}
	//Free resources and close SDL
    graphEngine.close();
	return 0;
}
