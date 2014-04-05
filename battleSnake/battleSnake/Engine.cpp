//
//  Engine.cpp
//  GiocoLabInf
//
//  Created by Enrico Migliorini on 17/03/14.
//  Copyright (c) 2014 Enrico Migliorini. All rights reserved.
//

#include <new>

#include "Engine.h"
#include "Character classes/Youngster.h"
#include "Character classes/Oldlady.h"
#include "Character classes/Hippie.h"
#include "Character classes/Bully.h"
#include "Character classes/Vandal.h"
#include "Character classes/Nerd.h"
#include "Character classes/Karateka.h"
#include "Character classes/Snob.h"


Engine::Engine() {
    moveBuffer[0] = NORTH;
    moveBuffer[1] = NORTH;
    moveBuffer[2] = NORTH;
}

// in addTeamMember I need some sort of control, to avoid having duplicate characters
void Engine::addTeamMember(Characters choice) {     // Of course temporary stats. Alpha and Beta testing will tell me what to balance.
    PartyMember* player;
    switch (choice) {
        case YOUNGSTER:
        {
            player = new Youngster("Youngster");
            break;
        }
        case OLD_LADY:
        {
            
            player = new Oldlady("Old lady");
            break;
        }
        case HIPPIE:
        {
            player = new Hippie("Hippie");
            break;
        }
        case BULLY:
        {
            player = new Bully("Bully");
            break;
        }
        case VANDAL:
        {
            player = new Vandal("Vandal");
            break;
        }

        case NERD:
        {
            player = new Nerd("Nerd");
            break;
        }

        case KARATEKA:
        {
            player = new Karateka("Karateka");
            break;
        }

        case SNOB:
        {
            player = new Snob("Snob");
            break;
        }
        default:
            player = nullptr;
            cerr << "This was never meant to happen! Invalid team member!" << endl;
            break;
    }
    team.push_back(*player);
}

// This one is temporary. As soon as I have implemented the 8 player classes, this function is going to change.
void Engine::teamBuilder(Screens &lastDisplayed, bool &isLMD, bool &isND, Graphics graph) {
    // Graphics::setView(CHARACTER_SELECT, lastDisplayed, isLMD, isND, renderer, texture); // I don't need it yet. YET.
    // INSERT text rendering, a little triangle next to the selection and the chance to choose the team. The checker could be something that grays out and makes inaccessible what you just picked, IDK how but it's feasible. Remember vector.size.
    bool completed = false;
    int teamsize = 0;
    int choice = 1;
    int tmpY = 5;
    while (!completed) {
        // CHANGE WITH SDL EVENTS ASAP
        cout << "Choose next member through 1-8 code; " << 4-teamsize << " remaining" << endl;
        cin >> choice;  // Problem with some non-number values of choice apparently
        if (choice >= 1 && choice <= 8) {
            addTeamMember(convertIntsToCharacter(choice));
            team[teamsize].setX(5);
            team[teamsize].setY(tmpY);
            teamsize++;
            tmpY ++;
            if (teamsize == MAX_TEAMSIZE) {
                completed = true;
            }
        }
        else {
            cout << "Invalid value" << endl;
        }
    }
}

void Engine::drawTeam(Graphics graph) {
    for (int i = 0; i<MAX_TEAMSIZE; i++) {
        team[i].drawOnScene(graph);
    }
}

// Horrible tool that will find its way to deletion as soon as i link SDL input
Characters Engine::convertIntsToCharacter(int toConvert) {
    Characters result = SPRITES_TOTAL;  // SPRITES_TOTAL is my invalid value
    switch (toConvert) {
        case 1:
            result = YOUNGSTER;
            break;
        case 2:
            result = OLD_LADY;
            break;
        case 3:
            result = HIPPIE;
            break;
        case 4:
            result = BULLY;
            break;
        case 5:
            result = VANDAL;
            break;
        case 6:
            result = NERD;
            break;
        case 7:
            result = KARATEKA;
            break;
        case 8:
            result = SNOB;
            break;
        default:
            result = SPRITES_TOTAL;
            cerr << "Valore non valido!" << endl;
            break;
    }
    return result;
}

void Engine::printTeamStats() {
    for (int i = 0; i<MAX_TEAMSIZE; i++) {
        cout << "Member in position " << i+1 << ": a " << team[i].getClass() << " called " << team[i].getName() << " who has these values for attack, defense and health: " << team[i].getAtk() << ", " << team[i].getDef() << ", " << team[i].getHP() << "." << endl;
    }
}

