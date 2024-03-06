#ifndef def_h
#define def_h

#include "SDL2/SDL.h"
#include "SDL2_image/SDL_image.h"
#include <string>
#include <iostream>

const int SCREEN_WIDTH = 440;           // 640
const int SCREEN_HEIGHT = 680;          // 480

SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;

void init();
void loadMedia();
void close();

void init(){
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

