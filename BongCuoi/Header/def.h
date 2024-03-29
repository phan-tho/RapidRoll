#ifndef def_h
#define def_h

#include "SDL2/SDL.h"
#include "SDL2_image/SDL_image.h"
#include "SDL2_ttf/SDL_ttf.h"
#include "SDL2_mixer/SDL_mixer.h"
#include <string>
#include <iostream>
// random
#include <cstdlib>
#include <ctime>
#include <random>

#include <deque>

const int SCREEN_WIDTH  = 428;      // Dimensions of origin background image
const int SCREEN_HEIGHT = 748;

const int CEILING = 90;             // Coordinate of 4 corner of board
const int FLOOR   = 666;
const int lPIVOT  = 40;
const int rPIVOT  = 390;

const int vGEN_BLOCK = 60;          // Velocity of processing generate block from floor

const int DENTA_X = 3;              // HORIZINAL VELOCITY OF BALL

const int LENGTH_SCORE = 6;

const int TOTAL_FIRE_TIME_LAPSE = 6;

enum Button{
    PAUSE,
    PLAY,
    REPLAY,
    EXIT,
    TOTAL_BUTTON_IN_GAME,
    NONE
};



/*
    This is important to set DOWN velocity of BALL and UP velocity of BLOCK
    This make sure that BALL and BLOCK can pass through other
*/

SDL_Window*   gWindow = NULL;
SDL_Renderer* gRenderer = NULL;
TTF_Font* gFont = NULL;

void init();
void loadMedia();
void close();

void init(){
    srand(int(time(0)));
    SDL_Init( SDL_INIT_VIDEO | SDL_INIT_AUDIO );
    
    //Create window
    gWindow = SDL_CreateWindow( "BY PHAN THO", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
    
    //Create vsynced renderer for window
    gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );

    //Initialize renderer color
    SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

    //Initialize PNG loading
    IMG_Init(IMG_INIT_PNG);
    
    //Initialize SDL_ttf
    TTF_Init();                       // WHEN WANT TO LOAD FONT
    
    if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0){          // number of channels is 2.
        printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
    }
}


#endif /* def_h */

