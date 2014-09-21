//
//  Main.cpp
//  GiocoLabInf
//
//  Created by Enrico Migliorini on 12/03/14.
//  Copyright (c) 2014 Enrico Migliorini. All rights reserved.
//

//==================================================//
//                                                  //
//          Main function and game loop             //
//                                                  //
//==================================================//

#include "Engine.h"

int main()
{
    Engine engi;
    // Here start the SDL business
    srand (static_cast<int>(time(NULL)));
    
    if( !engi.getGraphicsEngine()->init() )
	{
		cout << "Failed to initialize!" << endl;
	}
	else
	{
		//Load media
		if( !engi.getGraphicsEngine()->loadMedia())
		{
			cout << "Failed to load media!" << endl;
		}
		else
		{
            bool quit = false;
            bool finished = false;
            while (!quit && !finished) {
                Mix_FadeInMusic(engi.getGraphicsEngine()->getMenuTheme(), -1, 1000);
                SDL_Event e;
                // When you reboot, you must have points and kills to 0
                engi.cleanStats();
                SDL_RenderClear(engi.getGraphicsEngine()->getRenderer());
                engi.getGraphicsEngine()->setView(MAIN_MENU);
                SDL_RenderPresent(engi.getGraphicsEngine()->getRenderer());
                bool start = false;
                
                while (!start && !quit) {
                    while (SDL_PollEvent (&e) != 0)
                    {
                        if (e.type == SDL_QUIT) {
                            quit = true;
                        }
                        else if (e.type == SDL_MOUSEBUTTONDOWN) {
                            start = true;
                            SDL_ShowCursor(0);
                        }
                        else if (e.type == SDL_KEYDOWN) {
                            switch (e.key.keysym.sym) {
                                case SDLK_ESCAPE:
                                    quit = true;
                                    break;
                                default:
                                    break;
                            }
                        }
                    }
                }
                if (!quit) {
                    // Change the music
                    engi.narrate(ADMIRAL_SPEAK, 0);
                    Mix_FadeOutMusic(1000);
                    Mix_FadeInMusic(engi.getGraphicsEngine()->getMainTheme(), -1, 1000);
                    // TO CHANGE !!! !!! !!! !!! !!! !!! !!! !!!
                    engi.start(MAIN_STAGE);
                    Mix_FadeOutMusic(1000);
                    if (engi.getLastEvent() == MAIN_STAGE_CLEAR) {
                        engi.narrate(ADMIRAL_SPEAK, 1);
                        Mix_FadeInMusic(engi.getGraphicsEngine()->getBossTheme(), -1, 1000);
                        engi.start(BOSS_STAGE);
                        Mix_HaltMusic();
                        if (engi.getLastEvent() == BOSS_STAGE_CLEAR) {
                            Mix_FadeInMusic(engi.getGraphicsEngine()->getVictoryTheme(), -1, 1000);
                            engi.narrate(CHANCELLOR_SPEAK, 2);
                            finished = true;
                            Mix_FadeOutMusic(1000);
                        }
                        else if (engi.getLastEvent() == GAME_LOST) {
                            engi.getGraphicsEngine()->setView(GAME_OVER);
                            SDL_RenderPresent(engi.getGraphicsEngine()->getRenderer());
                            SDL_Delay(1000);
                            engi.getGraphicsEngine()->setView(END_GAME);
                            engi.getGraphicsEngine()->printkills(engi.getKills(), 300, 200);
                            engi.getGraphicsEngine()->printScore(engi.getScore(), 260, 300);
                            SDL_RenderPresent(engi.getGraphicsEngine()->getRenderer());
                            engi.waitForConfirmation();
                        }
                    }
                    else if (engi.getLastEvent() == GAME_LOST) {
                        engi.getGraphicsEngine()->setView(GAME_OVER);
                        SDL_RenderPresent(engi.getGraphicsEngine()->getRenderer());
                        SDL_Delay(1000);
                        engi.getGraphicsEngine()->setView(END_GAME);
                        engi.getGraphicsEngine()->printkills(engi.getKills(), 300, 200);
                        engi.getGraphicsEngine()->printScore(engi.getScore(), 260, 300);
                        SDL_RenderPresent(engi.getGraphicsEngine()->getRenderer());
                        engi.waitForConfirmation();
                    }
                }
                if (finished) {
                    engi.getGraphicsEngine()->setView(END_GAME);
                    engi.getGraphicsEngine()->printkills(engi.getKills(), 300, 200);
                    engi.getGraphicsEngine()->printScore(engi.getScore(), 260, 300);
                    SDL_RenderPresent(engi.getGraphicsEngine()->getRenderer());
                    engi.waitForConfirmation();
                }
            }
        }
    }
    engi.getGraphicsEngine()->close();
	return 0;
}
