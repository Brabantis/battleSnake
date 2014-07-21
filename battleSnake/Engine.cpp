//
//  Engine.cpp
//  GiocoLabInf
//
//  Created by Enrico Migliorini on 17/03/14.
//  Copyright (c) 2014 Enrico Migliorini. All rights reserved.
//

#include "Engine.h"

Engine::Engine() {
    // Initializing things that give trouble if I don't initialize them
    FPS = 40;
    fleetsize = 0;
    enemysize = 0;
    score = 0;
    parts = 1000;
    moveBuffer.push_front(NORTH);
    lastTriggered = EVE_DEFAULT;
}

void Engine::addFleetMember(Characters choice) {
    // Check if space is free
    int modX = 0, modY = 0;
    if (fleetsize != 0) {   // Must not check for first ship
        switch (fleet[fleetsize-1].getDirection()) {
            case NORTH:
                modY = 10;
                break;
            case EAST:
                modX = -10;
                break;
            case SOUTH:
                modY = -10;
                break;
            case WEST:
                modX = 10;
                break;
            default:
                break;
        }
        if (fleet[fleetsize-1].getX() + modX > 189 || fleet[fleetsize-1].getX() + modX < 1 || fleet[fleetsize-1].getY() + modY > 139 || fleet[fleetsize-1].getY() + modY < 1) // If they are in a valid zone1
            return;
    }
    // Adds ships
    FleetMember* player;
    switch (choice) {
        case FIGHTER:
        {
            parts -= 10;
            player = new FleetMember(100, 30, 100, FIGHTER);
            break;
        }
        case CORVETTE:
        {
            parts -= 20;
            player = new FleetMember(150, 50, 100, CORVETTE);
            break;
        }
        case FRIGATE:
        {
            parts -= 30;
            player = new FleetMember(300, 100, 500, FRIGATE);
            break;
        }
        case DESTROYER:
        {
            parts -= 40;
            player = new FleetMember(500, 100, 500, DESTROYER);
            break;
        }
        case CRUISER:
        {
            parts -= 50;
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
        fleet[fleetsize].setY(80);
    }
    else {
        fleet[fleetsize].setX(fleet[fleetsize-1].getX());
        fleet[fleetsize].setY(fleet[fleetsize-1].getY());
        switch (fleet[fleetsize-1].getDirection()) {
            case NORTH:
                fleet[fleetsize].setY(fleet[fleetsize].getY() + 10);
                fleet[fleetsize].setDirection(NORTH);
                break;
            case EAST:
                fleet[fleetsize].setX(fleet[fleetsize].getX() - 10);
                fleet[fleetsize].setDirection(EAST);
                break;
            case SOUTH:
                fleet[fleetsize].setY(fleet[fleetsize].getY() - 10);
                fleet[fleetsize].setDirection(SOUTH);
                break;
            case WEST:
                fleet[fleetsize].setX(fleet[fleetsize].getX() + 10);
                fleet[fleetsize].setDirection(WEST);
                break;
            default:
                break;
        }
    }
    getAllyOnMap(fleet[fleetsize].getX(), fleet[fleetsize].getY(), &(fleet[fleetsize]));
    fleetsize++;
    if (fleetsize != 1) {
        for (int i = 0; i<10; i++) {
            moveBuffer.push_back(fleet[fleetsize-1].getDirection());
        }
    }
    delete player;
    player = 0;
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
    getEnemyOnMap(x, y, &(enemyFleet[enemysize]));
    enemysize++;
    delete opponent;
    opponent = 0;
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
    }
    x = nearest.getX();
    y = nearest.getY();
}

void Engine::addLaserToMap() {
    for (int i = 0; i<fleetsize; i++) {
        int tmpx = 0, tmpy = 0;
        coordsOfNearestEnemy(tmpx, tmpy, i);
        Laser* tmp = fleet[i].shootLaser(tmpx, tmpy);
        lasersOnMap.push_back(tmp);
    }
}

void Engine::getAllyOnMap(int x, int y, Spaceship* ship) {
    for (int i = 0; i<10; i++) {
        for (int j = 0; j<10; j++) {
            currentLevel->giveCodeToTile(x+i, y+j, ALLY, ship);
        }
    }
}

