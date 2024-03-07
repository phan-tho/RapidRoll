#ifndef def_h
#define def_h

#include "SDL2/SDL.h"
#include "SDL2_image/SDL_image.h"
#include <string>
#include <iostream>
// random
#include <cstdlib>
#include <ctime>
#include <random>

#include <deque>


const int SCREEN_WIDTH  = 428;           // 640
const int SCREEN_HEIGHT = 748;          // 480

const int CEILING = 90;
const int FLOOR   = 666;
const int lPIVOT  = 40;
const int rPIVOT  = 390;

const int vGEN_BLOCK = 30;
const int Denta_Y    = 3;

SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;

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
//    TTF_Init();
}


#endif /* def_h */

