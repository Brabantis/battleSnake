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

#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#include <SDL2_ttf/SDL_ttf.h>

using namespace std;

// These enums are used throughout the game

enum Direction {NORTH, EAST, SOUTH, WEST, DIRECTION_TOTAL};

enum Screens
{
    MAIN_CAMERA,
    LOWER_MENU,
    MAIN_MENU,
    BATTLE,
    NARRATION,
    SELECTION_MENU,
    TEXTURES_TOTAL
};

enum Characters
{
    YOUNGSTER,
    OLD_LADY,
    HIPPIE,
    BULLY,
    VANDAL,
    NERD,
    KARATEKA,
    SNOB,
    SPRITES_TOTAL
};

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int SPRITE_WIDTH = SCREEN_WIDTH/20;
const int SPRITE_HEIGHT = SCREEN_HEIGHT/8;

class Graphics {
private:
    SDL_Window* gWindow;
    SDL_Renderer* gRenderer;
    
    // NOOOOOOOOOOO I need a bidimensional array somehow. Therefore I will be able to call a sprite from gTexture[sprite][Direction] or something like that
    SDL_Texture* gTexture[TEXTURES_TOTAL];
    SDL_Texture* gSprites[SPRITES_TOTAL][DIRECTION_TOTAL];
    TTF_Font* gFont;
    
public:
    Graphics();
    //Starts up SDL and creates window
    bool init();
    //Loads media
    bool loadMedia();
    //Frees media and shuts down SDL
    void close();
    //To apply upper level HUD-like GUI
    void copyLowerLayerTexture(Screens last, bool lower, bool upper);
    //loads a texture and assigns it to an element in a texture array
    SDL_Texture* loadTexture( string path);
    //Texture from text
    SDL_Texture* loadFromRenderedText(string textureText, SDL_Color textColor);
    //Adds a partial texture on top of another
    void addSubMenu(Screens choice, Screens lastDisplayed, bool &isLMD, bool &isND);
    //Sets camera view to choice
    void setView (Screens choice, Screens &lastDisplayed, bool &isLMD, bool &isND);
    //Writes something from text on screen. Gives some trouble to close apparently.
    void printTextOnScreen (string txtInput, SDL_Rect* destination);
    
    // We will never be free from getters
    SDL_Window* getWindow();
    SDL_Renderer* getRenderer();
    // Again, when i'll have the multidimensional array or whatever, i'll get this from there
    SDL_Texture* getTexture(Screens texture);
    SDL_Texture* getSprites(Characters sprite, Direction orient);
    TTF_Font* getFont();
};

#endif /* defined(__GiocoLabInf__Graphics__) */
