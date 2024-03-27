#include "Header/Header.h"
#include "Header/Game.h"
#include "Header/BasicMode.h"

int main( int argc, char* argv[] ){
    init();
    loadMedia();
    
    BasicMode basicMode;
    basicMode.Play();
    
    close();
    return 0;
}

void loadMedia(){
    gBackground.loadFromFile("BackGr.png");
//    gBackground.loadFromFile("/Users/mac/Documents/Code/Project/RapidRoll/BongCuoi/Image/BackGr.png");
    
    gBallTexture.loadFromFile( "Ball.png" );
    
    gFire[0].loadFromFile("fire1.png");
    gFire[1].loadFromFile("fire2.png");
    gFire[2].loadFromFile("fire3.png");
    gFire[3].loadFromFile("fire4.png");
    gFire[4].loadFromFile("fire5.png");
    gFire[5].loadFromFile("fire6.png");
    
    gGameOver.loadFromFile("GameOver.png");
    
    gBlock.loadFromFile("Block.png");
    gHeart.loadFromFile("tim.png");
    gTrap.loadFromFile("trap.png");
    
    gFuel.loadFromFile("fuel.png");
    gEnergyBar.loadFromFile("energyBar.png");
    
    gButtonControlGame[PAUSE].loadFromFile("Pause.png");
    gButtonControlGame[PLAY].loadFromFile("Play.png");
    gButtonControlGame[REPLAY].loadFromFile("Replay.png");
    gButtonControlGame[EXIT].loadFromFile("Exit.png");
    
    gFont = TTF_OpenFont("Sears_Tower.ttf", 30);
    
    gBackGrMusic = Mix_LoadMUS("BackGrMusic.wav");
    gMusicWhenMove = Mix_LoadWAV("fireMove.wav");
    gTailFireMove = Mix_LoadWAV("tailFireMove.wav");
    
    Mix_VolumeChunk(gMusicWhenMove, MIX_MAX_VOLUME/16);
    Mix_VolumeChunk(gTailFireMove, MIX_MAX_VOLUME/16);
    
    Mix_VolumeMusic(MIX_MAX_VOLUME/16);
}

void close()
{
    Mix_FreeMusic(gBackGrMusic);
    gBackGrMusic = NULL;
    
    Mix_FreeChunk(gMusicWhenMove);
    gMusicWhenMove = NULL;
    
    Mix_FreeChunk(gTailFireMove);
    gTailFireMove = NULL;
    
    //Free loaded images
    gBallTexture.freeFire();
    gBackground.freeFire();
    
    gFire[0].freeFire();
    gFire[1].freeFire();
    gFire[2].freeFire();
    gFire[3].freeFire();
    gFire[4].freeFire();
    gFire[5].freeFire();

    gButtonControlGame[0].freeFire();
    gButtonControlGame[1].freeFire();
    gButtonControlGame[2].freeFire();
    gButtonControlGame[3].freeFire();

    gGameOver.freeFire();
    
    gBlock.freeFire();
    gHeart.freeFire();
    gTrap.freeFire();
    
    gFuel.freeFire();
    gEnergyBar.freeFire();
    
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

























