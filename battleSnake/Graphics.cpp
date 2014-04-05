//
//  Graphics.cpp
//  GiocoLabInf
//
//  Created by Enrico Migliorini on 15/03/14.
//  Copyright (c) 2014 Enrico Migliorini. All rights reserved.
//

#include "Graphics.h"

using namespace std;

Graphics::Graphics() {
    // not by the constructor, these will be created by init
    gWindow = 0;
    gRenderer = 0;
    gFont = 0;
}

bool Graphics::init()
{
	//Initialization flag
	bool success = true;
    
	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		cout << "SDL could not initialize! SDL Error: " << SDL_GetError() << endl;
		success = false;
	}
	else
	{
		//Set texture filtering to linear
		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
		{
			cout << "Warning: Linear texture filtering not enabled!" << endl;
		}
        
		//Create window
		gWindow = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( gWindow == 0 )
		{
			cout << "Window could not be created! SDL Error: " << SDL_GetError() << endl;
			success = false;
		}
		else
		{
			//Create renderer for window
			gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED );
			if( gRenderer == 0 )
			{
				cout << "Renderer could not be created! SDL Error: " << SDL_GetError() << endl;
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
                
				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if( !( IMG_Init( imgFlags ) & imgFlags ) )
				{
					cout << "SDL_image could not initialize! SDL_image Error: " << IMG_GetError() << endl;
					success = false;
				}
                
                //Initialize SDL_ttf
				if( TTF_Init() == -1 )
				{
					printf( "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError() );
					success = false;
				}
			}
		}
	}
	return success;
}


