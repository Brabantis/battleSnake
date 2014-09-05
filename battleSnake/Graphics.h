//
//  Graphics.h
//  GiocoLabInf
//
//  Created by Enrico Migliorini on 15/03/14.
//  Copyright (c) 2014 Enrico Migliorini. All rights reserved.
//

//==================================================//
//                                                  //
//   Graphics has a number of drawing functions     //
//    that might then be used by other classes      //
//==================================================//

#ifndef __GiocoLabInf__Graphics__
#define __GiocoLabInf__Graphics__

#include <iostream>
#include <sstream>

#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#include <SDL2_ttf/SDL_ttf.h>
#include <SDL2_mixer/SDL_mixer.h>


using namespace std;

// These enums are used throughout the game

enum Screens
{
    MAIN_CAMERA,
    GAME_OVER,
    GAME_WON,
    LEVEL_WON,
    TEXTURES_TOTAL
};

enum Characters
{
    FIGHTER,
    CORVETTE,
    FRIGATE,
    DESTROYER,
    CRUISER,
    EN_FIGHTER,
    EN_CORVETTE,
    EN_FRIGATE,
    EN_DESTROYER,
    EN_CRUISER,
    SHIP_TYPES
};

enum OtherSprites
{
    LASER_ALLIED,
    LASER_ENEMY,
    LASER_EXPLODE,
    SHIP_PORTING,
    OTHER_TOTAL
};

// I define it here because I wouldn't know where to otherwise
const double pi = 3.141592;

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int SPRITE_WIDTH = 40;
const int SPRITE_HEIGHT = 40;

class Graphics {
private:
    SDL_Window* gWindow;
    SDL_Renderer* gRenderer;
    
    SDL_Texture* gTexture[TEXTURES_TOTAL];
    SDL_Texture* gSprites[SHIP_TYPES];
    SDL_Texture* gOther[OTHER_TOTAL];
    
    SDL_Texture* gScore;
    SDL_Texture* gPowerups;
    
    TTF_Font* gFont;
    
    Mix_Music *sStage;
    Mix_Music *sBoss;
    Mix_Music *sVictory;
    
    Mix_Chunk *sZap;
    Mix_Chunk *sBoom;
    Mix_Chunk *sPortIn;
    Mix_Chunk *sPortOut;
    
public:
    Graphics();
    // Starts up SDL and creates window
    bool init();
    // Loads media
    bool loadMedia();
    // Frees media and shuts down SDL
    void close();
    // Loads a texture and assigns it to an element in a texture array
    SDL_Texture* loadTexture( string path);
    // Texture from text
    SDL_Texture* loadFromRenderedText(string textureText, SDL_Color textColor);
    // Sets camera view to choice
    void setView (Screens choice, Screens &lastDisplayed);
    // Writes something from text on screen. Gives some trouble to close apparently.
    void printTextOnScreen (string txtInput, SDL_Rect* destination);
    // Printing an OtherSprite on the screen
    void printOtherOnScreen(OtherSprites sprtsource, int xCenter, int yCenter, double angle, double zoomFactor, int width = 40, int height = 40);
    
    // We will never be free from getters
    SDL_Window* getWindow();
    SDL_Renderer* getRenderer();
    // Again, when i'll have the multidimensional array or whatever, i'll get this from there
    SDL_Texture* getTexture(Screens texture);
    SDL_Texture* getSprite(Characters sprite);
    SDL_Texture* getOtherSprite(OtherSprites sprite);
    SDL_Texture* getScore();
    SDL_Texture* getPowerups();
    TTF_Font* getFont();
    Mix_Chunk* getZap();
    Mix_Chunk* getBoom();
    Mix_Chunk* getPortIn();
    Mix_Chunk* getPortOut();
    Mix_Music* getMainStage();
    Mix_Music* getBossTheme();
    Mix_Music* getVictoryTheme();
    
    string intToString(int input);    // Necessary for printing score and powerups
    void printScore(int score);
    void printPowerups(int powerups);
};

#endif /* defined(__GiocoLabInf__Graphics__) */
