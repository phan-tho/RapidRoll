#include "Header.h"

int checkLife(Dot& dot, Heart& heart){
    if( checkCollideTrap(dot) || dot.mPosY <= CEILING || dot.mPosY + dot.DOT_HEIGHT >= FLOOR){
        dot = Dot();
//        dot.mVelX = 0;
//        std::cout << "Pos in func" << dot.mPosY << "\n";
//        std::cout << (checkCollideTrap(dot) ? "Trap\n" : "NOT TRAP\n");
//        std::cout << (dot.mPosY <= CEILING ? "Ceiling\n" : "Not ceiling\n");
//        std::cout << (dot.mPosY + dot.DOT_HEIGHT >= FLOOR ? "FLOOR?" : "Not floor\n");
        return -1;
    }
    if( checkCollideHeart(dot, heart) ){
        heart.isEaten = true;
        heart.PosX = 0;
        heart.PosY = 0;
        return 1;
    }
    return 0;
}

int main( int argc, char* argv[] ){
    init();
    loadMedia();
    
    bool quit = false;
    SDL_Event e;
    
    int cnt = 0;
    int life = 1;

    Dot dot;
    Heart heart;
    
    std::cout << "posy : " << dot.mPosY << "\n";

    while( !quit ){
        while( SDL_PollEvent( &e ) != 0 ){
            if( e.type == SDL_QUIT || e.key.keysym.sym == SDLK_x)        quit = true;

            //Handle input for the dot
            dot.handleEvent( e );
        }
        
        //Clear screen
        SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
        SDL_RenderClear( gRenderer );

        //Render objects
        gBackground.render(0, 0, NULL);             // BACKGROUND
        
        if(life > 0){
            
            
            // GENERATE BLOCKS AND TRAPS --------------------------------------------------------------------------------GEN
            if(cnt % (vGEN_BLOCK*(BLOCK_ABOVE_TRAP + 1) ) == 0 && cnt){
                Trap trap;
                Traps.push_back(trap);
            }
            else if(cnt % vGEN_BLOCK == 0){                      // GEN BLOCK AND HEART
                Block block;
                Blocks.push_back(block);
                if( cnt % ( vGEN_BLOCK*(BLOCK_ABOVE_HEART + 1) ) == 0 && cnt){
                    heart.assignPos(block.PosX + block.BLOCK_HEIGHT/2 + heart.HEART_WIDTH, block.PosY - heart.HEART_HEIGHT);
                    heart.isEaten = false;
                }
            }
            cnt++;
            //END --------------------------------------------------------------------------------------------------------END
            
            
            // MOVE TRAP AND BLOCK GOING UP -----------------------------------------------------------------------------MOVE
            for(auto it = Blocks.begin(); it != Blocks.end(); it++)          (*it).move();     // MOVE ALL BLOCKS
            for(auto it = Traps.begin() ; it != Traps.end() ; it++)          (*it).move();     // MOVE ALL TRAPS
            //END --------------------------------------------------------------------------------------------------------END
            
            
            
            // CHECK LIFE AND MOVE THE BALL------------------------------------------------------------------------CHECK LIFE
            life += checkLife(dot, heart);                                      // CHECK LIFE OF BALL
            dot.move( checkCollideBlock(dot) );                                 // DOT
            gDotTexture.render(dot.getX(), dot.getY(), NULL);
            //END --------------------------------------------------------------------------------------------------------END
            
            
            // REMOVE TRAP AND BLOCK WHICH OUT BOARD-------------------------------------------------------------------REMOVE
            if( !Blocks.empty() && ( *Blocks.begin() ).PosY <= CEILING )           Blocks.pop_front();
            if( !Traps.empty()  && ( *Traps.begin()  ).PosY <= CEILING )           Traps.pop_front();
            //END --------------------------------------------------------------------------------------------------------END
            
            
            // RENDER ALL BLOCKS ADN TRAPS BEEING ON BOARD-------------------------------------------------------------RENDER
            for(auto it = Blocks.begin(); it != Blocks.end(); it++){                            // RENDER ALL BLOCKS
                gBlock.render((*it).PosX, (*it).PosY, NULL);
            }
            
            for(auto it = Traps.begin(); it != Traps.end(); it++){                              // RENDER ALL TRAPS
                gTrap.render((*it).PosX, (*it).PosY, NULL);
            }
            //END --------------------------------------------------------------------------------------------------------END
            
            // MOVE HEART. THIS PROCESS CAN ABUNDANT
            heart.move();
            if(heart.PosY >= CEILING && !heart.isEaten)     gHeart.render(heart.PosX, heart.PosY, NULL);
        }

                                                // GAME OVER
        // DONNOT CLEAR EX-RENDER
        
        else{
            if(!Blocks.empty() || !Traps.empty()){
                
                // MOVE TRAP AND BLOCK GOING UP -----------------------------------------------------------------------------MOVE
                for(auto it = Blocks.begin(); it != Blocks.end(); it++)          (*it).move();     // MOVE ALL BLOCKS
                for(auto it = Traps.begin() ; it != Traps.end() ; it++)          (*it).move();     // MOVE ALL TRAPS
                //END --------------------------------------------------------------------------------------------------------END
                
                // REMOVE TRAP AND BLOCK WHICH OUT BOARD-------------------------------------------------------------------REMOVE
                if( !Blocks.empty() && ( *Blocks.begin() ).PosY <= CEILING )           Blocks.pop_front();
                if( !Traps.empty()  && ( *Traps.begin()  ).PosY <= CEILING )           Traps.pop_front();
                //END --------------------------------------------------------------------------------------------------------END
                
                
                // RENDER ALL BLOCKS ADN TRAPS BEEING ON BOARD-------------------------------------------------------------RENDER
                for(auto it = Blocks.begin(); it != Blocks.end(); it++){                            // RENDER ALL BLOCKS
                    gBlock.render((*it).PosX, (*it).PosY, NULL);
                }
                
                for(auto it = Traps.begin(); it != Traps.end(); it++){                              // RENDER ALL TRAPS
                    gTrap.render((*it).PosX, (*it).PosY, NULL);
                }
                //END --------------------------------------------------------------------------------------------------------END
                
                // MOVE HEART. THIS PROCESS CAN ABUNDANT
                heart.move();
                if(heart.PosY >= CEILING && !heart.isEaten)     gHeart.render(heart.PosX, heart.PosY, NULL);
                
            }
            else{
                gGameOver.render(54, 310, NULL);
            }
        }

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


