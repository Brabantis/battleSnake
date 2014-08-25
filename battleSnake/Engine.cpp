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
    currentPattern = FLOWER;
    moveBuffer.push_front(NORTH);
    lastTriggered = EVE_DEFAULT;
}

// I don't think that this is much useful anymoar.
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
            player = new FleetMember(2, 30, 100, FIGHTER);
            break;
        }
        case CORVETTE:
        {
            parts -= 20;
            player = new FleetMember(100, 50, 100, CORVETTE);
            break;
        }
        case FRIGATE:
        {
            parts -= 30;
            player = new FleetMember(200, 100, 500, FRIGATE);
            break;
        }
        case DESTROYER:
        {
            parts -= 40;
            player = new FleetMember(300, 100, 500, DESTROYER);
            break;
        }
        case CRUISER:
        {
            parts -= 50;
            player = new FleetMember(500, 150, 1000, CRUISER);
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
            opponent = new Enemy(50, 30, 100, EN_FIGHTER, {x, y, SOUTH});
            break;
        }
        case CORVETTE:
        {
            opponent = new Enemy(10, 50, 100, EN_CORVETTE, {x, y, SOUTH});
            break;
        }
        case FRIGATE:
        {
            opponent = new Enemy(200, 100, 500, EN_FRIGATE, {x, y, SOUTH});
            break;
        }
        case DESTROYER:
        {
            opponent = new Enemy(300, 100, 500, EN_DESTROYER, {x, y, SOUTH});
            break;
        }
        case CRUISER:
        {
            opponent = new Enemy(500, 150, 1000, EN_CRUISER, {x, y, SOUTH});
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

void Engine::coordsOfNearestEnemy(int &x, int &y, int index) { // NOTE: this gives the TILE coordinate.
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

void Engine::allyShootsAimedLaser(int index) {
    int tmpx = 0, tmpy = 0;
    coordsOfNearestEnemy(tmpx, tmpy, index);
    Laser* tmp = fleet[index].shootLaser(tmpx, tmpy, LASER_ALLIED);
    lasersOnMap.push_back(tmp);
}

void Engine::allyShootsSingleLaser(int index) {
    Laser* tmp = fleet[index].shootLaser(pi/2, LASER_ALLIED);
    lasersOnMap.push_back(tmp);
}

void Engine::getAllyOnMap(int x, int y, FleetMember* ship) {
    for (int i = 0; i<10; i++) {
        for (int j = 0; j<10; j++) {
            // This is how it works (on the notes of Karma Police)
            if ((i == 3 || i == 4) && (j == 3 || j == 4) && ship == &fleet[0]) {
                currentLevel->giveCodeToTile(x+i, y+j, ALLY, ship);
            }
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

// This whole function is the true evil in this program. And as it is an evil, it shall be vanquished FOR THE POWER OF GRAYSKULL!
// Seriously, now this should only move the gregarious ships together with the main one.
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
            if (y->checkForCollision()) {    // Works not.
                fleet[0].takeDamage(y->occupyingEnemy->getAtk());
                y->occupyingEnemy->takeDamage(10000);
            }
            // Works like a charm
            moveBuffer.pop_back();
            moveBuffer.push_front(dest);
            
            for (int i = 0; i<fleetsize; i++) {
                removeAllyFromMap(fleet[i].getX(), fleet[i].getY());
                fleet[i].move(moveBuffer[i*10]);
                getAllyOnMap(fleet[i].getX(), fleet[i].getY(), &(fleet[i]));
            }
        }
    }
    if (y->partOfWall == true) {
        setLastEvent(GAME_LOST);
    }
}

// This one makes me move coherently the main antagonist, enemyfleet[0].
void Engine::moveEnemyOnMap(Direction dest) {
    // for moving
    int tmpX = enemyFleet[0].getX();
    int tmpY = enemyFleet[0].getY();
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
    {   // No control is needed here, I Tink (tinker tailor soldier sailor)
        removeEnemyFromMap(enemyFleet[0].getX(), enemyFleet[0].getY());
        enemyFleet[0].move(dest);
        getEnemyOnMap(enemyFleet[0].getX(), enemyFleet[0].getY(), &enemyFleet[0]);
    }
}

void Engine::startLevel() {
    currentLevel = new Level();
}

void Engine::spawnEnemy(int x, int y, Characters ch) {
    // Some permission stuff forbids me from changing here the tile code.
    // addEnemyFleetMember(x, y, intToCharacterConvert(getRandInSpan(0, 4)));
    addEnemyFleetMember(x, y, ch);
    enemyOnScreen = true;
}

void Engine::killEnemy(Enemy* dead) {
    for (int i = 0; i<(enemysize); i++) {
        if (dead == &enemyFleet[i]) {
            enemyFleet.erase(enemyFleet.begin()+i);
        }
    }
    dead = nullptr;
    
    enemysize--;
    
    // Points are quite ok, although they should escalate somehow. Maybe give another variable. Parts should be WAY more rare.
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

void Engine::nextMove(int turn, Pattern patt, Graphics* graph) {
    // I may place this into a nice class
    switch (patt) {
        case FLOWER:
            if (turn == 0) {
                spawnEnemy(98, 10, CORVETTE);
                patternTurnsHelper = 0;   // The pattern is nicer this way
            }
            else if (turn <= 30) {
                moveEnemyOnMap(SOUTH);
            }
            // I should make a function that prints warnings like "WARNING: TYPHOON MULTITARGET SYSTEM ENGAGED"
            if (enemyFleet[0].getHP() >= 0) {
                if (turn >= 31 && turn % 10 == 1 && turn < 500) {
                    for (int i = 0; i<=20; i++) {
                        Laser* tmp = enemyFleet[0].shootLaser(i*pi/10 + patternTurnsHelper*pi/32, LASER_ENEMY);
                        lasersOnMap.push_back(tmp);
                    }
                    Mix_PlayChannel(-1, graph->getZap(), 0);
                    
                    for (int i = 0; i<lasersOnMap.size(); i++) {
                        if (lasersOnMap[i]->getTurnsInLife() == 10 && !lasersOnMap[i]->isSBA()) {
                            lasersOnMap[i]->setAngle(lasersOnMap[i]->getAngle()+(pi/2));
                        }
                        if (lasersOnMap[i]->getTurnsInLife() > 10 && lasersOnMap[i]->getTurnsInLife() <= 100 && !lasersOnMap[i]->isSBA()) {
                            lasersOnMap[i]->setAngle(lasersOnMap[i]->getAngle()+(pi/3));
                        }
                        if (lasersOnMap[i]->getTurnsInLife() == 110 && !lasersOnMap[i]->isSBA()) {
                            lasersOnMap[i]->setAngle(lasersOnMap[i]->getAngle()-(pi/2));
                            lasersOnMap[i]->setSpeed(5);
                        }
                    }
                    patternTurnsHelper++;
                }
                else if (turn == 500) {
                    for (int i = 0; i<lasersOnMap.size(); i++) {
                        if (!lasersOnMap[i]->isSBA()) {
                            lasersOnMap[i]->setSpeed(5);
                        }
                    }
                }
            }
            break;
            
            // Note: Shooting single-line rotating sunrays is cool, like if you changed the 20 in FLOWER wit a 0
        case TYPHOON:
            // This looks like a typhoon, given enough lasers. Might be impossible, though.
            for (int i = 0; i<lasersOnMap.size(); i++) {
                if (lasersOnMap[i]->getTurnsInLife() == 20 && !lasersOnMap[i]->isSBA()) {
                    lasersOnMap[i]->setAngle(lasersOnMap[i]->getAngle()+(pi/2));
                }
                if (lasersOnMap[i]->getTurnsInLife() > 20 && !lasersOnMap[i]->isSBA()) {
                    lasersOnMap[i]->setAngle(lasersOnMap[i]->getAngle()+(pi/16));
                }
            }
            break;
            
            /*
        case TEST_TEST:
            if (turn == 0) {
                spawnEnemy(98, 10, CORVETTE);
                patternTurnsHelper = 0;   // The pattern is nicer this way
            }
            else if (turn <= 30) {
                moveEnemyOnMap(SOUTH);
            }
            else if (turn == 40) {
                Laser* tmp = enemyFleet[0].shootLaser(pi/2, LASER_ALLIED);
                                lasersOnMap.push_back(tmp);
            }
            else {
                for (int i = 0; i<lasersOnMap.size(); i++) {
                    if (!lasersOnMap[i]->isSBA()) {
                        fillRect = {static_cast<int>(lasersOnMap[i]->getgX()), static_cast<int>(lasersOnMap[i]->getgY()), 1, 1};
                    }
                }
            }
            break;
             */
             
        default:
            break;
    }
}


// LIST OF STUFF TO DO
/*
 * DONEALREADY
 * -- Remove the leak. Priority ULTRAVIOLET. Apparently there is no leak anymore, since the memory usage is now stable on the period.
 * -- Use references as much as possible to improve speed. Check EVERY function. Priority BLUE.
 * -- Have the ships move correctly. Priority INDIGO.
 * -- Solve multiship problems like trying to spawn a ship in a non permitted zone or ships crashing when going opposite directions. Priority BLUE
 * -- Make it run faster. Priority GREEN. It was really really trivial. I feel stupid now.
 * -- Write the pattern manager and create some nice ways for enemies to shoot. Priority VIOLET
 * -- Have the ship move fast and fluid. Priority BLUE
 * -- Avoid EXC_BAD_ACCESS when one hits the wall. make movement impossible and/or free the mouse instead. Priority ULTRAVIOLET
 *
 * TODO:
 *
 * -- Abolish direction. Priority VIOLET
 * -- The lasers collide with their tail. Avoid this. IDEA: I can find the float position in the 4 angles and check collision for THOSE. Trigonometry incoming. Priority VIOLET
 * -- Have the patterns work well and enemies move realistically. Priority BLUE
 * -- Move will obsolesce as soon as I make the ship follow the mouse. Make it so. Priority GREEN
 * -- Sometimes lasers deal no damage. Avoid this. Priority GREEN. Maybe done?
 * -- Reduce the hitbox of the player and other Touhou adjustments. Priority GREEN
 * -- Create menus, implement items. Priority YELLOW
 * -- Add little green/red health boxes over the ships, or life bars like in Touhou. Priority YELLOW
 * -- Make simple explode animations. PRiority YELLOW
 * -- Rewrite everything packaging more nicely some code bundles and using the GameLoop design. Priority RED
 * -- Graphical improvements, such as an animated background. Priority INFRARED
 *
 * NOTE:
 * -- Having lasers turn at weird irregular angles creates nice patterns
 * -- A nice location for shooting lasers is (98, 40)
 * -- Spawning allies without putting them on map is a nice way to make them bulletproof, like those spheres Reimu has
 */

// the main function, transferred in engine. Setting all the graphics stuff as children of Graphics and so on allowed me to make the functions slimmer
int Engine::start()
{
    srand (static_cast<int>(time(NULL)));
    Graphics* graphEngine = new Graphics;
    startLevel();
    //int tmpx = 2, tmpy = 2;
    
    // For intermediate animations. RIGHT NOW ONLY FOR SHOOTING LASERS, since I avoided tweening
    int intervalLaserCounter = 0;
    int turnCounter = 0;
    
    // For managing the laser shooting
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
			// Main loop flag
			bool quit = false;
            
			// Event handler
			SDL_Event e;
            
            // Background color
            SDL_SetRenderDrawColor(graphEngine->getRenderer(), 255, 255, 255, 1);
            
            // Reminder of the last displayed
            Screens gLastDisplayed = MAIN_CAMERA;
            
            // Is there an enemy?
            enemyOnScreen = false;
            
            // First ship
            parts += 10;
            addFleetMember(FIGHTER);
            
            // The address of the enemy ship hit by lasers each turn
            Spaceship* hitTarget = nullptr;
            
            //First drawing
            SDL_RenderClear(graphEngine->getRenderer());
            graphEngine->setView(MAIN_CAMERA, gLastDisplayed);
            drawFleet(graphEngine);
            SDL_RenderPresent(graphEngine->getRenderer());
            SDL_Delay(300); // TO DECUPLICATE
            // Start the music
            Mix_PlayMusic(graphEngine->getMainStage(), -1);
            
			while( !quit )
			{
                // This is to achieve a nice stable framerate
                Uint32 time = SDL_GetTicks();
                
				//Handle events on queue
				while (SDL_PollEvent (&e) != 0)
				{
					//User requests quit
					if (e.type == SDL_QUIT)
					{
						quit = true;
					}
                    
                    // User presses a key
                    // Key input is deactivated right now.
					else if (e.type == SDL_KEYDOWN)
					{
						switch (e.key.keysym.sym)
						{
                                /*
							case SDLK_UP:
                                    moveFleetOnMap(NORTH);
                                break;
							case SDLK_DOWN:
                                    moveFleetOnMap(SOUTH);
                                break;
							case SDLK_LEFT:
                                    moveFleetOnMap(WEST);
                                break;
							case SDLK_RIGHT:
                                    moveFleetOnMap(EAST);
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
                                */
                            case SDLK_t:
                            {
                                if (SDL_ShowCursor(-1) == 0) {
                                    SDL_ShowCursor(1);
                                }
                                else {
                                    SDL_ShowCursor(0);
                                }
                                break;
                            }
                            case SDLK_ESCAPE:
                            {
                                quit = true;
                                break;
                            }
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
                
                // This moves the ship on the position of the mouse.
                // Must start later
                int tX = 0, tY = 0;
                SDL_GetMouseState(&tX, &tY);
                tX /= TILE_WIDTH;
                tY /= TILE_HEIGHT;
                // Ths avoids me going out of screen
                if (tX > (SCREEN_WIDTH/TILE_WIDTH)-10) {
                    tX = (SCREEN_WIDTH/TILE_WIDTH)-10;
                }
                if (tX < 2) {
                    tX = 2;
                }
                if (tY > (SCREEN_HEIGHT/TILE_HEIGHT)-10) {
                    tY = (SCREEN_HEIGHT/TILE_HEIGHT)-10;
                }
                if (tY < 2) {
                    tY = 2;
                }
                
                removeAllyFromMap(fleet[0].getX(), fleet[0].getY());
                fleet[0].teleport(tX, tY);
                getAllyOnMap(tX, tY, &(fleet[0]));
                
                // cout << fleet[0].getHP() << endl;
                
                // Spawning a new enemy
                /*
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
                 
                }*/
                // TODO: Regulate this treshold to achieve believable rate of fire
                if (!quit && intervalLaserCounter==5) {
                    intervalLaserCounter = 1;
                    if (lasercounter == 1) {
                        lasercounter --;
                        readyToFire = true;
                    }
                    else {
                        lasercounter++;
                    }
                }
                
                graphEngine->setView(gLastDisplayed, gLastDisplayed);
                
                // These two are no longer troublesome
                graphEngine->printScore(score);
                graphEngine->printParts(parts);
                
                if (lastTriggered == GAME_LOST) {
                    SDL_Delay(50); // TO DECUPLICATE
                    quit = true;
                }
                else if (lastTriggered == LEVEL_CLEAR) {
                    graphEngine->setView(GAME_WON, gLastDisplayed);
                    SDL_Delay(1000);
                    quit = true;
                }
                for (int i = 0; i<fleetsize; i++) {
                    if (readyToFire) { // I shall make the count start from a ertain turn
                        readyToFire = false;
                        // Contrarily to popular belief, lasers do NOT cause memory leaks.
                        // This means that the zeroeth element shoots.
                        allyShootsSingleLaser(0);
                    }
                }
                
                currentPattern = FLOWER;
                if (turnCounter > 600) {
                    turnCounter = 0;
                }
                nextMove(turnCounter, currentPattern, graphEngine);
                
                // Damaging enemies
                for (int i = 0; i<static_cast<int>(lasersOnMap.size()); i++) {
                    hitTarget = nullptr;
                    
                    if (lasersOnMap[i]->isHittingWall(currentLevel)) {
                        delete(lasersOnMap[i]);
                        lasersOnMap.erase(lasersOnMap.begin()+i);
                    }
                    else if (lasersOnMap[i]->isHittingTarget(currentLevel, hitTarget)) {
                        if (hitTarget != nullptr){
                            // TODO: Una nave occupa uno spazio 10X10. Un hitbox dovrebbe essere circa 4X4, o meglio 2X2. Quindi consiglio di controllare la x e la y del laser, e danneggiare se e solo se
                            hitTarget->takeDamage(lasersOnMap[i]->getPower());
                            delete(lasersOnMap[i]);
                            lasersOnMap.erase(lasersOnMap.begin()+i);
                        }
                        hitTarget = nullptr;
                        
                        // The center of the laser impact must be the center of the explosion
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
                    hitTarget = nullptr;
                    enemyOnScreen = false;
                }
                if (fleet[0].getHP() <= 0) {
                    setLastEvent(GAME_LOST);
                }
                drawFleet(graphEngine);
                drawEnemyFleet(graphEngine);
                
                /*
                SDL_SetRenderDrawColor(graphEngine->getRenderer(), 255, 0, 0, 0);
                SDL_RenderFillRect(graphEngine->getRenderer(), &fillRect);
                */
                
                intervalLaserCounter++;
                turnCounter++;
                SDL_RenderPresent(graphEngine->getRenderer());

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
