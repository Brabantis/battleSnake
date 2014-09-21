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
    score = 0;
    kills = 0;
    lastTriggered = EVE_DEFAULT;
    //areLasersAimed = false;
    graphEngine = new Graphics;
}

// This function must be checked to see if it works well for gregarious guys
void Engine::addProtagonist() {
    // Adds ships

    protagonist = new FleetMember(20, 0, 1000, FIGHTER);
    protagonist->setX(400);
    protagonist->setY(320);
}

// I must be careful when I write the patterns. Consider a safeguard
void Engine::addEnemyFleetMember(double x, double y, Characters choice, float hM) {
    Enemy* opponent;
    switch (choice) {
            // Values are to be adjusted (thank you, Captain Obvious)
        case EN_FIGHTER:
        {
            opponent = new Enemy(200, 50, 500*hM, EN_FIGHTER, {x, y});
            break;
        }
        case EN_CORVETTE:
        {
            opponent = new Enemy(300, 50, 2500, EN_CORVETTE, {x, y});
            break;
        }
        default:
            opponent = nullptr;
            cerr << "This was never meant to happen! Invalid fleet member!" << endl;
            break;
    }
    enemyFleet.push_back(opponent);
}

void Engine::cleanEnemyFleet() {
    while (enemyFleet.size()) {
        delete enemyFleet[0];
        enemyFleet.erase(enemyFleet.begin());
    }
    enemyOnScreen = false;
}

void Engine::cleanLasers() {
    while (lasersOnMap.size()) {
        delete lasersOnMap[0];
        lasersOnMap.erase(lasersOnMap.begin());
    }
}

void Engine::waitForConfirmation() {
    bool confGot = false;
    SDL_Event e;
    while (!confGot) {
        while (SDL_PollEvent (&e) != 0)
        {
            if (e.type == SDL_QUIT || e.type == SDL_MOUSEBUTTONDOWN || e.type == SDL_KEYDOWN) {
                confGot = true;
            }
        }
    }
}

void Engine::coordsOfNearestEnemy(int &x, int &y, int index, bool shipSearch) { // NOTE: this gives the GRAPHIC coordinate.
    Enemy* nearest = nullptr;
    double minDist = 999;
    for (int i = 0; i<enemyFleet.size(); i++) {
        if (enemyFleet[i]->isAlive()) {
            double currDist;
            if (shipSearch) {
                currDist = sqrt(pow((enemyFleet[i]->getCenterX() - protagonist->getCenterX()), 2) + pow((enemyFleet[i]->getCenterY() - protagonist->getCenterY()), 2));
            }
            else {
                currDist = sqrt(pow((enemyFleet[i]->getCenterX() - lasersOnMap[index]->getCentergX()), 2) + pow((enemyFleet[i]->getCenterY() - lasersOnMap[index]->getCentergY()), 2));
            }
            if (currDist < minDist) {
                nearest = enemyFleet[i];
                minDist = currDist;
            }
        }
    }
    if (nearest) {
        x = nearest->getCenterX();
        y = nearest->getCenterY();
    }
}

// Aimed lasers make it too easy, since this is way easier than Touhou, but if I ever want them back, here they are.
/*
void Engine::protagonistShootsAimedLaser() {
    int tmpx = 0, tmpy = 0;
    if (!areLasersAimed) {
        areLasersAimed = true;
    }
    // This func is only called if (enemyOnScreen) so it is safe
    // 0 is a fine falue since index is only used for lasers
    coordsOfNearestEnemy(tmpx, tmpy, 0, true);
    Laser* tmp = protagonist->shootLaser(tmpx, tmpy, LASER_ALLIED);
    lasersOnMap.push_back(tmp);
}
 */

void Engine::protagonistShootsSingleLaser() {
    Laser* tmp = protagonist->shootLaser(pi/2, LASER_ALLIED);
    tmp->setSpeed(20);
    lasersOnMap.push_back(tmp);
}

// This is the circle collider
bool Engine::laserIsHittingTarget(int index, Spaceship* &target) {
    bool retValue = false;
    if (lasersOnMap[index]->isSBA()) {
        for (int i = 0; i<enemyFleet.size(); i++) {
            if (enemyFleet[i]->isAlive()) {
                double dist = sqrt(pow((lasersOnMap[index]->getCentergX() - enemyFleet[i]->getCenterX()), 2) + pow((lasersOnMap[index]->getCentergY() - enemyFleet[i]->getCenterY()), 2));
                // Twenty is a magic number
                if (dist < 20) {
                    target = enemyFleet[i];
                    retValue = true;
                    score += 50;
                }
            }
        }
    }
    else {
        double dist = sqrt(pow((lasersOnMap[index]->getCentergX() - protagonist->getCenterX()), 2) + pow((lasersOnMap[index]->getCentergY() - protagonist->getCenterY()), 2));
        // Twelve is a magic number
        if (dist < 12) {
            target = protagonist;
            retValue = true;
        }
        if (dist >= 12 && dist <= 16) {
            // Graze bonus
            score += 250;
        }
    }
    if (retValue == false) {
        target = nullptr;
    }
    return retValue;
}

