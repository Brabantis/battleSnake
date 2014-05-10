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
    score = 0;
    parts = 0;
    moveBuffer.push_back(NORTH);
    lastTriggered = EVE_DEFAULT;
}

void Engine::addFleetMember(Characters choice) {
    FleetMember* player;
    switch (choice) {
        case FIGHTER:
        {
            player = new FleetMember(100, 30, 100, FIGHTER);
            break;
        }
        case CORVETTE:
        {
            
            player = new FleetMember(150, 50, 100, CORVETTE);
            break;
        }
        case FRIGATE:
        {
            player = new FleetMember(300, 100, 500, FRIGATE);
            break;
        }
        case DESTROYER:
        {
            player = new FleetMember(500, 100, 500, DESTROYER);
            break;
        }
        case CRUISER:
        {
            player = new FleetMember(1000, 150, 1000, CRUISER);
            break;
        }
        default:
            player = nullptr;
            cerr << "This was never meant to happen! Invalid fleet member!" << endl;
            break;
    }
    fleet.push_back(*player);
    if (fleetsize == 0) {
        fleet[fleetsize].setX(100);
        fleet[fleetsize].setY(140);
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
    delete player;
    //moveBuffer.push_back(moveBuffer[fleetsize-1]);
}

void Engine::addEnemyFleetMember(int x, int y, Characters choice) {
    Enemy* opponent;
    switch (choice) {
        case FIGHTER:
        {
            opponent = new Enemy(100, 30, 100, EN_FIGHTER, {x, y, SOUTH});
            break;
        }
        case CORVETTE:
        {
            opponent = new Enemy(150, 50, 100, EN_CORVETTE, {x, y, SOUTH});
            break;
        }
        case FRIGATE:
        {
            opponent = new Enemy(300, 100, 500, EN_FRIGATE, {x, y, SOUTH});
            break;
        }
        case DESTROYER:
        {
            opponent = new Enemy(500, 100, 500, EN_DESTROYER, {x, y, SOUTH});
            break;
        }
        case CRUISER:
        {
            opponent = new Enemy(1000, 150, 1000, EN_CRUISER, {x, y, SOUTH});
            break;
        }
        default:
            opponent = nullptr;
            cerr << "This was never meant to happen! Invalid fleet member!" << endl;
            break;
    }
    enemyFleet.push_back(*opponent);
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

Direction Engine::intToDirectionConvert(int input) {
    Direction output = NORTH;
    switch (input) {
        case 0:
            output = NORTH;
            break;
        case 1:
            output = EAST;
            break;
        case 2:
            output = SOUTH;
            break;
        case 3:
            output = WEST;
            break;
    }
    return output;
}

void Engine::coordsOfNearestEnemy(int &x, int &y, int index) {  // NOTE: this gives the TILE coordinate.
    Enemy nearest;
    double minDist = 999;
    for (int i = 0; i<enemysize; i++) {
        double currDist = sqrt(pow((enemyFleet[i].getX() - fleet[index].getX()), 2) + pow((enemyFleet[i].getY() - fleet[index].getY()), 2));
        if (currDist < minDist) {
            nearest = enemyFleet[i];
        }
    }   // TODO: once i'm finished with the new Tile map, this might get incremented of 5 to give the canter and make it look more realistic
    x = nearest.getX();
    y = nearest.getY();
}

void Engine::addLaserToMap() {
    for (int i = 0; i<fleetsize; i++) {
        int tmpx = 0, tmpy = 0;
        coordsOfNearestEnemy(tmpx, tmpy, i);
        Laser tmp = fleet[i].shootLaser(tmpx, tmpy);
        lasersOnMap.push_back(tmp);
    }
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
        cout << "Member in position " << i+1 << ": a " << fleet[i].getClass() << " that has these values for attack, defense and health: " << fleet[i].getAtk() << ", " << fleet[i].getDef() << ", " << fleet[i].getHP() << "." << endl;
    }
}

void Engine::moveFleetOnMap(Direction dest) {
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
    // TODOTODO: This NEEDS a COMPLETE and TOTAL OVERHAUL, to check for all the 100 tiles. right now it's like it was, only redesigned
    Tile y = currentLevel.getTile(tmpX, tmpY);
    if (y.partOfWall == false)   // Not moving through walls!
    {
        if (y.occupiedByAlly == false) // WALKABLE // Do I really really need to check for this? If one hits with itself what should happen?
        {
            if (isOccupied(tmpX, tmpY, currentLevel)) {
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
        }
        if (y.occupiedByEnemy == true)
        {
            setLastEvent(currentLevel.getEventFromCode(ENEMY));
        }
    }
    if (y.occupiedByAlly == true || y.partOfWall == true) {
        setLastEvent(GAME_LOST);
    }
}

void Engine::startLevel() {
    // TODO: Un costruttore di livelli a partire dal codice
    currentLevel = *(new Level());
}

void Engine::spawnEnemy(int x, int y, Level current) {
    // Some permission stuff forbids me from changing here the tile code.
    addEnemyFleetMember(x, y, intToCharacterConvert(getRandInSpan(0, 4)));
}

void Engine::killEnemy() {
    enemyFleet.pop_back();
    enemysize--;
    // TEMPORARY
    parts += 1; // Instead of parts, I can use a credit system
    //
    score += 100;   // This is why I don't need a addPoints func
}

void Engine::collectParts(int amount) {
    parts += amount;
}

void Engine::setLastEvent(Event lastEvent) {
    lastTriggered = lastEvent;
}

bool Engine::isOccupied(int x, int y, Level curr) {
    bool result = false;
    for (int i = 0; i<fleetsize; i++) {
        if (fleet[i].getX() == x && fleet[i].getY() == y) {
            result = true;
        }
    }
    if (curr.getTile(x, y).partOfWall == true) {
        result = true;
    }
    return result;
}

int Engine::getRandInSpan(int lower, int upper) {
    if (lower>=upper) {
        return 0;
    }
    int result = rand() % (upper - lower +1) + lower;
    return result;
}

// THIS MIGHT BE USELESS, CHECK LATER
Event Engine::getLastEvent() {
    return lastTriggered;
}

// LIST OF STUFF
// What there is to do: Directions work, collision does NOT; the system sees a 30000 dimension map, but positions are still limited. Need to overhaul movement, directions and position. Need something like a "herearespaceships" that marks for collision 10x10 tiles starting from the upper left corner
// Things that are fucked up: Apparently the walls are the borders of the screen. Try to make a border around everything.

// the main function, transferred in engine. Setting all the graphics stuff as children of Graphics and so on allowed me to make the functions slimmer
int Engine::start()
{
    srand (static_cast<int>(time(NULL)));
    Graphics graphEngine;
    startLevel();
    int tmpx = 2, tmpy = 2;
    
    //Many counters for things that need to be done every few frames
    
    // For intermediate animations. RIGHT NOW ONLY FOR SHOOTING LASERS, since I avoided tweening
    int intervalCounter = 1;
    
    // For managing the laser shooting
    RateOfFire rof = MEDIUM;
    int lasercounter = 0;
    bool readyToFire = true;
    
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
            SDL_Delay(300); // TO DECUPLICATE
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
						{   // Potrebbe essere conveniente ruotare la texture e renderizzare appena un tasto viene premuto. Dovrebbe dare l'illusione di una maggior responsività
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
                                break;
                            case SDLK_1:
                                if (parts >= 10) {
                                    parts -= 10;
                                    addFleetMember(FIGHTER);
                                }
                                break;
                            case SDLK_2:
                                if (parts >= 20) {
                                    parts -= 20;
                                    addFleetMember(CORVETTE);
                                }
                                break;
                            case SDLK_3:
                                if (parts >= 30) {
                                    parts -= 30;
                                    addFleetMember(FRIGATE);
                                }
                                break;
                            case SDLK_4:
                                if (parts >= 40) {
                                    parts -= 40;
                                    addFleetMember(DESTROYER);
                                }
                                break;
                            case SDLK_5:
                                if (parts >= 50) {
                                    parts -= 50;
                                    addFleetMember(CRUISER);
                                }
                                break;
                            case SDLK_SPACE:    // For a triggerable breakpoint
                                cout << "Game Paused" << endl;
                                break;
                            default:
                                break;
						}
					}
                // Things over this brace are only invoked at the press of a button
				}
                // Here I invoke the functions I need each time
                
                if (!enemyOnScreen) {   // Spawning a new enemy
                    do {
                        tmpx = getRandInSpan(11, 190);
                        tmpy = getRandInSpan(11, 140);
                    }
                    while (isOccupied(tmpx, tmpy, currentLevel));
                    spawnEnemy(tmpx, tmpy, currentLevel);
                    currentLevel.giveCodeToTile(tmpx, tmpy, ENEMY);
                    enemyOnScreen = true;
                }
                // TODO: Regulate this treshold to achieve believable rate of fire
                if (!quit && intervalCounter==20) {
                    intervalCounter = 1;
                    if (lasercounter == rof) {
                        lasercounter -= rof;
                        readyToFire = true;
                    }
                    else {
                        lasercounter++;
                    }
                    moveFleetOnMap(choice); // I woulda coulda shoulda change this + drawfleet + drawEnemyFleet into something tweening
                    
                    // This TECHNICALLY works. But VERY technically. Like "it's horrible and totally not understandable and it fucks up collision"
                    /*
                    for (int i = 0; i<enemyFleet.size(); i++) {
                        int dir = getRandInSpan(0, 3);
                        enemyFleet[i].move(intToDirectionConvert(dir));
                        if (enemyFleet[i].getX() > 19 || enemyFleet[i].getX() < 2 ||
                            enemyFleet[i].getY() > 14 || enemyFleet[i].getY() < 2) {
                            enemyFleet[i].move(intToDirectionConvert((dir+2)%4));
                        }
                    }*/
                }
                graphEngine.setView(gLastDisplayed, gLastDisplayed);
                graphEngine.printScore(score);
                graphEngine.printParts(parts);
                
                switch (lastTriggered) {
                    case GAME_LOST:
                        SDL_Delay(50); // TO DECUPLICATE
                        quit = true;
                        break;
                    case EAT_ENEMY:
                        // THESE are the functions for destroying an enemy and will be eventually moved
                        // Now, I think that I should NOT write a destructor in order to remove effectively every trace of the enemy being, freeing the tiles that were occupied by it. RATHER, seeing how it should interact with the object currentLevel, rather than something depending on Spaceships, I should put that in killEnemy.
                        enemyOnScreen = false;
                        killEnemy();
                        currentLevel.giveCodeToTile(tmpx, tmpy, EMPTY);  // If I can, I should include those in killEnemy();
                        lastTriggered = EVE_DEFAULT;
                        tmpx++;
                        tmpy++;
                        break;
                        
                        break;
                    default:
                        break;
                }
                for (int i = 0; i<fleetsize; i++) {
                    if (readyToFire) {
                        readyToFire = false;
                        // Forse è meglio se spara una volta ogni due turni
                        addLaserToMap();
                    }
                    // Find a way to pop elements in the middle, since you must destroy the laser once it hits a target. Also, consider switching back to vector once you've found your way
                }
                for (int i = static_cast<int>(lasersOnMap.size()) - 1; i>=0; i--) {
                    if (lasersOnMap[i].isHittingEnemy(currentLevel)) {
                        lasersOnMap.erase(lasersOnMap.begin()+i);
                        // Create a function that changes for 1 frame the sprite to explosion, then removes the laser. A possibility is to set a bool markedForDeletion, that is set with an impact func that also sets the sprite to explode, damages the target and stops the travel. Then a sweeper function removes all the markedForDeletion. Also the hit ship must not move between the impact and the destruction. Check with the frame intervals
                    }
                    else if (lasersOnMap[i].isHittingWall(currentLevel)) {
                        lasersOnMap.erase(lasersOnMap.begin()+i);
                        
                    }
                }
                for (int i = 0; i<lasersOnMap.size(); i++) {
                    lasersOnMap[i].travel();
                    lasersOnMap[i].drawOnScreen(graphEngine);
                }
                drawFleet(graphEngine);
                drawEnemyFleet(graphEngine);
                intervalCounter++;
                // Right now it renders at about 20 FPS
                SDL_RenderPresent(graphEngine.getRenderer());
                SDL_Delay(DELAYFOURTH);
			}
            graphEngine.setView(GAME_OVER, gLastDisplayed);
            SDL_RenderPresent(graphEngine.getRenderer());
            SDL_Delay(200); // TO DECUPLICATE
            // Here ends the main loop
		}
	}
	//Free resources and close SDL
    graphEngine.close();
	return 0;
}