// This is a very long and copy-pasty function. Funny thing, writing another function to slow it down would likely not help. maybe I'll try later
bool Graphics::loadMedia()
{
	//Loading success flag
	bool success = true;
    
    gFont = TTF_OpenFont( "Font/Computerfont.ttf", 28 );
    if( gFont == 0 )
    {
        printf( "Failed to load the font! SDL_ttf Error: %s\n", TTF_GetError() );
        success = false;
    }
    
	//Load PNG texture
	gTexture[MAIN_CAMERA] = loadTexture( "BGimages/main_camera.png");
	if( gTexture[MAIN_CAMERA] == 0 )
	{
		cout << "Failed to load texture image!" << endl;
		success = false;
	}
    gTexture[MAIN_MENU] = loadTexture( "BGimages/main_menu.png");
	if( gTexture[MAIN_MENU] == 0 )
	{
		cout << "Failed to load texture image!" << endl;
		success = false;
	}
    gTexture[BATTLE] = loadTexture( "BGimages/battle.png");
	if( gTexture[BATTLE] == 0 )
	{
		cout << "Failed to load texture image!" << endl;
		success = false;
	}
    gTexture[LOWER_MENU] = loadTexture( "BGimages/lower_menu.png");
	if( gTexture[LOWER_MENU] == 0 )
	{
		cout << "Failed to load texture image!" << endl;
		success = false;
	}
    gTexture[NARRATION] = loadTexture( "BGimages/narration.png");
	if( gTexture[NARRATION] == 0 )
	{
		cout << "Failed to load texture image!" << endl;
		success = false;
	}
    gTexture[SELECTION_MENU] = loadTexture( "BGimages/selection_menu.png");
	if( gTexture[SELECTION_MENU] == 0 )
	{
		cout << "Failed to load texture image!" << endl;
		success = false;
	}
    
    
    //Loading sprites
    gSprites[YOUNGSTER][NORTH] = loadTexture("CharSprites/player1_n.png");
    if (gSprites[YOUNGSTER][NORTH] == 0) {
        cout << "Failed to load texture image!" << endl;
        success = false;
    }
    gSprites[YOUNGSTER][EAST] = loadTexture("CharSprites/player1_e.png");
    if (gSprites[YOUNGSTER][EAST] == 0) {
        cout << "Failed to load texture image!" << endl;
        success = false;
    }
    gSprites[YOUNGSTER][SOUTH] = loadTexture("CharSprites/player1_s.png");
    if (gSprites[YOUNGSTER][SOUTH] == 0) {
        cout << "Failed to load texture image!" << endl;
        success = false;
    }
    gSprites[YOUNGSTER][WEST] = loadTexture("CharSprites/player1_w.png");
    if (gSprites[YOUNGSTER][WEST] == 0) {
        cout << "Failed to load texture image!" << endl;
        success = false;
    }
    
    gSprites[OLD_LADY][NORTH] = loadTexture("CharSprites/player2_n.png");
    if (gSprites[OLD_LADY][NORTH] == 0) {
        cout << "Failed to load texture image!" << endl;
        success = false;
    }
    gSprites[OLD_LADY][EAST] = loadTexture("CharSprites/player2_e.png");
    if (gSprites[OLD_LADY][EAST] == 0) {
        cout << "Failed to load texture image!" << endl;
        success = false;
    }
    gSprites[OLD_LADY][SOUTH] = loadTexture("CharSprites/player2_s.png");
    if (gSprites[OLD_LADY][SOUTH] == 0) {
        cout << "Failed to load texture image!" << endl;
        success = false;
    }
    gSprites[OLD_LADY][WEST] = loadTexture("CharSprites/player2_w.png");
    if (gSprites[OLD_LADY][WEST] == 0) {
        cout << "Failed to load texture image!" << endl;
        success = false;
    }
    
    gSprites[HIPPIE][NORTH] = loadTexture("CharSprites/player3_n.png");
    if (gSprites[HIPPIE][NORTH] == 0) {
        cout << "Failed to load texture image!" << endl;
        success = false;
    }
    gSprites[HIPPIE][EAST] = loadTexture("CharSprites/player3_e.png");
    if (gSprites[HIPPIE][EAST] == 0) {
        cout << "Failed to load texture image!" << endl;
        success = false;
    }
    gSprites[HIPPIE][SOUTH] = loadTexture("CharSprites/player3_s.png");
    if (gSprites[HIPPIE][SOUTH] == 0) {
        cout << "Failed to load texture image!" << endl;
        success = false;
    }
    gSprites[HIPPIE][WEST] = loadTexture("CharSprites/player3_w.png");
    if (gSprites[HIPPIE][WEST] == 0) {
        cout << "Failed to load texture image!" << endl;
        success = false;
    }
    
    gSprites[BULLY][NORTH] = loadTexture("CharSprites/player4_n.png");
    if (gSprites[BULLY][NORTH] == 0) {
        cout << "Failed to load texture image!" << endl;
        success = false;
    }
    gSprites[BULLY][EAST] = loadTexture("CharSprites/player4_e.png");
    if (gSprites[BULLY][EAST] == 0) {
        cout << "Failed to load texture image!" << endl;
        success = false;
    }
    gSprites[BULLY][SOUTH] = loadTexture("CharSprites/player4_s.png");
    if (gSprites[BULLY][SOUTH] == 0) {
        cout << "Failed to load texture image!" << endl;
        success = false;
    }
    gSprites[BULLY][WEST] = loadTexture("CharSprites/player4_w.png");
    if (gSprites[BULLY][WEST] == 0) {
        cout << "Failed to load texture image!" << endl;
        success = false;
    }
    
    gSprites[VANDAL][NORTH] = loadTexture("CharSprites/player5_n.png");
    if (gSprites[VANDAL][NORTH] == 0) {
        cout << "Failed to load texture image!" << endl;
        success = false;
    }
    gSprites[VANDAL][EAST] = loadTexture("CharSprites/player5_e.png");
    if (gSprites[VANDAL][EAST] == 0) {
        cout << "Failed to load texture image!" << endl;
        success = false;
    }
    gSprites[VANDAL][SOUTH] = loadTexture("CharSprites/player5_s.png");
    if (gSprites[VANDAL][SOUTH] == 0) {
        cout << "Failed to load texture image!" << endl;
        success = false;
    }
    gSprites[VANDAL][WEST] = loadTexture("CharSprites/player5_w.png");
    if (gSprites[VANDAL][WEST] == 0) {
        cout << "Failed to load texture image!" << endl;
        success = false;
    }
    
    gSprites[NERD][NORTH] = loadTexture("CharSprites/player6_n.png");
    if (gSprites[NERD][NORTH] == 0) {
        cout << "Failed to load texture image!" << endl;
        success = false;
    }
    gSprites[NERD][EAST] = loadTexture("CharSprites/player6_e.png");
    if (gSprites[NERD][EAST] == 0) {
        cout << "Failed to load texture image!" << endl;
        success = false;
    }
    gSprites[NERD][SOUTH] = loadTexture("CharSprites/player6_s.png");
    if (gSprites[NERD][SOUTH] == 0) {
        cout << "Failed to load texture image!" << endl;
        success = false;
    }
    gSprites[NERD][WEST] = loadTexture("CharSprites/player6_w.png");
    if (gSprites[NERD][WEST] == 0) {
        cout << "Failed to load texture image!" << endl;
        success = false;
    }
    
    gSprites[KARATEKA][NORTH] = loadTexture("CharSprites/player7_n.png");
    if (gSprites[KARATEKA][NORTH] == 0) {
        cout << "Failed to load texture image!" << endl;
        success = false;
    }
    gSprites[KARATEKA][EAST] = loadTexture("CharSprites/player7_e.png");
    if (gSprites[KARATEKA][EAST] == 0) {
        cout << "Failed to load texture image!" << endl;
        success = false;
    }
    gSprites[KARATEKA][SOUTH] = loadTexture("CharSprites/player7_s.png");
    if (gSprites[KARATEKA][SOUTH] == 0) {
        cout << "Failed to load texture image!" << endl;
        success = false;
    }
    gSprites[KARATEKA][WEST] = loadTexture("CharSprites/player7_w.png");
    if (gSprites[KARATEKA][WEST] == 0) {
        cout << "Failed to load texture image!" << endl;
        success = false;
    }
    
    gSprites[SNOB][NORTH] = loadTexture("CharSprites/player8_n.png");
    if (gSprites[SNOB][NORTH] == 0) {
        cout << "Failed to load texture image!" << endl;
        success = false;
    }
    gSprites[SNOB][EAST] = loadTexture("CharSprites/player8_e.png");
    if (gSprites[SNOB][EAST] == 0) {
        cout << "Failed to load texture image!" << endl;
        success = false;
    }
    gSprites[SNOB][SOUTH] = loadTexture("CharSprites/player8_s.png");
    if (gSprites[SNOB][SOUTH] == 0) {
        cout << "Failed to load texture image!" << endl;
        success = false;
    }
    gSprites[SNOB][WEST] = loadTexture("CharSprites/player8_w.png");
    if (gSprites[SNOB][WEST] == 0) {
        cout << "Failed to load texture image!" << endl;
        success = false;
    }
    
	return success;
}