void Engine::drawFleet(Graphics* graph) {
    protagonist->drawOnScene(graph);
}

void Engine::drawEnemyFleet(Graphics* graph) {
    for (int i = 0; i<enemyFleet.size(); i++) {
        if (enemyFleet[i]->isAlive()) {
            enemyFleet[i]->drawOnScene(graph);
        }
    }
}


// This whole function is the true evil in this program. And as it is an evil, it shall be vanquished FOR THE POWER OF GRAYSKULL!
// Here stood moveFleetOnMap. Let it not be forgotten, for the pain it brought was much.

// SOMETHING DOES NOT WORK IF IT MOVES LEFT AND UP
// This one makes me move coherently the antagonists, enemyfleet[index].
void Engine::moveEnemyOnMap(double directangle, double speed, Spaceship* enemy) {
    // for moving
    double tmpX = (*enemy).getX();
    double tmpY = (*enemy).getY();
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
        int addendX = doubleToInt(plusX);
        int addendY = doubleToInt(plusY);
        (*enemy).teleport((*enemy).getX() + addendX, (*enemy).getY() + addendY);
    }
}

void Engine::spawnEnemy(int x, int y, Characters ch, float healthMod) {
    addEnemyFleetMember(x, y, ch, healthMod);
    enemyOnScreen = true;
}

void Engine::setLastEvent(Event lastEvent) {
    lastTriggered = lastEvent;
}

Event Engine::getLastEvent() {
    return lastTriggered;
}

Graphics* Engine::getGraphicsEngine() {
    return graphEngine;
}

int Engine::getScore() {
    return score;
}

int Engine::getKills() {
    return kills;
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
    if (difference < (-0.5)) {
        tmp--;
    }
    return tmp;
}

void Engine::printHealth() {
    for (int i = 0; i < 10; i++) {
        SDL_Rect destrect = {350 + 20*i, 10, 20, 30};
        if (protagonist->getHP() > i*100) {
            SDL_SetRenderDrawColor(graphEngine->getRenderer(), 0, 255, 0, 0);
        }
        else {
            SDL_SetRenderDrawColor(graphEngine->getRenderer(), 255, 0, 0, 0);
        }
        SDL_RenderFillRect(graphEngine->getRenderer(), &destrect);
    }
}

void Engine::narrate(Screens narrator, int phase) {
    bool exit = false;
    graphEngine->setView(narrator);
    string dialStr[3];
    switch (phase) {
        case 0:
            dialStr[0] = "Here you will crush the rebellion!";
            dialStr[1] = "Onward, soldier, exterminate them!";
            dialStr[2] = "Show no mercy! Search and destroy!";
            break;
        case 1:
            dialStr[0] = "You have outlived your usefulness.";
            dialStr[1] = "The reward for this shall be mine.";
            dialStr[2] = "Goodbye, soldier, see you in Hell!";
            break;
        case 2:
            dialStr[0] = "Betray? The Admiral? Unbelievable!";
            dialStr[1] = "Still, he did it and you saved us.";
            dialStr[2] = "Congratulations to you... Admiral.";
            break;
        default:
            break;
    }
    SDL_Rect destRect = {150, 450, 500, 30};
    graphEngine->printTextOnScreen(dialStr[0], &destRect);
    destRect = {150, 480, 500, 30};
    graphEngine->printTextOnScreen(dialStr[1], &destRect);
    destRect = {150, 510, 500, 30};
    graphEngine->printTextOnScreen(dialStr[2], &destRect);
    SDL_RenderPresent(graphEngine->getRenderer());
    SDL_Event ev;
    while (!exit) {
        while (SDL_PollEvent (&ev) != 0)
        {
            if (ev.type == SDL_MOUSEBUTTONDOWN || (ev.type == SDL_KEYDOWN && ev.key.keysym.sym == SDLK_ESCAPE)) {
                exit = true;
            }
        }
    }
}

void Engine::cleanStats() {
    score = 0;
    kills = 0;
}