void Engine::getEnemyOnMap(int x, int y, Spaceship* ship) {
    for (int i = 0; i<10; i++) {
        for (int j = 0; j<10; j++) {
            currentLevel->giveCodeToTile(x+i, y+j, ENEMY, ship);
        }
    }
}

// This function becomes very beautiful for removing ships
void Engine::removeAllyFromMap(int x, int y) {
    for (int i = 0; i<10; i++) {
        for (int j = 0; j<10; j++) {
            currentLevel->giveCodeToTile(x+i, y+j, NOT_ALLY);
        }
    }
}

void Engine::removeEnemyFromMap(int x, int y) {
    for (int i = 0; i<10; i++) {
        for (int j = 0; j<10; j++) {
            currentLevel->giveCodeToTile(x+i, y+j, NOT_ENEMY);
        }
    }
}

void Engine::killShipsStartingWith(int index) {
    
}

void Engine::drawFleet(Graphics* graph) {
    for (int i = 0; i<fleetsize; i++) {
        fleet[i].drawOnScene(graph);
    }
}

void Engine::drawEnemyFleet(Graphics* graph) {
    for (int i = 0; i<enemysize; i++) {
        enemyFleet[i].drawOnScene(graph);
    }
}

void Engine::printFleetStats() {
    for (int i = 0; i<fleetsize; i++) {
        cout << "Member in position " << i+1 << ": a " << fleet[i].getClass() << " that has these values for attack, defense and health: " << fleet[i].getAtk() << ", " << fleet[i].getDef() << ", " << fleet[i].getHP() << "." << endl;
    }
}

// This whole function is the true evil in this program.
void Engine::moveFleetOnMap(Direction dest) {
    //for moving
    int tmpX = fleet[0].getX();
    int tmpY = fleet[0].getY();
    switch (dest) {
        case NORTH:
            tmpY--;
            break;
        case EAST:
            tmpX+= 11;
            break;
        case SOUTH:
            tmpY+= 11;
            break;
        case WEST:
            tmpX--;
            break;
        default:
            cerr << "Invalid destination for team! How is this possible?" << endl;
            break;
    }
    Tile* y = currentLevel->getTile(tmpX, tmpY);
    if (y->partOfWall == false)   // Not moving through walls!
    {
        if (/*!(y.occupiedByAlly)*/ true)
        {
            if (y->checkForWallCollision()) {
                setLastEvent(GAME_LOST);
            }
            if (y->checkForCollision()) {    // Non funge, uff.
                fleet[0].takeDamage(y->occupyingEnemy->getAtk());
                y->occupyingEnemy->takeDamage(1000000);
            }
            // Works like a charm
            moveBuffer.pop_back();
            moveBuffer.push_front(dest);
            
            for (int i = 0; i<fleetsize; i++) {
                removeAllyFromMap(fleet[i].getX(), fleet[i].getY());
                fleet[i].move(moveBuffer[i*10], currentLevel);
                getAllyOnMap(fleet[i].getX(), fleet[i].getY(), &(fleet[i]));
            }
        }
    }
    if (y->partOfWall == true) {
        setLastEvent(GAME_LOST);
    }
}

void Engine::startLevel() {
    currentLevel = new Level();
}

void Engine::spawnEnemy(int x, int y) {
    // Some permission stuff forbids me from changing here the tile code.
    // addEnemyFleetMember(x, y, intToCharacterConvert(getRandInSpan(0, 4)));
    addEnemyFleetMember(x, y, FIGHTER);
}

