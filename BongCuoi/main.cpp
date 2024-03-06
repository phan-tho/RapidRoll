#include "def.h"
#include "ball.h"
#include "LTexture.h"
#include <iostream>

LTexture gDotTexture;
LTexture gBackground;

int main( int argc, char* argv[] ){
    init();
    loadMedia();
    
    bool quit = false;
    SDL_Event e;

//    The dot that will be moving around on the screen
    Dot dot;

    while( !quit ){
        while( SDL_PollEvent( &e ) != 0 ){
            if( e.type == SDL_QUIT || e.key.keysym.sym == SDLK_x)        quit = true;

            //Handle input for the dot
            dot.handleEvent( e );
        }

        //Move the dot
        dot.move();

        //Clear screen
        SDL_SetRenderDrawColor( gRenderer, 0x00, 0xFF, 0xFF, 0xFF );
        SDL_RenderClear( gRenderer );

        //Render objects
        gBackground.render(0, 0, NULL);
        gDotTexture.render(dot.getX(), dot.getY(), NULL);

        //Update screen
        SDL_RenderPresent( gRenderer );
    }
    
    close();

    return 0;
}

void loadMedia(){
    //Load dot texture
    gDotTexture.loadFromFile( "dot.bmp" );
    gBackground.loadFromFile("background.png");
}

void close()
{
    //Free loaded images
    gDotTexture.freeFire();
    gBackground.freeFire();

    //Destroy window
    SDL_DestroyRenderer( gRenderer );
    SDL_DestroyWindow( gWindow );
    gWindow = NULL;
    gRenderer = NULL;

    //Quit SDL subsystems
    IMG_Quit();
    SDL_Quit();
}