void Graphics::close()
{
	//Free loaded image
    for (int i = 0; i<TEXTURES_TOTAL; i++) {
        SDL_DestroyTexture(gTexture[i]);
        gTexture[i] = 0;
    }
    
    for (int i = 0; i<SPRITES_TOTAL; i++) {
        for (int j = 0; j<DIRECTION_TOTAL; j++) {
            SDL_DestroyTexture(gSprites[i][j]);
            gSprites[i][j] = 0;
        }
    }
    
    //Remove font
    TTF_CloseFont(gFont);
    gFont = 0;
    
	//Destroy window
	SDL_DestroyRenderer(gRenderer);
    gRenderer = 0;
    
	SDL_DestroyWindow(gWindow);
	gWindow = 0;
    
	//Quit SDL subsystems
    TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}

void Graphics::copyLowerLayerTexture(Screens last, bool lower, bool upper)
{
    SDL_RenderClear(gRenderer);
    SDL_RenderCopy(gRenderer, gTexture[last], 0, 0);
    if (lower) {
        SDL_RenderCopy(gRenderer, gTexture[LOWER_MENU], 0, 0);
    }
    if (upper) {
        SDL_RenderCopy(gRenderer, gTexture[NARRATION], 0, 0);
    }
}

SDL_Texture* Graphics::loadTexture(string path)
{
	//The final texture
	SDL_Texture* newTexture = 0;
    
	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
	if( loadedSurface == 0 )
	{
		cout << "Unable to load image " << path.c_str() << "! SDL_image Error: " << IMG_GetError();
	}
	else
	{
		//Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface( gRenderer, loadedSurface );
		if( newTexture == 0 )
		{
			cout << "Unable to create texture from " << path.c_str() << "! SDL Error: " << SDL_GetError() << endl;
		}
        
		//Get rid of old loaded surface
		SDL_FreeSurface( loadedSurface );
	}
    
	return newTexture;
}

