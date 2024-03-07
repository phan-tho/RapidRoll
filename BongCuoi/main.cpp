#include "Header.h"

int main( int argc, char* argv[] ){
    init();
    loadMedia();
    
    bool quit = false;
    SDL_Event e;
    
    int cnt = 0;

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
        
        if(cnt % (vGEN_BLOCK*(BLOCK_ABOVE_TRAP + 1) ) == 0 && cnt){
            Trap trap;
            Traps.push_back(trap);
        }
        else if(cnt % vGEN_BLOCK == 0){                      // GEN BLOCK AND HEART
            Block block;
            Blocks.push_back(block);
            if( cnt % ( vGEN_BLOCK*(BLOCK_ABOVE_HEART + 1) ) == 0 && cnt){
                heart.assignPos(block.PosX + block.BLOCK_HEIGHT/2 + heart.HEART_WIDTH, block.PosY - heart.HEART_HEIGHT);
            }
        }
        cnt++;
        
        
        for(auto it = Blocks.begin(); it != Blocks.end(); it++)          (*it).move();     // MOVE ALL BLOCKS
        for(auto it = Traps.begin() ; it != Traps.end() ; it++)          (*it).move();     // MOVE ALL TRAPS
        
        //Move the dot
        dot.move( checkCollideBlock(dot) );                                 // DOT
        gDotTexture.render(dot.getX(), dot.getY(), NULL);
                
        heart.move();                                                                       // MOVE HEART
        
        if( !Blocks.empty() && ( *Blocks.begin() ).PosY <= CEILING )           Blocks.pop_front();          // Eliminate block out board
        if( !Traps.empty()  && ( *Traps.begin()  ).PosY <= CEILING )           Traps.pop_front();           // Eliminate trap out board
        
        for(auto it = Blocks.begin(); it != Blocks.end(); it++){                            // RENDER ALL BLOCKS
            gBlock.render((*it).PosX, (*it).PosY, NULL);
        }
            
        for(auto it = Traps.begin(); it != Traps.end(); it++){                              // RENDER ALL TRAPS
            gTrap.render((*it).PosX, (*it).PosY, NULL);
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
    gTrap.loadFromFile("trap.png");
    
}

void close()
{
    //Free loaded images
    gDotTexture.freeFire();
    gBackground.freeFire();
    
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


