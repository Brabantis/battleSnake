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
    
    gFont = TTF_OpenFont( "Font/5Identification.ttf", 28 );
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
    gTexture[GAME_OVER] = loadTexture("BGimages/game_over.png");
    if( gTexture[GAME_OVER] == 0 )
	{
		cout << "Failed to load texture image!" << endl;
		success = false;
	}
    
    //Loading sprites
    gSprites[FIGHTER] = loadTexture("CharSprites/fighter.png");
    if (gSprites[FIGHTER] == 0) {
        cout << "Failed to load texture image!" << endl;
        success = false;
    }
    gSprites[CORVETTE] = loadTexture("CharSprites/corvette.png");
    if (gSprites[CORVETTE] == 0) {
        cout << "Failed to load texture image!" << endl;
        success = false;
    }
    gSprites[FRIGATE] = loadTexture("CharSprites/frigate.png");
    if (gSprites[FRIGATE] == 0) {
        cout << "Failed to load texture image!" << endl;
        success = false;
    }
    gSprites[DESTROYER] = loadTexture("CharSprites/destroyer.png");
    if (gSprites[DESTROYER] == 0) {
        cout << "Failed to load texture image!" << endl;
        success = false;
    }
    gSprites[CRUISER] = loadTexture("CharSprites/cruiser.png");
    if (gSprites[CRUISER] == 0) {
        cout << "Failed to load texture image!" << endl;
        success = false;
    }
    
    // Loading enemies
    gSprites[EN_FIGHTER] = loadTexture("EnemySprites/en_fighter.png");
    if (gSprites[EN_FIGHTER] == 0) {
        cout << "Failed to load texture image!" << endl;
        success = false;
    }
    gSprites[EN_CORVETTE] = loadTexture("EnemySprites/en_fighter.png");
    if (gSprites[EN_CORVETTE] == 0) {
        cout << "Failed to load texture image!" << endl;
        success = false;
    }
    gSprites[EN_FRIGATE] = loadTexture("EnemySprites/en_fighter.png");
    if (gSprites[EN_FRIGATE] == 0) {
        cout << "Failed to load texture image!" << endl;
        success = false;
    }
    gSprites[EN_DESTROYER] = loadTexture("EnemySprites/en_fighter.png");
    if (gSprites[EN_DESTROYER] == 0) {
        cout << "Failed to load texture image!" << endl;
        success = false;
    }
    gSprites[EN_CRUISER] = loadTexture("EnemySprites/en_fighter.png");
    if (gSprites[EN_CRUISER] == 0) {
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
        SDL_DestroyTexture(gSprites[i]);
        gSprites[i] = 0;
    }
    
    SDL_DestroyTexture(gScore);
    gScore = 0;
    
    SDL_DestroyTexture(gParts);
    gParts = 0;
    
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

string Graphics::intToString(int input) {
    string s;
    stringstream out;
    out << input;
    s = out.str();
    return s;
}

void Graphics::printScore(int score) {
    gScore = loadFromRenderedText(intToString(score), {0, 0, 0});
    int width = 0;
    if (score == 0)
        width = 30;
    else if (score < 1000)
        width = 90;
    else if (score < 10000)
        width = 120;
    else
        width = 150;
    SDL_Rect dst = {40, 10, width, 30};
    SDL_RenderCopy(gRenderer, gScore, 0, &dst);
}

void Graphics::printParts(int parts) {
    gParts = loadFromRenderedText(intToString(parts), {0, 0, 0});
    int width = 0;
    if (parts < 10)
        width = 30;
    else if (parts < 100)
        width = 60;
    else if (parts < 1000)
        width = 90;
    else
        width = 120;
    SDL_Rect dst = {760-width, 10, width, 30};
    SDL_RenderCopy(gRenderer, gParts, 0, &dst);
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
    return gSprites[sprite];
}

SDL_Texture* Graphics::getScore() {
    return gScore;
}

SDL_Texture* Graphics::getParts() {
    return gParts;
}

TTF_Font* Graphics::getFont() {
    return gFont;
}
