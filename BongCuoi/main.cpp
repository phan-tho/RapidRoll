#include "Header/Header.h"
#include "Header/Game.h"
#include "Header/BasicMode.h"
#include "Header/ShootingMode.h"

int main( int argc, char* argv[] ){
    init();
    loadMedia();
    
//    BasicMode basicMode;
//    basicMode.Play();
    ShootingMode shootingMode;
    shootingMode.Play();
    
    close();
    return 0;
}

void loadMedia(){
    gBackground.loadFromFile("BackGr.png");
    
    gGameOver.loadFromFile("GameOver.png");
    
    gBlock.loadFromFile("Block.png");
    gHeart.loadFromFile("tim.png");
    gTrap.loadFromFile("trap.png");
    
    gFuel.loadFromFile("fuel.png");
    
    gFont = TTF_OpenFont("Sears_Tower.ttf", 30);
}

void close(){
    //Free loaded images
    gBackground.freeFire();

    gGameOver.freeFire();
    
    gBlock.freeFire();
    gHeart.freeFire();
    gTrap.freeFire();
    
    gFuel.freeFire();
    
    gTextTexture.freeFire();
    
    TTF_CloseFont(gFont);
    gFont = NULL;

    //Destroy window
    SDL_DestroyRenderer( gRenderer );
    SDL_DestroyWindow( gWindow );
    gWindow = NULL;
    gRenderer = NULL;

    //Quit SDL subsystems
    IMG_Quit();
    TTF_Quit();
    Mix_Quit();
    SDL_Quit();
}

