void Engine::moveTeamOnMap(Direction dest, Level currLevel) {
    int tmpX = team[0].getX();
    int tmpY = team[0].getY();
    switch (dest) {
        case NORTH:
            tmpY -= 1;
            break;
        case EAST:
            tmpX += 1;
            break;
        case SOUTH:
            tmpY += 1;
            break;
        case WEST:
            tmpX -= 1;
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
            team[0].move(dest);
            team[1].move(moveBuffer[0]);
            team[2].move(moveBuffer[1]);
            team[3].move(moveBuffer[2]);
            moveBuffer[2] = moveBuffer[1];
            moveBuffer[1] = moveBuffer[0];
            moveBuffer[0] = dest;
            break;
        case 3:         // Different codes for different events
        case 4:
        case 5:
        case 6:
            team[0].move(dest);
            team[1].move(moveBuffer[0]);
            team[2].move(moveBuffer[1]);
            team[3].move(moveBuffer[2]);
            moveBuffer[2] = moveBuffer[1];
            moveBuffer[1] = moveBuffer[0];
            moveBuffer[0] = dest;
            // Commands are repeated, but either writing a function or adding a check boolean or two seems an overkill at this stage
            setLastEvent(currLevel.getEventFromCode(currLevel.getTileCode(tmpX, tmpY)));
            break;
        default:
            break;
    }
}

void Engine::setLastEvent(Event lastEvent) {
    lastTriggered = lastEvent;
}

// THIS MIGHT BE USELESS, CHECK LATER
Event Engine::getLastEvent() {
    return lastTriggered;
}

void Engine::specialEvent(Event triggered) {    // You can say it's incomplete
    switch (triggered) {
        case LEV_CLOSURE:
            break;
        case BAT_TUTORIAL:
            break;
        default:
            break;
    }
}

// the main function, transferred in engine. Setting all the graphics stuff as children of Graphics and so on may allow me to make the functions slimmer
int Engine::start()
{
    Level currentLevel("Borboroth");
    Graphics graphEngine;
    
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
            bool isLowerMenuDisplayed = false;
            bool isNarrationDisplayed = false;
            
            // Teambuilding
            teamBuilder(gLastDisplayed, isLowerMenuDisplayed, isNarrationDisplayed, graphEngine);
            
            //First drawing
            SDL_RenderClear(graphEngine.getRenderer());
            graphEngine.setView(MAIN_CAMERA, gLastDisplayed, isLowerMenuDisplayed, isNarrationDisplayed);
            drawTeam(graphEngine);
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
            
			while( !quit )  // REMEMBER REMEMBER THE FIFTH OF NOVEMBER setView ed addSubMenu create screens and menus
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
                                // If I wished, I could control them separately by using a currentControlled int and moving engi.team[currentControlled], then switching control on the press of 1, 2, 3, 4.
							case SDLK_UP:
                                moveTeamOnMap(NORTH, currentLevel);
                                break;
							case SDLK_DOWN:
                                moveTeamOnMap(SOUTH, currentLevel);
                                break;
							case SDLK_LEFT:
                                moveTeamOnMap(WEST, currentLevel);
                                break;
							case SDLK_RIGHT:
                                moveTeamOnMap(EAST, currentLevel);
                                break;
                            case SDLK_1:
                                // I predict that the characters will disappear. THEY DO NOT, though they are on a higher level.
                                // This might mean that I do not need to do much to set the rendering view, just some kind of rect when I'm rendering in the maincamera
                                graphEngine.addSubMenu(LOWER_MENU, gLastDisplayed, isLowerMenuDisplayed, isNarrationDisplayed);
                                break;
							default:
                                break;
						}
                        
                        // APPARENTLY what is not on the renderer is destroyed when you present it. BAD thing, but I just need to use gLastDisplayed and the boolean sisters and redraw everything.
                        
                        graphEngine.setView(gLastDisplayed, gLastDisplayed, isLowerMenuDisplayed, isNarrationDisplayed);
                        drawTeam(graphEngine);
                        
                        //write the coordinates for checking
                        cout << team[0].getX() << ", " << team[0].getY() << endl;
                        
                        // Update screen    // ATTENTION, CALLING IT AT EVERY CYCLE SLOWS EVERYTHING DOWN DRAMATICALLY. Still, it might be the only viable solution for animations.
                        // I COULD call it every half a second or so, this way i can animate idle and keep dynamic response. multithreading needed? I hope not.
                        SDL_RenderPresent(graphEngine.getRenderer());
                        
                        switch (lastTriggered) {
                            case LEV_CLOSURE:
                                quit = true;
                                break;
                            case LEV_START:
                                graphEngine.setView(MAIN_CAMERA, gLastDisplayed, isLowerMenuDisplayed, isNarrationDisplayed);
                            default:
                                specialEvent(lastTriggered);
                                break;
                        }
					}
				}
			}
		}
	}
	//Free resources and close SDL
    graphEngine.close();
	return 0;
}
