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
    powerups = 1000;
    currentPattern = FLOWER;
    lastTriggered = EVE_DEFAULT;
}

// This function must be checked to see if it works well for gregarious guys
void Engine::addFleetMember(Characters choice) {
    // Check if space is free
    int modX = 0, modY = 0;
    if (fleetsize != 0) {   // Must not check for first ship
        modX -= SPRITE_WIDTH;
        modY -= SPRITE_HEIGHT/2;
        // Magic numbers again. 4 was the TILE_WIDTH and TILE_HEIGHT
        if (fleet[fleetsize-1].getX() + modX > 189*4 || fleet[fleetsize-1].getX() + modX < 4 || fleet[fleetsize-1].getY() + modY > 139*4 || fleet[fleetsize-1].getY() + modY < 4) // If they are not in a valid zone
            return;
        modX += 2*SPRITE_WIDTH;
        if (fleet[fleetsize-1].getX() + modX > 189*4 || fleet[fleetsize-1].getX() + modX < 4 || fleet[fleetsize-1].getY() + modY > 139*4 || fleet[fleetsize-1].getY() + modY < 4) // If they are not in a valid zone
            return;
    }
    // Adds ships
    FleetMember* player;
    switch (choice) {
        case FIGHTER:
        {
            powerups -= 10;
            player = new FleetMember(2, 30, 10000, FIGHTER);
            break;
        }
        case CORVETTE:
        {
            powerups -= 20;
            player = new FleetMember(100, 50, 100, CORVETTE);
            break;
        }
        case FRIGATE:
        {
            powerups -= 30;
            player = new FleetMember(200, 100, 500, FRIGATE);
            break;
        }
        case DESTROYER:
        {
            powerups -= 40;
            player = new FleetMember(300, 100, 500, DESTROYER);
            break;
        }
        case CRUISER:
        {
            powerups -= 50;
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
        fleet[fleetsize].setX(400);
        fleet[fleetsize].setY(320);
    }
    else {
        if (fleetsize == 1) {
            fleet[fleetsize].setX(fleet[fleetsize].getX() - 40);
            fleet[fleetsize].setY(fleet[fleetsize].getY() - 20);
        }
        else if (fleetsize == 2) {
            fleet[fleetsize].setX(fleet[fleetsize].getX() + 40);
            fleet[fleetsize].setY(fleet[fleetsize].getY() - 20);
        }
    }
    fleetsize++;
    delete player;
    player = 0;
}

// I must be careful when I write the patterns. Consider a safeguard
void Engine::addEnemyFleetMember(double x, double y, Characters choice) {
    Enemy* opponent;
    switch (choice) {
        case FIGHTER:
        {
            opponent = new Enemy(50, 30, 100, EN_FIGHTER, {x, y});
            break;
        }
        case CORVETTE:
        {
            opponent = new Enemy(10, 50, 100, EN_CORVETTE, {x, y});
            break;
        }
        case FRIGATE:
        {
            opponent = new Enemy(200, 100, 500, EN_FRIGATE, {x, y});
            break;
        }
        case DESTROYER:
        {
            opponent = new Enemy(300, 100, 500, EN_DESTROYER, {x, y});
            break;
        }
        case CRUISER:
        {
            opponent = new Enemy(500, 150, 1000, EN_CRUISER, {x, y});
            break;
        }
        default:
            opponent = nullptr;
            cerr << "This was never meant to happen! Invalid fleet member!" << endl;
            break;
    }
    enemyFleet.push_back(*opponent);
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

void Engine::coordsOfNearestEnemy(int &x, int &y, int index) { // NOTE: this gives the GRAPHIC coordinate.
    Enemy nearest;
    double minDist = 999;
    for (int i = 0; i<enemysize; i++) {
        double currDist = sqrt(pow((enemyFleet[i].getCenterX() - fleet[index].getCenterX()), 2) + pow((enemyFleet[i].getCenterY() - fleet[index].getCenterY()), 2));
        if (currDist < minDist) {
            nearest = enemyFleet[i];
        }
    }
    x = nearest.getCenterX();
    y = nearest.getCenterY();
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

// This is the circle collider
bool Engine::laserIsHittingTarget(int index, Spaceship* &target) {
    bool retValue = false;
    if (lasersOnMap[index]->isSBA()) {
        for (int i = 0; i<enemysize; i++) {
            double dist = sqrt(pow((lasersOnMap[index]->getCentergX() - enemyFleet[i].getCenterX()), 2) + pow((lasersOnMap[index]->getCentergY() - enemyFleet[i].getCenterY()), 2));
            // Twenty is a magic number
            if (dist < 20) {
                target = &enemyFleet[i];
                retValue = true;
            }
        }
    }
    else {
        double dist = sqrt(pow((lasersOnMap[index]->getCentergX() - fleet[0].getCenterX()), 2) + pow((lasersOnMap[index]->getCentergY() - fleet[0].getCenterY()), 2));
        // Twelve is a magic number
        if (dist < 12) {
            target = &fleet[0];
            retValue = true;
        }
    }
    if (retValue == false) {
        target = nullptr;
    }
    return retValue;
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
// Here stood moveFleetOnMap. Let it not be forgotten, for the pain it brought was much.

// This one makes me move coherently the main antagonist, enemyfleet[0].
void Engine::moveEnemyOnMap(double directangle, int speed) {
    // for moving
    int tmpX = enemyFleet[0].getX();
    int tmpY = enemyFleet[0].getY();
    if (cos(directangle) > 0) {
        tmpX += SPRITE_WIDTH;
    }
    if (sin(directangle) < 0) {
        tmpY += SPRITE_HEIGHT;
    }
    tmpX += cos(directangle) * speed;
    tmpY -= sin(directangle) * speed;
    if (tmpX < 780 && tmpY < 580 && tmpX > 20 && tmpY > 20)   // Not moving through walls!
    {   // No control is needed here, I Tink (tinker tailor soldier sailor)
        double plusX = cos(directangle);
        double plusY = -(sin(directangle));
        plusX *= speed;
        plusY *= speed;
        enemyFleet[0].teleport(enemyFleet[0].getX() + doubleToInt(plusX), enemyFleet[0].getY() + doubleToInt(plusY));
    }
}

void Engine::spawnEnemy(int x, int y, Characters ch) {
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
    
    // Points are quite ok, although they should escalate somehow. Maybe give another variable. Powerups should be WAY more rare.
    powerups += 1;
    
    score += 100;   // This is why I don't need a addPoints func
}

void Engine::collectPowerups(int amount) {
    powerups += amount;
}

void Engine::setLastEvent(Event lastEvent) {
    lastTriggered = lastEvent;
}

int Engine::getRandInSpan(int lower, int upper) {
    if (lower>=upper) {
        return 0;
    }
    int result = rand() % (upper - lower +1) + lower;
    return result;
}

int Engine::doubleToInt(double input) {
    int tmp = input;
    double difference = input - static_cast<double>(tmp);
    if (difference > (0.5)) {
        tmp++;
    }
    return tmp;
}

void Engine::nextMove(int turn, Pattern patt, Graphics* graph) {
    // I may place this into a nice class
    switch (patt) {
        case FLOWER:
            if (turn == 0) {
                if (enemysize == 0)
                    spawnEnemy(380, 40, CORVETTE);
                patternTurnsHelper = 0;   // The pattern is nicer this way
            }
            else if (turn <= 30) {
                // TODO: Adjust values
                moveEnemyOnMap((pi*1.5), 3);
            }
            // I should make a function that prints warnings like "WARNING: TYPHOON MULTITARGET SYSTEM ENGAGED"
            if (turn >= 31 && turn % 10 == 1 && turn < 500) {
                if (enemyFleet[0].getHP() >= 0 && enemysize > 0) {
                    for (int i = 0; i<=20; i++) {
                        Laser* tmp = enemyFleet[0].shootLaser(i*pi/10 + patternTurnsHelper*pi/32, LASER_ENEMY);
                        tmp->setSpeed(8);
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
                            lasersOnMap[i]->setSpeed(6);
                        }
                    }
                    patternTurnsHelper++;
                }
            }
            else if (turn == 500) {
                for (int i = 0; i<lasersOnMap.size(); i++) {
                    if (!lasersOnMap[i]->isSBA()) {
                        lasersOnMap[i]->setSpeed(6);
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
            
            
        case TEST_TEST:
            
            if (turn == 0) {
                spawnEnemy(400, 80, CORVETTE);
                patternTurnsHelper = 0;   // The pattern is nicer this way
            }
            else if (turn == 40) {
                Laser* tmp = enemyFleet[0].shootLaser(0, LASER_ALLIED);
                tmp->setSpeed(2);
                lasersOnMap.push_back(tmp);
            }
            else {
                for (int i = 0; i<lasersOnMap.size(); i++) {
                    if (!lasersOnMap[i]->isSBA()) {
                        // Checking all the collisionboxes
                        /*
                        fillRect1 = {static_cast<int>(lasersOnMap[i]->getCentergX()), static_cast<int>(lasersOnMap[i]->getCentergY()), 1, 1};
                        fillRect2 = {static_cast<int>(lasersOnMap[i]->getgX() + (20*cos(lasersOnMap[i]->getAngle())) - (2*sin(lasersOnMap[i]->getAngle()))), static_cast<int>(lasersOnMap[i]->getgY() - (20*sin(lasersOnMap[i]->getAngle())) - (2*cos(lasersOnMap[i]->getAngle()))), 1, 1};
                        fillRect3 = {static_cast<int>(lasersOnMap[i]->getgX() + (20*cos(lasersOnMap[i]->getAngle())) + (8*sin(lasersOnMap[i]->getAngle())) - (2*sin(lasersOnMap[i]->getAngle()))), static_cast<int>(lasersOnMap[i]->getgY() - (20*sin(lasersOnMap[i]->getAngle())) + (8*cos(lasersOnMap[i]->getAngle())) - (2*cos(lasersOnMap[i]->getAngle()))), 1, 1};
                        fillRect4 = {static_cast<int>(lasersOnMap[i]->getgX() + (8*sin(lasersOnMap[i]->getAngle())) - (2*sin(lasersOnMap[i]->getAngle()))), static_cast<int>(lasersOnMap[i]->getgY() + (8*cos(lasersOnMap[i]->getAngle())) - (2*cos(lasersOnMap[i]->getAngle()))), 1, 1};
                         */
                    }
                }
            }
            
            break;
             
             
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
 * -- Abolish direction. Priority VIOLET
 * -- Move will obsolesce as soon as I make the ship follow the mouse. Make it so. Priority GREEN
 * -- Sometimes lasers deal no damage. Avoid this. Priority GREEN. Maybe done?
 * -- Reduce the hitbox of the player and other Touhou adjustments. Priority GREEN
 *
 * TODO:
 *
 * -- Circle collider. Priority VIOLET
 * -- Have the patterns work well and enemies move realistically. Priority BLUE
 * -- It pains my heart, but Tiles shall be deprecated. Priority BLUE
 * -- Safeguard adding new ships against going out of the borders. Priority YELLOW
 * -- Create menus, implement items. Priority YELLOW
 * -- Add little green/red health boxes over the ships, or life bars like in Touhou. Priority YELLOW
 * -- Make simple explode animations for lasers and ships. PRiority YELLOW
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
            powerups += 10;
            addFleetMember(FIGHTER);
            
            // The address of the enemy ship hit by lasers each turn
            Spaceship* hitTarget = nullptr;
            
            currentPattern = FLOWER;

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
                                if (powerups >= 10) {
                                    addFleetMember(FIGHTER);
                                }
                                break;
                            case SDLK_2:
                                if (powerups >= 20) {
                                    addFleetMember(CORVETTE);
                                }
                                break;
                            case SDLK_3:
                                if (powerups >= 30) {
                                    addFleetMember(FRIGATE);
                                }
                                break;
                            case SDLK_4:
                                if (powerups >= 40) {
                                    addFleetMember(DESTROYER);
                                }
                                break;
                            case SDLK_5:
                                if (powerups >= 50) {
                                    addFleetMember(CRUISER);
                                }
                                break;
                                */
                            case SDLK_ESCAPE:
                            {
                                quit = true;
                                break;
                            }
                            case SDLK_SPACE:    // For a triggerable breakpoint
                            {
                                Mix_PauseMusic();
                                SDL_Delay(1000);
                                cout << "Game Paused" << endl;
                                Mix_ResumeMusic();
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
                // Ths avoids me going out of screen
                if (tX > SCREEN_WIDTH-40) {
                    tX = SCREEN_WIDTH-40;
                }
                if (tX < 2) {
                    tX = 2;
                }
                if (tY > SCREEN_HEIGHT-40) {
                    tY = SCREEN_HEIGHT-40;
                }
                if (tY < 2) {
                    tY = 2;
                }
                
                fleet[0].teleport(tX, tY);
                
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
                graphEngine->printPowerups(powerups);
                
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
                
                if (turnCounter > 600) {
                    turnCounter = 0;
                }
                nextMove(turnCounter, currentPattern, graphEngine);
                
                // Damaging enemies
                bool explosionPlayed = false;
                for (int i = 0; i<static_cast<int>(lasersOnMap.size()); i++) {
                    hitTarget = nullptr;
                    
                    if (lasersOnMap[i]->isMFD()) {
                        delete(lasersOnMap[i]);
                        lasersOnMap.erase(lasersOnMap.begin()+i);
                        if (!explosionPlayed) {
                            Mix_PlayChannel(-1, graphEngine->getBoom(), 0);
                        }
                    }
                    else if (lasersOnMap[i]->isHittingWall()) {
                        delete(lasersOnMap[i]);
                        lasersOnMap.erase(lasersOnMap.begin()+i);
                    }
                    else if (laserIsHittingTarget(i, hitTarget)) {
                        if (hitTarget != nullptr){
                            hitTarget->takeDamage(lasersOnMap[i]->getPower());
                            lasersOnMap[i]->explode();
                        }
                        hitTarget = nullptr;
                    }
                }
                // Cleaning the lasers
                for (int i = 0; i<lasersOnMap.size(); i++) {
                    lasersOnMap[i]->travel();
                }
                // Killing the enemy // TO MODIFICATE
                if (enemyOnScreen == true && enemyFleet[0].getHP() <= 0) {
                    killEnemy(&(enemyFleet[0]));
                    hitTarget = nullptr;
                    enemyOnScreen = false;
                }
                if (fleet[0].getHP() <= 0) {
                    setLastEvent(GAME_LOST);
                }
                drawFleet(graphEngine);
                drawEnemyFleet(graphEngine);
                // Must render here else explosion will not be seen
                for (int i = 0; i<lasersOnMap.size(); i++) {
                    lasersOnMap[i]->drawOnScreen(graphEngine);
                }
                
                /*
                SDL_SetRenderDrawColor(graphEngine->getRenderer(), 255, 0, 0, 0);
                SDL_RenderFillRect(graphEngine->getRenderer(), &fillRect1);
                SDL_RenderFillRect(graphEngine->getRenderer(), &fillRect2);
                SDL_RenderFillRect(graphEngine->getRenderer(), &fillRect3);
                SDL_RenderFillRect(graphEngine->getRenderer(), &fillRect4);
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