// Debug like there is no tomorrow
SDL_Texture* Graphics::loadFromRenderedText(string textureText, SDL_Color textColor)
{
    //Destination texture
    SDL_Texture* newTexture = 0;
    
    //Render text surface
	SDL_Surface* textSurface = TTF_RenderText_Solid(gFont, textureText.c_str(), textColor);
	if( textSurface == 0 )
	{
		printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
	}
	else
	{
		//Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface(gRenderer, textSurface);
		if(newTexture == 0)
		{
			printf("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
		}
		//Get rid of old surface
		SDL_FreeSurface(textSurface);
	}
    
	return newTexture;
}

void Graphics::addSubMenu(Screens choice, Screens lastDisplayed, bool &isLowMenuDisplayed, bool &isNarrDisplayed) {
    if (choice == NARRATION) {
        if (!isNarrDisplayed) {
            isNarrDisplayed = true;
            copyLowerLayerTexture(lastDisplayed, isLowMenuDisplayed, isNarrDisplayed);
            SDL_RenderCopy(gRenderer, gTexture[NARRATION], 0, 0);
        }
        else {
            isNarrDisplayed = false;
            copyLowerLayerTexture(lastDisplayed, isLowMenuDisplayed, isNarrDisplayed);
        }
    }
    else if (choice == LOWER_MENU) {
        if (!isLowMenuDisplayed) {
            isLowMenuDisplayed = true;
            copyLowerLayerTexture(lastDisplayed, isLowMenuDisplayed, isNarrDisplayed);
            SDL_RenderCopy(gRenderer, gTexture[LOWER_MENU], 0, 0);
        }
        else {
            isLowMenuDisplayed = false;
            copyLowerLayerTexture(lastDisplayed, isLowMenuDisplayed, isNarrDisplayed);
        }
    }
}

void Graphics::setView (Screens choice, Screens &lastDisplayed, bool &isLowMenuDisplayed, bool &isNarrDisplayed) {
    if (lastDisplayed != choice) {
        SDL_RenderClear(gRenderer);
        SDL_RenderCopy(gRenderer, gTexture[choice], 0, 0);
        isLowMenuDisplayed = false;
        isNarrDisplayed = false;
        lastDisplayed = choice;
    }
    else {
        // This is basically redrawing everything, keeping the menus displayed. Otherwise the menus would disappear.
        SDL_RenderClear(gRenderer);
        SDL_RenderCopy(gRenderer, gTexture[choice], 0, 0);
        if (isNarrDisplayed) {
            copyLowerLayerTexture(lastDisplayed, isLowMenuDisplayed, isNarrDisplayed);
            SDL_RenderCopy(gRenderer, gTexture[NARRATION], 0, 0);
        }
        if (isLowMenuDisplayed) {
            copyLowerLayerTexture(lastDisplayed, isLowMenuDisplayed, isNarrDisplayed);
            SDL_RenderCopy(gRenderer, gTexture[LOWER_MENU], 0, 0);
        }
    }
}

// for now this one needs a rect, i'll have to learn how to get the width of the rendered texture
void Graphics::printTextOnScreen (string txtInput, SDL_Rect* destination) {
    SDL_Texture* tempTexture = loadFromRenderedText(txtInput, {0, 0, 0}); // NOTE: everything is black dabadee dabadack
    SDL_RenderCopy(gRenderer, tempTexture, 0, destination);
    free(tempTexture);
    tempTexture = 0;
}


SDL_Window* Graphics::getWindow() {
    return gWindow;
}
SDL_Renderer* Graphics::getRenderer() {
    return gRenderer;
}

SDL_Texture* Graphics::getTexture(Screens texture) {
    return gTexture[texture];
}

SDL_Texture* Graphics::getSprites(Characters sprite, Direction facing) {
    return gSprites[sprite][facing];
}
TTF_Font* Graphics::getFont() {
    return gFont;
}