void Engine::nextMove(int turn, Pattern patt, Graphics* graph) {
    // Big and ugly switch function, but much less work than using functors, for the same effect
    // TODO: change the values that now are magic numbers so that the destruction of the ships in a stage triggers the beginning of another one
    
    switch (patt) {
        case MAIN_STAGE:
// FIRST ENEMY
            if (turn == 0) {
                Mix_PlayChannel(-1, graph->getPortIn(), 0);
            }
            if (turn < 20 ) {
                // This is an excellent porting
                graph->printOtherOnScreen(SHIP_PORTING, 60, 60, turn*18, static_cast<double>(turn)/20);
            }
            else if (turn == 20) {
                spawnEnemy(40, 40, EN_FIGHTER, 1);
            }
            else if (turn > 20 && turn < 360) {
                if (enemyFleet[0]) {
                    moveEnemyOnMap(0, 2, enemyFleet[0]);
                }
            }
            else if (turn == 360 && enemyOnScreen) {
                // Here I need not use enemy1 and so on because I'm cleaning everything
                if (enemyFleet[0]->isAlive()) {
                    expiredEnemy0 = true;
                }
                cleanEnemyFleet();
                Mix_PlayChannel(-1, graph->getPortOut(), 0);
            }
            else if (turn > 360 && turn < 380) {
                if (expiredEnemy0) {
                    // Maybe turning the lasers a little in the direction of movement helps
                    graph->printOtherOnScreen(SHIP_PORTING, 740, 60, turn*18, static_cast<double>(380 - turn)/20);
                }
            }
            else if (turn == 380) {
                cleanEnemyFleet();
                expiredEnemy0 = false;
            }
            if (turn >= 31 && turn % 15 == 1 && turn < 360) {
                if (enemyFleet[0]->isAlive()) {
                    for (int i = -4; i <= 4; i++) {
                        Laser* tmp = enemyFleet[0]->shootLaser(i*pi/24 + (pi*1.5), LASER_ENEMY);
                        tmp->setSpeed(6);
                        lasersOnMap.push_back(tmp);
                    }
                    Mix_PlayChannel(-1, graph->getZap(), 0);
                }
            }
// SECOND ENEMY
            if (turn == 400) {
                Mix_PlayChannel(-1, graph->getPortIn(), 0);
            }
            if (turn > 400 && turn < 420 ) {
                graph->printOtherOnScreen(SHIP_PORTING, 740, 60, (turn-400)*18, static_cast<double>(turn - 400)/20);
            }
            else if (turn == 420) {
                spawnEnemy(720, 40, EN_FIGHTER, 1);
            }
            else if (turn > 420 && turn < 760) {
                if (enemyFleet[0]) {
                    moveEnemyOnMap(pi, 2, enemyFleet[0]);
                }
            }
            else if (turn == 760 && enemyOnScreen) {
                if (enemyFleet[0]->isAlive()) {
                    expiredEnemy0 = true;
                }
                cleanEnemyFleet();
                Mix_PlayChannel(-1, graph->getPortOut(), 0);
            }
            else if (turn > 760 && turn < 780) {
                if (expiredEnemy0) {
                    graph->printOtherOnScreen(SHIP_PORTING, 60, 60, turn*18, static_cast<double>(780 - turn)/20);
                }
            }
            else if (turn == 780) {
                cleanEnemyFleet();
                expiredEnemy0 = false;
            }
            if (turn >= 431 && turn % 15 == 1 && turn < 760) {
                if (enemyFleet[0]->isAlive()) {
                    for (int i = -4; i <= 4; i++) {
                        Laser* tmp = enemyFleet[0]->shootLaser(i*pi/24 + (pi*1.5), LASER_ENEMY);
                        tmp->setSpeed(6);
                        lasersOnMap.push_back(tmp);
                    }
                    Mix_PlayChannel(-1, graph->getZap(), 0);
                }
            }
// THIRD AND FOURTH ENEMIES
            // They move in sinusoids
            if (turn == 800) {
                Mix_PlayChannel(-1, graph->getPortIn(), 0);
            }
            if (turn > 800 && turn < 820 ) {
                graph->printOtherOnScreen(SHIP_PORTING, 60, 100, (turn-800)*18, static_cast<double>(turn - 800)/20);
                graph->printOtherOnScreen(SHIP_PORTING, 760, 100, (turn-800)*18, static_cast<double>(turn - 800)/20);
            }
            else if (turn == 820) {
                // enemy1 is the one on the right
                spawnEnemy(40, 80, EN_FIGHTER, 0.4);
                spawnEnemy(740, 80, EN_FIGHTER, 0.4);
            }
            else if (turn > 820 && turn < 1160) {
                if (enemyFleet[0]) {
                    moveEnemyOnMap(0, 2, enemyFleet[0]);
                    moveEnemyOnMap(pi/2, 5*sin((turn-800)/(2*pi)), enemyFleet[0]);
                }
                if (enemyFleet[1]) {
                    moveEnemyOnMap(pi, 2, enemyFleet[1]);
                    moveEnemyOnMap(pi/2, 5*sin((turn-800)/(2*pi)), enemyFleet[1]);
                }
            }
            else if (turn == 1160 && enemyOnScreen) {
                if (enemyFleet[0]->isAlive()) {
                    expiredEnemy0 = true;
                }
                if (enemyFleet[1]->isAlive()) {
                    expiredEnemy1 = true;
                }
                cleanEnemyFleet();
                Mix_PlayChannel(-1, graph->getPortOut(), 0);
            }
            else if (turn > 1160 && turn < 1180) {
                if (expiredEnemy0 == true) {
                    graph->printOtherOnScreen(SHIP_PORTING, 738, 148, turn*18, static_cast<double>(1180 - turn)/20);
                }
                if (expiredEnemy1 == true) {
                    graph->printOtherOnScreen(SHIP_PORTING, 82, 148, turn*18, static_cast<double>(1180 - turn)/20);
                }
            }
            else if (turn == 1180) {
                cleanEnemyFleet();
                expiredEnemy0 = false;
                expiredEnemy1 = false;
            }
            if (turn >= 831 && turn % 10 == 1 && turn < 1160) {
                bool laserPlayed = false;
                if (enemyFleet[0]->isAlive()) {
                    // Not exactly right, now i'll think
                    for (int i = -1; i<= 1; i++) {
                        Laser* tmp = enemyFleet[0]->shootLaser(i*pi/6 + 1.5*pi - (pi*(turn-1000)/350), LASER_ENEMY);
                        tmp->setSpeed(6);
                        lasersOnMap.push_back(tmp);
                        if (!laserPlayed) {
                            Mix_PlayChannel(-1, graph->getZap(), 0);
                            laserPlayed = true;
                        }
                    }
                }
                if (enemyFleet[1]->isAlive()) {
                    for (int i = -1; i <= 1; i++) {
                        Laser* tmp = enemyFleet[1]->shootLaser(i*pi/6 + 1.5*pi + (pi*(turn-1000)/350), LASER_ENEMY);
                        tmp->setSpeed(6);
                        lasersOnMap.push_back(tmp);
                        if (!laserPlayed) {
                            Mix_PlayChannel(-1, graph->getZap(), 0);
                            laserPlayed = true;
                        }
                    }
                }
            }
// FIFTH AND SIXTH ENEMIES
            // They move in circles
            if (turn == 1200) {
                Mix_PlayChannel(-1, graph->getPortIn(), 0);
            }
            if (turn > 1200 && turn < 1220 ) {
                graph->printOtherOnScreen(SHIP_PORTING, 160, 60, (turn-1200)*18, static_cast<double>(turn - 1200)/20);
                graph->printOtherOnScreen(SHIP_PORTING, 640, 60, (turn-1200)*18, static_cast<double>(turn - 1200)/20);
            }
            else if (turn == 1220) {
                // enemy1 is the one on the right
                spawnEnemy(180, 40, EN_FIGHTER, 0.8);
                spawnEnemy(580, 40, EN_FIGHTER, 0.8);
            }
            else if (turn > 1220 && turn < 1940) {
                if (enemyFleet[0]) {
                    moveEnemyOnMap(pi + (turn-1200)*pi/180, 2, enemyFleet[0]);
                }
                if (enemyFleet[1]) {
                    moveEnemyOnMap(-(turn-1200)*pi/180, 2, enemyFleet[1]);
                }
            }
            else if (turn == 1940 && enemyOnScreen) {
                if (enemyFleet[0]->isAlive()) {
                    expiredEnemy0 = true;
                }
                if (enemyFleet[1]->isAlive()) {
                    expiredEnemy1 = true;
                }
                cleanEnemyFleet();
                Mix_PlayChannel(-1, graph->getPortOut(), 0);
            }
            else if (turn > 1940 && turn < 1960) {
                if (expiredEnemy0) {
                    graph->printOtherOnScreen(SHIP_PORTING, 202, 59, turn*18, static_cast<double>(1960 - turn)/20);
                }
                if (expiredEnemy1) {
                    graph->printOtherOnScreen(SHIP_PORTING, 598, 59, turn*18, static_cast<double>(1960 - turn)/20);
                }
            }
            else if (turn == 1940) {
                cleanEnemyFleet();
                expiredEnemy0 = false;
                expiredEnemy1 = false;
            }
            if (turn >= 1231 && turn % 12 == 1 && turn < 1940) {
                double angleToProtagonist0 = atan2( enemyFleet[0]->getY() - protagonist->getY(), -(enemyFleet[0]->getX() - protagonist->getX()));
                double angleToProtagonist1 = atan2( enemyFleet[1]->getY() - protagonist->getY(), -(enemyFleet[1]->getX() - protagonist->getX()));
                // To use as the angle of the central laser in the burst. Redirecting the lasers will require a bunch of new atan2s
                bool laserPlayed = false;
                if (enemyFleet[0]->isAlive()) {
                    for (int i = -2; i<=2; i++) {
                        Laser* tmp = enemyFleet[0]->shootLaser(angleToProtagonist0 + i*pi/50, LASER_ENEMY);
                        tmp->setSpeed(6);
                        lasersOnMap.push_back(tmp);
                        if (!laserPlayed) {
                            Mix_PlayChannel(-1, graph->getZap(), 0);
                            laserPlayed = true;
                        }
                    }
                }
                if (enemyFleet[1]->isAlive()) {
                    for (int i = -2; i<=2; i++) {
                        Laser* tmp = enemyFleet[1]->shootLaser(angleToProtagonist1 + i*pi/50, LASER_ENEMY);
                        tmp->setSpeed(6);
                        lasersOnMap.push_back(tmp);
                        if (!laserPlayed) {
                            Mix_PlayChannel(-1, graph->getZap(), 0);
                            laserPlayed = true;
                        }
                    }
                }
            }
// SEVENTH AND FINAL ENEMY
            if (turn == 1980) {
                Mix_PlayChannel(-1, graph->getPortIn(), 0);
            }
            if (turn > 1980 && turn < 2000 ) {
                graph->printOtherOnScreen(SHIP_PORTING, 400, 80, (turn-1980)*18, static_cast<double>(turn - 1980)/20);
            }
            else if (turn == 2000) {
                // enemy1 is the one on the right
                spawnEnemy(380, 60, EN_FIGHTER, 2);
            }
            if (((turn%90 == 10 && turn >= 2000) || turn == 2000) && enemyFleet[0]->isAlive()) {
                bool laserPlayed = false;
                for (int i = 0; i < 18; i++) {
                    Laser* tmp = enemyFleet[0]->shootLaser(i*pi/9, LASER_ENEMY);
                    tmp->setSpeed(4);
                    lasersOnMap.push_back(tmp);
                    if (!laserPlayed) {
                        Mix_PlayChannel(-1, graph->getZap(), 0);
                        laserPlayed = true;
                    }
                }
            }
            if (turn >= 2000 && turn%90 >= 20) {
                int directionSwitch = ((turn-2000)/90)%3;
                moveEnemyOnMap(5*pi/3 - directionSwitch*2*pi/3, 4, enemyFleet[0]);
            }
            if (turn >= 2000) {
                for (int i = 0; i<lasersOnMap.size(); i++) {
                    if (!lasersOnMap[i]->isSBA() && lasersOnMap[i]->getTurnsInLife() > 16 && lasersOnMap[i]->getTurnsInLife() <= 20) {
                        lasersOnMap[i]->setSpeed(lasersOnMap[i]->getSpeed() - 1);
                    }
                    else if (!lasersOnMap[i]->isSBA() && lasersOnMap[i]->getTurnsInLife() == 40) {
                        double angDest = atan2( lasersOnMap[i]->getCentergY() - protagonist->getCenterY(), -(lasersOnMap[i]->getCentergX() - protagonist->getCenterX()));
                        lasersOnMap[i]->setAngle(angDest);
                    }
                    else if (!lasersOnMap[i]->isSBA() && lasersOnMap[i]->getTurnsInLife() == 50) {
                        lasersOnMap[i]->setSpeed(25);
                    }
                }
            }
            break;
            
// Formerly FLOWER
        case BOSS_STAGE:
            if (turn == 0) {
                cleanEnemyFleet();
                cleanLasers();
                floweringBoss = true;
                bossPosition = 0;
                Mix_PlayChannel(-1, graph->getPortIn(), 0);
            }
            if (turn < 20 ) {
                // This is an excellent porting
                graph->printOtherOnScreen(SHIP_PORTING, 400, 60, turn*18, static_cast<double>(turn)/20);
            }
            else if (turn == 20) {
                if (enemyFleet.size() == 0) {
                    spawnEnemy(380, 40, EN_CORVETTE, 1);
                }
                patternTurnsHelper = 0;   // The pattern is nicer this way
            }
            else if (turn <= 30) {
                // TODO: Adjust values
                moveEnemyOnMap((pi*1.5), 10, enemyFleet[0]);
            }
            
            if (turn >= 31 && turn % 500 <= 400) {
                if (enemyFleet[0]->isAlive() && enemyFleet.size() > 0 && turn%7 == 1) {
                    
                    // When it's the Flowering Night, then time will stop on the Lunar Clock. Only she who possesses the Pocket Watch of Blood will be able to go against the flow in stillness and against the stillness in the flow. She is Sakuya Izayoi, the maid in the Scarlet Devil Mansion.
                    // On unrelated news, the patterny pattern when it's not flowering is given later by manipulating the lazerbeams
                    if (floweringBoss) {
                        for (int i = 0; i<=20; i++) {
                            Laser* tmp = enemyFleet[0]->shootLaser(i*pi/10 + patternTurnsHelper*pi/32, LASER_ENEMY);
                            tmp->setSpeed(8);
                            lasersOnMap.push_back(tmp);
                        }
                    }
                    
                    // The pattern part when you're not flowering is given by shooting harmonically
                    else {
                        for (int i = 0; i<=20; i++) {
                            Laser* tmp = enemyFleet[0]->shootLaser(i*pi/10 + (pi/8)*sin((pi/100)*(turn%500)), LASER_ENEMY);
                            tmp->setSpeed(8);
                            lasersOnMap.push_back(tmp);
                        }
                    }
                    Mix_PlayChannel(-1, graph->getZap(), 0);
                }
            }
            
            /*      Possible pattern?
             *            entry
             *              |
             *      (. . . .V. . . .)
             *      (. 4 . .0. . 2 .)
             *      (. 6 . .8. . 5 .)
             *      (. 1 . .3. . 7 .)
             *      (. . . . . . . .)
             *      (. . . . . . . .)
             */
            
            if (turn >= 31 && turn % 500 >= 400) {
                switch (bossPosition) {
                    case 0:
                        if (turn%500 < 429) {
                            moveEnemyOnMap(atan2(-200, -220), 10, enemyFleet[0]);
                        }
                        if (turn%500 == 430) {
                            enemyFleet[0]->teleport(160, 340);
                        }
                        break;
                    case 1:
                        if (turn%500 < 448) {
                            moveEnemyOnMap(atan2(200, 440), 10, enemyFleet[0]);
                        }
                        if (turn%500 == 449) {
                            enemyFleet[0]->teleport(600, 140);
                        }
                        break;
                    case 2:
                        if (turn%500 < 429) {
                            moveEnemyOnMap(atan2(-200, -220), 10, enemyFleet[0]);
                        }
                        if (turn%500 == 430) {
                            enemyFleet[0]->teleport(380, 340);
                        }
                        break;
                    case 3:
                        if (turn%500 < 429) {
                            moveEnemyOnMap(atan2(200, -220), 10, enemyFleet[0]);
                        }
                        if (turn%500 == 430) {
                            enemyFleet[0]->teleport(160, 140);
                        }
                        break;
                    case 4:
                        if (turn%500 < 445) {
                            moveEnemyOnMap(atan2(-100, 440), 10, enemyFleet[0]);
                        }
                        if (turn%500 == 446) {
                            enemyFleet[0]->teleport(600, 240);
                        }
                        break;
                    case 5:
                        if (turn%500 < 444) {
                            moveEnemyOnMap(atan2(0, -220), 10, enemyFleet[0]);
                        }
                        if (turn%500 == 445) {
                            enemyFleet[0]->teleport(160, 240);
                        }
                        break;
                    case 6:
                        if (turn%500 < 445) {
                            moveEnemyOnMap(atan2(-100, 440), 10, enemyFleet[0]);
                        }
                        if (turn%500 == 446) {
                            enemyFleet[0]->teleport(600, 340);
                        }
                        break;
                    case 7:
                        if (turn%500 < 424) {
                            moveEnemyOnMap(atan2(100, -220), 10, enemyFleet[0]);
                        }
                        if (turn%500 == 425) {
                            enemyFleet[0]->teleport(380, 240);
                        }
                        break;
                    case 8:
                        if (turn%500 < 410) {
                            moveEnemyOnMap(atan2(100, 0), 10, enemyFleet[0]);
                        }
                        if (turn%500 == 411) {
                            enemyFleet[0]->teleport(380, 140);
                        }
                        break;
                        
                    default:
                        break;
                }
            }
            
            // This must be synced to the rate of fire
            if (turn%7 == 1) {
                if (floweringBoss) {
                    for (int i = 0; i<lasersOnMap.size(); i++) {
                        if (lasersOnMap[i]->getTurnsInLife() == 10 && !lasersOnMap[i]->isSBA()) {
                            lasersOnMap[i]->setAngle(lasersOnMap[i]->getAngle()+(pi/2));
                        }
                        if (lasersOnMap[i]->getTurnsInLife() > 10 && lasersOnMap[i]->getTurnsInLife() <= 90 && !lasersOnMap[i]->isSBA()) {
                            lasersOnMap[i]->setAngle(lasersOnMap[i]->getAngle()+(pi/3));
                        }
                        if (lasersOnMap[i]->getTurnsInLife() == 100 && !lasersOnMap[i]->isSBA()) {
                            lasersOnMap[i]->setAngle(lasersOnMap[i]->getAngle()-(pi/2));
                        }
                    }
                }
                patternTurnsHelper++;
            }
            if (turn%500 == 0 && turn != 0) {
                floweringBoss = !floweringBoss;
                bossPosition = (bossPosition+1)%9;
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
 * -- Circle collider. Priority VIOLET
 * -- It pains my heart, but Tiles shall be deprecated. Priority BLUE
 * -- Safeguard adding new ships against going out of the borders. Priority YELLOW
 * -- Have the patterns work well. Priority BLUE
 * -- Think about points and implement kills maybe. Priority GREEN
 * -- Create menus. Possibly making Engine only work in gaming and having the menus as a responsibility of main. Priority YELLOW
 * -- Make simple explode animations for lasers and ships. Priority YELLOW
 *
 * TODO:
 *
 * -- All done! Didn't think this day would come!
 *
 * NOTE:
 * -- Having lasers turn at weird irregular angles creates nice patterns
 * -- A nice location for shooting lasers is (98, 40)
 *
 */

// the main function, transferred in engine. Setting all the graphics stuff as children of Graphics and so on allowed me to make the functions slimmer
int Engine::start(Pattern patt)
{
    //int tmpx = 2, tmpy = 2;
    
    // For second plays
    cleanEnemyFleet();
    cleanLasers();
    
    lastTriggered = EVE_DEFAULT;
    
    
    // For intermediate animations.
    int intervalLaserCounter = 0;
    int turnCounter = 0;
    
    // For managing the laser shooting
    int lasercounter = 0;
    bool readyToFire = true;
    int passTurnTimer = 30;
    
    currentPattern = patt;
    
    // Getting the pointer straight
    protagonist = nullptr;
    
    expiredEnemy0 = false;
    expiredEnemy1 = false;
    
    //Start up SDL and create window
    
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
    addProtagonist();
    
    // The address of the enemy ship hit by lasers each turn
    Spaceship* hitTarget = nullptr;
    
    //First drawing
    SDL_RenderClear(graphEngine->getRenderer());
    graphEngine->setView(MAIN_CAMERA);
    gLastDisplayed = MAIN_CAMERA;
    drawFleet(graphEngine);
    SDL_RenderPresent(graphEngine->getRenderer());
    SDL_Delay(300); // TO DECUPLICATE
    
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
            else if (e.type == SDL_KEYDOWN)
            {
                switch (e.key.keysym.sym)
                {
                    case SDLK_x:
                    {
                        protagonist->setAtk(500);
                        break;
                    }
                    case SDLK_n:
                    {
                        protagonist->setAtk(20);
                        break;
                    }
                    case SDLK_ESCAPE:
                    {
                        quit = true;
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
        
        protagonist->teleport(tX, tY);
        
        // TODO: Regulate this treshold to achieve believable rate of fire
        if (!quit && intervalLaserCounter==3) {
            intervalLaserCounter = 1;
            if (lasercounter == 1) {
                lasercounter --;
                readyToFire = true;
            }
            else {
                lasercounter++;
            }
        }
        
        graphEngine->setView(gLastDisplayed);
        
        // These two are no longer troublesome
        graphEngine->printScore(score);
        graphEngine->printkills(kills);
        printHealth();
        
        if (lastTriggered == GAME_LOST) {
            SDL_Delay(50); // TO DECUPLICATE
            quit = true;
        }
        else if (lastTriggered == MAIN_STAGE_CLEAR && currentPattern == MAIN_STAGE) {
            quit = true;
        }
        else if (lastTriggered == BOSS_STAGE_CLEAR && currentPattern == BOSS_STAGE) {
            quit = true;
        }
        
        if (readyToFire && enemyOnScreen) { // I shall make the count start from a certain turn
            readyToFire = false;
            // Contrarily to popular belief, lasers do NOT cause memory leaks.
            protagonistShootsSingleLaser();
            //areLasersAimed = true;
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
                    explosionPlayed = true;
                }
            }
            else if (lasersOnMap[i]->isHittingWall()) {
                delete(lasersOnMap[i]);
                lasersOnMap.erase(lasersOnMap.begin()+i);
            }
            else if (laserIsHittingTarget(i, hitTarget)) {
                if (hitTarget != nullptr){
                    hitTarget->takeDamage(lasersOnMap[i]->getPower());
                    if (!hitTarget->isAlive() && !hitTarget->isAllied()) {
                        kills++;
                        score += 500;
                        explosion tmp = {static_cast<int>(hitTarget->getCenterX()), static_cast<int>(hitTarget->getCenterY()), 10};
                        kabooms.push_back(tmp);
                    }
                    lasersOnMap[i]->explode();
                }
                hitTarget = nullptr;
            }
        }
        // Killing the enemy
        for (int i = 0; i<enemyFleet.size(); i++) {
            if (enemyOnScreen == true && enemyFleet[i]->getHP() <= 0) {
                hitTarget = nullptr;
            }
        }
        bool everyoneDead = true;
        for (int i = 0; i<enemyFleet.size(); i++) {
            if (enemyFleet[i]->isAlive()) {
                everyoneDead = false;
            }
        }
        if (everyoneDead) {
            enemyOnScreen = false;
        }
        //Adding points
        score += (SDL_GetTicks() - time);
        
        // Cleaning the lasers
        for (int i = 0; i<lasersOnMap.size(); i++) {
            //if (!areLasersAimed) {
            lasersOnMap[i]->travel();
            //}
            // This is all in case I ever want to aim the lasers again
            /*
            else {
                if (lasersOnMap[i]->isSBA() && enemyOnScreen) {
                    int x = 0, y = 0;
                    coordsOfNearestEnemy(x, y, i, false);
                    
                    int cosfact = x - lasersOnMap[i]->getCentergX();
                    int sinfact = y - lasersOnMap[i]->getCentergY();
                    double targAngle;
                    if (cosfact != 0) {
                        targAngle = atan2(sinfact, cosfact);
                    }
                    else {
                        if (sinfact > 0) {
                            targAngle = pi/2;
                        }
                        else {
                            targAngle = -pi/2;
                        }
                    }
                    lasersOnMap[i]->setAngle(-targAngle);
                    lasersOnMap[i]->travel();
                }
                else {
                    lasersOnMap[i]->travel();
                }
            }
            */
        }
        if (enemyFleet.size() == 0) {
            enemyOnScreen = false;
        }
        if (protagonist->getHP() <= 0) {
            setLastEvent(GAME_LOST);
        }
        if (turnCounter > 2000 && !enemyOnScreen && currentPattern == MAIN_STAGE) {
            if (passTurnTimer == 0) {
                cleanEnemyFleet();
                cleanLasers();
                setLastEvent(MAIN_STAGE_CLEAR);
            }
            else {
                passTurnTimer--;
            }
        }
        if (turnCounter > 20 && !enemyOnScreen && currentPattern == BOSS_STAGE) {
            if (passTurnTimer == 0) {
                cleanEnemyFleet();
                cleanLasers();
                setLastEvent(BOSS_STAGE_CLEAR);
            }
            else {
                passTurnTimer--;
            }
        }
        for (int i = 0; i<kabooms.size(); i++) {
            if (kabooms[i].timer == 0) {
                kabooms.erase(kabooms.begin()+i);
            }
        }
        for (int i = 0; i<kabooms.size(); i++) {
            OtherSprites fireball = rand() >= RAND_MAX/2 ? SHIP_EXPLODE_1 : SHIP_EXPLODE_2;
            double zF = (10-kabooms[i].timer);
            zF /= 10;
            graphEngine->printOtherOnScreen(fireball, kabooms[i].x, kabooms[i].y, 0, zF, 80, 80);
            kabooms[i].timer -= 1;
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
    // Here ends the main loop
    //Free resources and close SDL
    return 0;
}
