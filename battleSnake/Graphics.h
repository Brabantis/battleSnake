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
    MAIN_MENU,
    MAIN_CAMERA,
    GAME_OVER,
    ADMIRAL_SPEAK,
    CHANCELLOR_SPEAK,
    END_GAME,
    TEXTURES_TOTAL
};

enum Characters
{
    FIGHTER,
    EN_FIGHTER,
    EN_CORVETTE,
    SHIP_TYPES
};

enum OtherSprites
{
    LASER_ALLIED,
    LASER_ENEMY,
    LASER_EXPLODE,
    SHIP_EXPLODE_1,
    SHIP_EXPLODE_2,
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
    SDL_Window* gWindow = nullptr;
    SDL_Renderer* gRenderer = nullptr;
    
    SDL_Texture* gTexture[TEXTURES_TOTAL];
    SDL_Texture* gSprites[SHIP_TYPES];
    SDL_Texture* gOther[OTHER_TOTAL];
    
    SDL_Texture* gScore = nullptr;
    SDL_Texture* gKills = nullptr;
    
    TTF_Font* gFont = nullptr;
    
    Mix_Music *sStage = nullptr;
    Mix_Music *sBoss = nullptr;
    Mix_Music *sVictory = nullptr;
    Mix_Music *sMenu = nullptr;
    
    Mix_Chunk *sZap = nullptr;
    Mix_Chunk *sBoom = nullptr;
    Mix_Chunk *sPortIn = nullptr;
    Mix_Chunk *sPortOut = nullptr;
    
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
    void setView (Screens choice);
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
    SDL_Texture* getkills();
    
    TTF_Font* getFont();
    
    Mix_Chunk* getZap();
    Mix_Chunk* getBoom();
    Mix_Chunk* getPortIn();
    Mix_Chunk* getPortOut();
    
    Mix_Music* getMainTheme();
    Mix_Music* getBossTheme();
    Mix_Music* getVictoryTheme();
    Mix_Music* getMenuTheme();
    
    string intToString(int input);    // Necessary for printing score and kills
    // These numbers are actually the result of careful calculations
    void printScore(int score, int x = 40, int y = 10);
    void printkills(int kills, int x = 580, int y = 10);
};

#endif /* defined(__GiocoLabInf__Graphics__) */
