#ifndef def_h
#define def_h

#include "SDL2/SDL.h"
#include "SDL2_image/SDL_image.h"
#include "SDL2_ttf/SDL_ttf.h"
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
//const int Denta_Y    = 3;

const int DENTA_X = 3;              // HORIZINAL VELOCITY OF BALL
//const int DENTA_Y = 2;              // VERTICAL  VELOCITY OF BALL AND WALL

const int BLOCK_ABOVE_TRAP = 3;             // 4  BLOCK ==> 1 TRAP
const int BLOCK_ABOVE_HEART = 30;           // 15 BLOCK ==> 1 HEART

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
    srand(time(0));
    SDL_Init( SDL_INIT_VIDEO );
    
    //Create window
    gWindow = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
    
    //Create vsynced renderer for window
    gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );

    //Initialize renderer color
    SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

    //Initialize PNG loading
    IMG_Init(IMG_INIT_PNG);
    
    //Initialize SDL_ttf
    TTF_Init();                       // WHEN WANT TO LOAD FONT
}


#endif /* def_h */

