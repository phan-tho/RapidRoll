#include "def.h"
#include "ball.h"
#include "LTexture.h"
#include "Block.h"
#include "Heart.h"
#include <iostream>

LTexture gDotTexture;
LTexture gBackground;

LTexture gBlock;
LTexture gHeart;

int main( int argc, char* argv[] ){
    init();
    loadMedia();
    
    bool quit = false;
    SDL_Event e;
    
    std::deque<Block> Blocks;
    int cnt = 0;
//    The dot that will be moving around on the screen
    Dot dot;
    
    Heart heart;

    while( !quit ){
        while( SDL_PollEvent( &e ) != 0 ){
            if( e.type == SDL_QUIT || e.key.keysym.sym == SDLK_x)        quit = true;

            //Handle input for the dot
            dot.handleEvent( e );
        }

        //Clear screen
        SDL_SetRenderDrawColor( gRenderer, 0x00, 0xFF, 0xFF, 0xFF );
        SDL_RenderClear( gRenderer );

        //Render objects
        gBackground.render(0, 0, NULL);             // BACKGROUND
        
        //Move the dot
        dot.move();                                 // DOT
        gDotTexture.render(dot.getX(), dot.getY(), NULL);
        

        if(cnt % vGEN_BLOCK == 0){                      // GEN BLOCK AND HEART
            Block block;
            Blocks.push_back(block);
            if( cnt % (vGEN_BLOCK*20) == 0 && cnt){
                heart.assignPos(block.PosX + block.BLOCK_HEIGHT/2 + heart.HEART_WIDTH, block.PosY - heart.HEART_HEIGHT);
            }
        }
        cnt++;
        
        for(auto it = Blocks.begin(); it != Blocks.end(); it++)        (*it).move();            // MOVE ALL BLOCKS
    
        heart.move();                                                                           // MOVE HEART
        
        if( ( *Blocks.begin() ).PosY <= CEILING )           Blocks.pop_front();             // eliminate block out board

        for(auto it = Blocks.begin(); it != Blocks.end(); it++){                            // RENDER ALL BLOCKS
            gBlock.render((*it).PosX,  (*it).PosY, NULL);
        }
        
        if(heart.PosY >= CEILING)     gHeart.render(heart.PosX, heart.PosY, NULL);

        //Update screen
        SDL_RenderPresent( gRenderer );
    }
    
    close();

    return 0;
}

void loadMedia(){
    //Load dot texture
    gDotTexture.loadFromFile( "dot.bmp" );
    gBackground.loadFromFile("BackGr.png");
    
    gBlock.loadFromFile("Block.png");
    gHeart.loadFromFile("Heart.png");
    
}

void close()
{
    //Free loaded images
    gDotTexture.freeFire();
    gBackground.freeFire();
    
    gBlock.freeFire();
    gHeart.freeFire();

    //Destroy window
    SDL_DestroyRenderer( gRenderer );
    SDL_DestroyWindow( gWindow );
    gWindow = NULL;
    gRenderer = NULL;

    //Quit SDL subsystems
    IMG_Quit();
    SDL_Quit();
}


