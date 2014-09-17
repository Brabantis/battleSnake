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
            SDL_Event e;
            SDL_RenderClear(engi.getGraphicsEngine()->getRenderer());
            engi.getGraphicsEngine()->setView(MAIN_MENU);
            SDL_RenderPresent(engi.getGraphicsEngine()->getRenderer());
            bool start = false;
            bool quit = false;
            
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
                // Start the music
                engi.narrate(ADMIRAL_SPEAK, 0);
                Mix_FadeInMusic(engi.getGraphicsEngine()->getMainStage(), -1, 1000);
                engi.start(MAIN_STAGE);
                Mix_FadeOutMusic(1000);
                if (engi.getLastEvent() == MAIN_STAGE_CLEAR) {
                    engi.narrate(ADMIRAL_SPEAK, 1);
                    Mix_FadeInMusic(engi.getGraphicsEngine()->getBossTheme(), -1, 1000);
                    engi.start(BOSS_STAGE);
                    Mix_HaltMusic();
                    engi.narrate(CHANCELLOR_SPEAK, 2);
                }
                else {
                    quit = true;
                }
            }
        }
    }
    if (engi.getLastEvent() == GAME_LOST) {
        engi.getGraphicsEngine()->setView(GAME_OVER);
        SDL_RenderPresent(engi.getGraphicsEngine()->getRenderer());
        SDL_Delay(1000);
    }
    engi.getGraphicsEngine()->close();
	return 0;
}
