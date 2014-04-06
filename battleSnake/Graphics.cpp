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
    gTexture[SELECTION_MENU] = loadTexture( "BGimages/char_select.png");
	if( gTexture[SELECTION_MENU] == 0 )
	{
		cout << "Failed to load texture image!" << endl;
		success = false;
	}
    
    
    //Loading sprites
    gAllies[FIGHTER] = loadTexture("CharSprites/fighter.png");
    if (gAllies[FIGHTER] == 0) {
        cout << "Failed to load texture image!" << endl;
        success = false;
    }
    gAllies[CORVETTE] = loadTexture("CharSprites/corvette.png");
    if (gAllies[CORVETTE] == 0) {
        cout << "Failed to load texture image!" << endl;
        success = false;
    }
    gAllies[FRIGATE] = loadTexture("CharSprites/frigate.png");
    if (gAllies[FRIGATE] == 0) {
        cout << "Failed to load texture image!" << endl;
        success = false;
    }
    gAllies[DESTROYER] = loadTexture("CharSprites/destroyer.png");
    if (gAllies[DESTROYER] == 0) {
        cout << "Failed to load texture image!" << endl;
        success = false;
    }
    gAllies[CRUISER] = loadTexture("CharSprites/cruiser.png");
    if (gAllies[CRUISER] == 0) {
        cout << "Failed to load texture image!" << endl;
        success = false;
    }
    
    const int SUBTRACT_ELEMENT = 5; // Half an array
    // Loading enemies
    gEnemies[EN_FIGHTER-SUBTRACT_ELEMENT] = loadTexture("EnemySprites/en_fighter.png");
    if (gEnemies[EN_FIGHTER-SUBTRACT_ELEMENT] == 0) {
        cout << "Failed to load texture image!" << endl;
        success = false;
    }
    gEnemies[EN_CORVETTE-SUBTRACT_ELEMENT] = loadTexture("EnemySprites/en_fighter.png");
    if (gEnemies[EN_CORVETTE-SUBTRACT_ELEMENT] == 0) {
        cout << "Failed to load texture image!" << endl;
        success = false;
    }
    gEnemies[EN_FRIGATE-SUBTRACT_ELEMENT] = loadTexture("EnemySprites/en_fighter.png");
    if (gEnemies[EN_FRIGATE-SUBTRACT_ELEMENT] == 0) {
        cout << "Failed to load texture image!" << endl;
        success = false;
    }
    gEnemies[EN_DESTROYER-SUBTRACT_ELEMENT] = loadTexture("EnemySprites/en_fighter.png");
    if (gEnemies[EN_DESTROYER-SUBTRACT_ELEMENT] == 0) {
        cout << "Failed to load texture image!" << endl;
        success = false;
    }
    gEnemies[EN_CRUISER-SUBTRACT_ELEMENT] = loadTexture("EnemySprites/en_fighter.png");
    if (gEnemies[EN_CRUISER-SUBTRACT_ELEMENT] == 0) {
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
    
    for (int i = 0; i<SHIP_TYPES; i++) {
            SDL_DestroyTexture(gAllies[i]);
            gAllies[i] = 0;
    }
    
    for (int i = 0; i<SHIP_TYPES; i++) {
        SDL_DestroyTexture(gEnemies[i]);
        gEnemies[i] = 0;
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

void Graphics::setView (Screens choice, Screens &lastDisplayed) {
    if (lastDisplayed != choice) {
        SDL_RenderClear(gRenderer);
        SDL_RenderCopy(gRenderer, gTexture[choice], 0, 0);
        lastDisplayed = choice;
    }
    else {
        // This is basically redrawing everything, keeping the menus displayed. Otherwise the menus would disappear.
        SDL_RenderClear(gRenderer);
        SDL_RenderCopy(gRenderer, gTexture[choice], 0, 0);
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

SDL_Texture* Graphics::getSprite(Characters sprite) {
    return gAllies[sprite];
}

SDL_Texture* Graphics::getEnFighterSprite() {
    return gEnemies[0];
}

TTF_Font* Graphics::getFont() {
    return gFont;
}
