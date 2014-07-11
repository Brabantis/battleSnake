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
            currentLevel.giveCodeToTile(x+i, y+j, ALLY, ship);
        }
    }
}

void Engine::getEnemyOnMap(int x, int y, Spaceship* ship) {
    for (int i = 0; i<10; i++) {
        for (int j = 0; j<10; j++) {
            currentLevel.giveCodeToTile(x+i, y+j, ENEMY, ship);
        }
    }
}

// This function becomes very beautiful for removing ships
void Engine::removeAllyFromMap(int x, int y) {
    for (int i = 0; i<10; i++) {
        for (int j = 0; j<10; j++) {
            currentLevel.giveCodeToTile(x+i, y+j, NOT_ALLY);
        }
    }
}

void Engine::removeEnemyFromMap(int x, int y) {
    for (int i = 0; i<10; i++) {
        for (int j = 0; j<10; j++) {
            currentLevel.giveCodeToTile(x+i, y+j, NOT_ENEMY);
        }
    }
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
    
    Tile y = currentLevel.getTile(tmpX, tmpY);
    if (y.partOfWall == false)   // Not moving through walls!
    {
        if (/*!(y.occupiedByAlly)*/ true) // WALKABLE
            // The little problem here is that it checks for the tile of destination, which may be part of himself. Which means that it could never ever ever go left or down.
        {
            // Collision works not with the left border. Why Elmo Why Wait But This Stop This Stop Sign.
            if (currentLevel.thereIsCollision(tmpX, tmpY)) {
                setLastEvent(GAME_LOST);
            }
            
            // NOTE: This is a horrible thing that hurts my pride but I need to solve the problem that the collider has when touching the lower bound
            if (tmpY > 139) {
                setLastEvent(GAME_LOST);
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
    if (y.partOfWall == true) {
        setLastEvent(GAME_LOST);
    }
}

void Engine::startLevel() {
    /*
    // TODO: Un costruttore di livelli a partire dal codice
    currentLevel = Level();
 */
}

void Engine::spawnEnemy(int x, int y, Level &current) {
    // Some permission stuff forbids me from changing here the tile code.
    // addEnemyFleetMember(x, y, intToCharacterConvert(getRandInSpan(0, 4)));
    addEnemyFleetMember(x, y, FIGHTER);
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

bool Engine::isOccupied(int x, int y) {
    bool result = false;
    for (int i = 0; i<fleetsize; i++) {
        if ((x - fleet[i].getX())<10 && (x - fleet[i].getX())>0 && (y - fleet[i].getY())<10 && (y - fleet[i].getY())>0) {
            result = true;
        }
    }
    if (currentLevel.getTile(x, y).partOfWall == true) {
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
 *
 * TODO:
 *
 * -- Activate the collider for allied ships in a coherent way. Priority VIOLET
 * -- Make enemy ships move. Priority BLUE
 * -- Make it run faster. Priority GREEN. SPECIAL: Still in the dark as to HOW. It is the graphic that slows, I tried removing the delay, then unfocusing the windows, and speed went up the axis. Priority temporarily set to INFRAFRED, need to ask for further instructions.
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
            addFleetMember(FIGHTER);
            
            // The address of the enemy ship hit by lasers each turn
            Spaceship* hitEnemy = 0;
            
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
                                currentLevel.printMap(graphEngine);
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
                
                if (!enemyOnScreen) {   // Spawning a new enemy
                    bool validLocation = false;
                    do {
                        validLocation = false;
                        tmpx = getRandInSpan(11, 190);
                        tmpy = getRandInSpan(11, 140);
                        if (currentLevel.thereIsCollision(tmpx, tmpy) == false) {
                            validLocation = true;
                        }
                    }
                    while (isOccupied(tmpx, tmpy) && !validLocation);
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
                    
                    // This TECHNICALLY works. But VERY technically. Like "it's horrible and totally not understandable and it fucks up collision". Look later when I can collide safely
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
                graphEngine->setView(gLastDisplayed, gLastDisplayed);
                
                // These two are no longer troublesome
                graphEngine->printScore(score);
                graphEngine->printParts(parts);
                
                switch (lastTriggered) {
                    case GAME_LOST:
                        SDL_Delay(50); // TO DECUPLICATE
                        quit = true;
                        break;
                    case KILL_ENEMY:
                        // THESE are the functions for destroying an enemy and will be eventually moved
                        // Now, I think that I should NOT write a destructor in order to remove effectively every trace of the enemy being, freeing the tiles that were occupied by it. RATHER, seeing how it should interact with the object currentLevel, rather than something depending on Spaceships, I should put that in killEnemy.
                        // Currently, an enemy eaten causes a hell of a lot of respawns and so on. This is because I am not freeing the tiles. Remedy ASAP.
                        enemyOnScreen = false;
                        removeEnemyFromMap(enemyFleet[enemysize-1].getX(), enemyFleet[enemysize-1].getY());
                        killEnemy();
                        lastTriggered = EVE_DEFAULT;
                        tmpx++;
                        tmpy++;
                        break;
                    default:
                        break;
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
                // Now properly giving damage.
                for (int i = static_cast<int>(lasersOnMap.size()) - 1; i>=0; i--) {
                    if (lasersOnMap[i]->isHittingEnemy(currentLevel, hitEnemy)) {
                        if (hitEnemy != 0)
                            hitEnemy->takeDamage(lasersOnMap[i]->getPower());
                        delete(lasersOnMap[i]);
                        lasersOnMap.erase(lasersOnMap.begin()+i);
                        
                        // Create a function that changes for 1 frame the sprite to explosion, then removes the laser. A possibility is to set a bool markedForDeletion, that is set with an impact func that also sets the sprite to explode, damages the target and stops the travel. Then a sweeper function removes all the markedForDeletion. Also the hit ship must not move between the impact and the destruction. Check with the frame intervals
                    }
                    else if (lasersOnMap[i]->isHittingWall(currentLevel)) {
                        delete(lasersOnMap[i]);
                        lasersOnMap.erase(lasersOnMap.begin()+i);
                    }
                }
                // TODO: check all the enemies
                if (enemyOnScreen == true && enemyFleet[0].getHP() <= 0) {
                    setLastEvent(KILL_ENEMY);
                }
                
                for (int i = 0; i<lasersOnMap.size(); i++) {
                    lasersOnMap[i]->travel();
                    lasersOnMap[i]->drawOnScreen(graphEngine);
                }
                drawFleet(graphEngine);
                drawEnemyFleet(graphEngine);
                intervalCounter++;
                SDL_RenderPresent(graphEngine->getRenderer());
                // 20 is 1000/FPS
                if(20>(SDL_GetTicks()-time))
                {
                    SDL_Delay(20-(SDL_GetTicks()-time)); //SDL_Delay pauses the execution.
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
