#include "Header.h"

int main( int argc, char* argv[] ){
    init();
    loadMedia();
    
    Game game;
    game.Play();
    
    close();

    return 0;
}

void loadMedia(){
    gBackground.loadFromFile("BackGr.png");
    
    gDotTexture.loadFromFile( "Ball.png" );
    
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
    
    gButtonControlGame[PAUSE].loadFromFile("Pause.png");
    gButtonControlGame[PLAY].loadFromFile("Play.png");
    gButtonControlGame[REPLAY].loadFromFile("Replay.png");
    gButtonControlGame[EXIT].loadFromFile("Exit.png");
    
    gFont = TTF_OpenFont("Sears_Tower.ttf", 30);
}

void close()
{
    //Free loaded images
    gDotTexture.freeFire();
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
    SDL_Quit();
}