void Engine::killEnemy(Enemy* dead) {
    for (int i = 0; i<(enemysize); i++) {
        if (dead == &enemyFleet[i]) {
            enemyFleet.erase(enemyFleet.begin()+i);
        }
    }
    dead = nullptr;
    
    enemysize--;
    
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

bool Engine::isOccupied(int x, int y) {
    bool result = false;
    for (int i = 0; i<fleetsize; i++) {
        if ((x - fleet[i].getX())<10 && (x - fleet[i].getX())>0 && (y - fleet[i].getY())<10 && (y - fleet[i].getY())>0) {
            result = true;
        }
    }
    if (currentLevel->getTile(x, y)->partOfWall == true) {
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

// LIST OF STUFF TO DO
/*
 * MAYBEDONEALREADY
 * -- Remove the leak. Priority ULTRAVIOLET. Apparently there is no leak anymore, since the memory usage is now stable on the period.
 * -- Use references as much as possible to improve speed. Check EVERY function. Priority BLUE.
 * -- Have the ships move correctly. Priority INDIGO.
 * -- Solve multiship problems like trying to spawn a ship in a non permitted zone or ships crashing when going opposite directions. Priority BLUE
 * -- Make it run faster. Priority GREEN. It was really really trivial. I feel stupid now.
 *
 * TODO:
 *
 * -- Activate the collider for allied ships in a coherent way. Priority VIOLET
 * -- Add and remove multiple ships. Have them collide? Priority VIOLET
 * -- Make enemy ships move. Priority BLUE
 * -- Create menus, implement items. Priority YELLOW.
 * -- Rewrite everything packaging more nicely some code bundles and using the GameLoop design. Priority ORANGE.
 * -- Graphical improvements, such as an animated background. Priority INFRARED.
 *
 */

// the main function, transferred in engine. Setting all the graphics stuff as children of Graphics and so on allowed me to make the functions slimmer
int Engine::start()
{
    srand (static_cast<int>(time(NULL)));
    Graphics* graphEngine = new Graphics;
    startLevel();
    int tmpx = 2, tmpy = 2;
    
    // For intermediate animations. RIGHT NOW ONLY FOR SHOOTING LASERS, since I avoided tweening
    int intervalCounter = 1;
    
    // For managing the laser shooting
    RateOfFire rof = MEDIUM;
    int lasercounter = 0;
    bool readyToFire = true;
    
    //Start up SDL and create window
	if( !graphEngine->init() )
	{
		cout << "Failed to initialize!" << endl;
	}
	else
	{
		//Load media
		if( !graphEngine->loadMedia())
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
            SDL_SetRenderDrawColor(graphEngine->getRenderer(), 255, 255, 255, 1);
            
            //Reminder of the last displayed
            Screens gLastDisplayed = MAIN_CAMERA;
            
            // Moving direction
            Direction choice = NORTH;
            
            // Is there an enemy?
            bool enemyOnScreen = false;
            
            // First ship
            parts += 10;
            addFleetMember(FIGHTER);
            
            // The address of the enemy ship hit by lasers each turn
            Spaceship* hitEnemy = nullptr;
            
            //First drawing
            SDL_RenderClear(graphEngine->getRenderer());
            graphEngine->setView(MAIN_CAMERA, gLastDisplayed);
            drawFleet(graphEngine);
            SDL_RenderPresent(graphEngine->getRenderer());
            SDL_Delay(300); // TO DECUPLICATE
			while( !quit )
			{
                // This is to achieve a nice stable framerate
                Uint32 time = SDL_GetTicks();
                
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
                                break;
                            case SDLK_1:
                                if (parts >= 10) {
                                    addFleetMember(FIGHTER);
                                }
                                break;
                            case SDLK_2:
                                if (parts >= 20) {
                                    addFleetMember(CORVETTE);
                                }
                                break;
                            case SDLK_3:
                                if (parts >= 30) {
                                    addFleetMember(FRIGATE);
                                }
                                break;
                            case SDLK_4:
                                if (parts >= 40) {
                                    addFleetMember(DESTROYER);
                                }
                                break;
                            case SDLK_5:
                                if (parts >= 50) {
                                    addFleetMember(CRUISER);
                                }
                                break;
                            case SDLK_SPACE:    // For a triggerable breakpoint
                            {
                                currentLevel->printMap(graphEngine);
                                SDL_Delay(3000);
                                cout << "Game Paused" << endl;
                                break;
                            }
                            default:
                                break;
						}
					}
                    // Things over this brace are only invoked at the press of a button
				}
                // Here I invoke the functions I need each time
                
                // Spawning a new enemy
                if (!enemyOnScreen) {
                    bool validLocation = false;
                    do {
                        validLocation = true;
                        tmpx = getRandInSpan(21, 180);
                        tmpy = getRandInSpan(21, 130);
                        // This should make it spawn far enough from the player and his ships. SHOULD.
                        for (int i = -9; i<9; i++) {
                            for (int j = -9; j<9; j++) {
                                if (currentLevel->thereIsCollision(tmpx+i, tmpy+j)) {
                                    validLocation = false;
                                }
                            }
                        }
                    }
                    while (isOccupied(tmpx, tmpy) && !validLocation);
                    spawnEnemy(tmpx, tmpy);
                    currentLevel->giveCodeToTile(tmpx, tmpy, ENEMY);
                    enemyOnScreen = true;
                }
                // TODO: Regulate this treshold to achieve believable rate of fire
                if (!quit && intervalCounter==10) {
                    intervalCounter = 1;
                    if (lasercounter == rof) {
                        lasercounter -= rof;
                        readyToFire = true;
                    }
                    else {
                        lasercounter++;
                    }
                    // I woulda coulda shoulda change this + drawfleet + drawEnemyFleet into something tweening
                }
                // I can set the speed by having it move every x turns.
                moveFleetOnMap(choice);
                graphEngine->setView(gLastDisplayed, gLastDisplayed);
                
                // These two are no longer troublesome
                graphEngine->printScore(score);
                graphEngine->printParts(parts);
                
                if (lastTriggered == GAME_LOST) {
                    SDL_Delay(50); // TO DECUPLICATE
                    quit = true;
                }
                for (int i = 0; i<fleetsize; i++) {
                    if (readyToFire) {
                        readyToFire = false;
                        // Forse è meglio se spara una volta ogni due turni
                        // Contrarily to popular belief, lasers do NOT cause memory leaks.
                        addLaserToMap();
                    }
                    // Find a way to pop elements in the middle, since you must destroy the laser once it hits a target. Also, consider switching back to vector once you've found your way
                }
                
                // Damaging enemies
                for (int i = 0; i<static_cast<int>(lasersOnMap.size()); i++) {
                    hitEnemy = nullptr;
                    if (lasersOnMap[i]->isHittingWall(currentLevel)) {
                        delete(lasersOnMap[i]);
                        lasersOnMap.erase(lasersOnMap.begin()+i);
                    }
                    
                    else if (lasersOnMap[i]->isHittingEnemy(currentLevel, hitEnemy)) {
                        // Apparently isHittingEnemy nullifies tiles. This is apparent since I start with all tiles clear. Maybe the problem is in removeAllyFromMap, though I don't see why and HOW.
                        // The problem seems to be somewhere in the Laser class.
                        if (hitEnemy != nullptr)
                            hitEnemy->takeDamage(lasersOnMap[i]->getPower());
                        hitEnemy = nullptr;
                        
                        // The center of the laser impact must be the center of the explosion
                        delete(lasersOnMap[i]);
                        lasersOnMap.erase(lasersOnMap.begin()+i);
                        
                        // Create a function that changes for 1 frame the sprite to explosion, then removes the laser. A possibility is to set a bool markedForDeletion, that is set with an impact func that also sets the sprite to explode, damages the target and stops the travel. Then a sweeper function removes all the markedForDeletion. Also the hit ship must not move between the impact and the destruction. Check with the frame intervals
                    }
                }
                // Cleaning the lasers
                for (int i = 0; i<lasersOnMap.size(); i++) {
                    lasersOnMap[i]->travel();
                    lasersOnMap[i]->drawOnScreen(graphEngine);
                }
                // Killing the enemy // TO MODIFICATE
                if (enemyOnScreen == true && enemyFleet[0].getHP() <= 0) {
                    removeEnemyFromMap(enemyFleet[0].getX(), enemyFleet[0].getY());
                    killEnemy(&(enemyFleet[0]));
                    hitEnemy = nullptr;
                    enemyOnScreen = false;
                }
                drawFleet(graphEngine);
                drawEnemyFleet(graphEngine);
                intervalCounter++;
                SDL_RenderPresent(graphEngine->getRenderer());
                // 20 is 1000/FPS
                if((1000/FPS)>(SDL_GetTicks()-time))
                {
                    SDL_Delay((1000/FPS)-(SDL_GetTicks()-time)); //SDL_Delay pauses the execution.
                }
            }
            graphEngine->setView(GAME_OVER, gLastDisplayed);
            SDL_RenderPresent(graphEngine->getRenderer());
            SDL_Delay(200); // TO DECUPLICATE
            // Here ends the main loop
        }
    }
    //Free resources and close SDL
    graphEngine->close();
    return 0;
}
