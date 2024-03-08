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
    //Load dot texture
    gDotTexture.loadFromFile( "dot.bmp" );
    gBackground.loadFromFile("BackGr.png");
    
    gGameOver.loadFromFile("GameOver.png");
    
    gBlock.loadFromFile("Block.png");
    gHeart.loadFromFile("tim.png");
    gTrap.loadFromFile("trap.png");
    
}

void close()
{
    //Free loaded images
    gDotTexture.freeFire();
    gBackground.freeFire();
    
    gGameOver.freeFire();
    
    gBlock.freeFire();
    gHeart.freeFire();
    gTrap.freeFire();

    //Destroy window
    SDL_DestroyRenderer( gRenderer );
    SDL_DestroyWindow( gWindow );
    gWindow = NULL;
    gRenderer = NULL;

    //Quit SDL subsystems
    IMG_Quit();
    SDL_Quit();
}


