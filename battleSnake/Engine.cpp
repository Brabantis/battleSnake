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
    // Initializing things that give trouble if I don't initialize them
    fleetsize = 0;
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
    if (fleetsize == 0) {
        fleet[fleetsize].setX(10);
        fleet[fleetsize].setY(14);
    }
    else {
        fleet[fleetsize].setX(fleet[fleetsize-1].getX());
        fleet[fleetsize].setY(fleet[fleetsize-1].getY());
        switch (fleet[fleetsize-1].getDirection()) {
            case NORTH:
                fleet[fleetsize].move(SOUTH);
                fleet[fleetsize].setDirection(NORTH);
                break;
            case EAST:
                fleet[fleetsize].move(WEST);
                fleet[fleetsize].setDirection(EAST);
                break;
            case SOUTH:
                fleet[fleetsize].move(NORTH);
                fleet[fleetsize].setDirection(SOUTH);
                break;
            case WEST:
                fleet[fleetsize].move(EAST);
                fleet[fleetsize].setDirection(WEST);
                break;
            default:
                break;
        }
    }
    fleetsize++;
    //moveBuffer.push_back(moveBuffer[fleetsize-1]);
}

void Engine::addEnemyFleetMember(int x, int y, Characters choice) {
    Enemy* player;
    switch (choice) {
        case FIGHTER:
        {
            player = new EnFighter("EnFighter");
            break;
        }
        case CORVETTE:
        {
            
            player = new EnCorvette("EnCorvette");
            break;
        }
        case FRIGATE:
        {
            player = new EnFrigate("EnFrigate");
            break;
        }
        case DESTROYER:
        {
            player = new EnDestroyer("EnDestroyer");
            break;
        }
        case CRUISER:
        {
            player = new EnCruiser("EnCruiser");
            break;
        }
        default:
            player = nullptr;
            cerr << "This was never meant to happen! Invalid fleet member!" << endl;
            break;
    }
    enemyFleet.push_back(*player);
    enemyFleet[enemysize].setX(x);
    enemyFleet[enemysize].setY(y);
    enemysize++;
}

// Apparently something bad happens when you use an enum from another file
Characters Engine::intToCharacterConvert(int input) {
    Characters output = FIGHTER;
    switch (input) {
        case 0:
            output = FIGHTER;
            break;
        case 1:
            output = CORVETTE;
            break;
        case 2:
            output = FRIGATE;
            break;
        case 3:
            output = DESTROYER;
            break;
        case 4:
            output = CRUISER;
            break;
        default:
            break;
    }
    return output;
}

void Engine::drawFleet(Graphics graph) {
    for (int i = 0; i<fleetsize; i++) {
        fleet[i].drawOnScene(graph);
    }
}

void Engine::drawEnemyFleet(Graphics graph) {
    for (int i = 0; i<enemysize; i++) {
        enemyFleet[i].drawOnScene(graph);
    }
}

void Engine::printFleetStats() {
    for (int i = 0; i<fleetsize; i++) {
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
            if (isOccupied(tmpX, tmpY)) {
                setLastEvent(GAME_LOST);
            }
            fleet[0].move(dest);
            // Works like a charm, adding at the end and changing the buffer. But HOW COME the buffer works if it's always formed by only one element? For now, I'll accept it as a mystery
            for (int i = 1; i<fleetsize; i++) {
                fleet[i].move(moveBuffer[i-1]);
            }
            for (int i = fleetsize-1; i>0; i--) {
                moveBuffer[i] = moveBuffer[i-1];
            }
            moveBuffer[0] = dest;
            break;
        case 3:         // Different codes for different events
        case 4:
        case 5:
        case 6:
            if (isOccupied(tmpX, tmpY)) {
                setLastEvent(GAME_LOST);
            }
            fleet[0].move(dest);
            for (int i = 1; i<fleetsize; i++) {
                fleet[i].move(moveBuffer[i-1]);
            }
            for (int i = fleetsize-1; i>0; i--) {
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
    addEnemyFleetMember(x, y, intToCharacterConvert(getRandInSpan(0, 4)));
}

void Engine::eatEnemy() {
    enemyFleet.pop_back();
    enemysize--;
    addFleetMember(intToCharacterConvert(getRandInSpan(0, 4)));
}

void Engine::setLastEvent(Event lastEvent) {
    lastTriggered = lastEvent;
}

bool Engine::isOccupied(int x, int y) {
    bool result = false;
    for (int i = 0; i<fleetsize; i++) {
        if (fleet[i].getX() == x && fleet[i].getY() == y) {
            result = true;
        }
    }
    return result;
}

// THIS MIGHT BE USELESS, CHECK LATER
Event Engine::getLastEvent() {
    return lastTriggered;
}

// the main function, transferred in engine. Setting all the graphics stuff as children of Graphics and so on allowed me to make the functions slimmer
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
            
            // First ship
            addFleetMember(FIGHTER);
            
            //First drawing
            SDL_RenderClear(graphEngine.getRenderer());
            graphEngine.setView(MAIN_CAMERA, gLastDisplayed);
            drawFleet(graphEngine);
            SDL_RenderPresent(graphEngine.getRenderer());
            SDL_Delay(3000);
            
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
                                if (fleet[0].getDirection() != SOUTH) {
                                    choice = NORTH;
                                }
                                break;
							case SDLK_DOWN:
                                if (fleet[0].getDirection() != NORTH) {
                                    choice = SOUTH;
                                }
                                break;
							case SDLK_LEFT:
                                if (fleet[0].getDirection() != EAST) {
                                    choice = WEST;
                                }
                                break;
							case SDLK_RIGHT:
                                if (fleet[0].getDirection() != WEST) {
                                    choice = EAST;
                                }
                            default:
                                break;
						}
                        
                        // APPARENTLY what is not on the renderer is destroyed when you present it. BAD thing, but I just need to use gLastDisplayed and the boolean sisters and redraw everything.
					}
                // Things over this brace are only invoked at the press of a button
				}
                // Here I invoke the functions I need each time
                
                moveFleetOnMap(choice, currentLevel);
                switch (lastTriggered) {
                    case GAME_LOST:
                        SDL_Delay(500);
                        quit = true;
                        break;
                    case EAT_ENEMY: // When I'm on a tile with a trigger, the trigger is fired continuously, beware
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
                    do {
                        tmpx = getRandInSpan(2, 19);
                        tmpy = getRandInSpan(2, 14);
                    }
                    while (isOccupied(tmpx, tmpy));
                    spawnEnemy(tmpx, tmpy, currentLevel);
                    currentLevel.giveCodeToTile(tmpx, tmpy, ENEMY_HERE);
                    enemyOnScreen = true;
                }
                if (!quit) {
                    graphEngine.setView(gLastDisplayed, gLastDisplayed);
                    drawFleet(graphEngine);
                    drawEnemyFleet(graphEngine);
                    SDL_RenderPresent(graphEngine.getRenderer());
                    SDL_Delay(150);
                }
			}
            graphEngine.setView(GAME_OVER, gLastDisplayed);
            SDL_RenderPresent(graphEngine.getRenderer());
            SDL_Delay(2000);
            // Here ends the main loop
		}
	}
	//Free resources and close SDL
    graphEngine.close();
	return 0;
}
