//  Created by Phan Tho on 25/03/2024.


#ifndef BasicMode_h
#define BasicMode_h

#include "Game.h"

class BasicMode: public Game{
public:
    BasicMode();
    
    void Play();
    
    // RENDER ITEM NOT MOVE AND PAUSE MUSIC
    void handleWhenPause();
    
    // PROCESSING
    void handleWhenPlay();
    // GEN, MOVE ITEM AND CHECK COLLDE
    // AUTO MOVE
    
    void handleWhenDie();
    // RENDER ITEM WITHIN 1.5S AND RENDER GAMEOVER. HANDLE MUSIC
    
    void resetParameter();
    // INHERIT FROM GAME. RESET PARAMETER WHEN REPLAY
    
private:
    Dot dot;
    Heart heart;
    Fuel fuel;
    
    int score;
    // SCORE INCREASE WHEN MOVE
    int life;
    // DIE WHEN LIFE = 0
    
    int idNearBlock;
    int idNearTrap;
    
    int cntTime;
    // TIME DELAY WITHIN 1.5S
    // TIME_DELAY = 90
    
    // Musical
    enum Channel{
        FIRST_CHANNEL = 0,
        SECOND_CHANNEL = 1
    };
};

void BasicMode::Play(){
//    Play Background Music
    Mix_PlayMusic(gBackGrMusic, -1);

    bool quit = false;
    SDL_Event e;

    while( !quit ){
        while( SDL_PollEvent( &e ) != 0 ){
            if( e.type == SDL_QUIT || e.key.keysym.sym == SDLK_x)        quit = true;
            
            //Handle input for the dot
            dot.handleEvent( e, DENTA_Y );
            OptionInGame.handleEvent( &e );
            
            if (OptionInGame.mCurrentState[EXIT]){   quit = true;   }        // EXIT
        }
        
        if(OptionInGame.mCurrentState[REPLAY]){                     // RESET PARAMETER WHEN REPLACE
            resetParameter();
            OptionInGame.mCurrentState[PAUSE] = false;
            OptionInGame.mCurrentState[REPLAY] = false;
            
//            PLAY ONLY BACKGROUND MUSIC
            Mix_PlayMusic(gBackGrMusic, -1);
        }
        
        SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
        SDL_RenderClear( gRenderer );
        
        gBackground.render(0, 0, NULL);             // BACKGROUND
        OptionInGame.render();                      // OPTION PAUSE PLAY REPLAY EXIT
        renderLifeAndScore(score, life);
        
        if(OptionInGame.mCurrentState[PAUSE]){          // PAUSE
            handleWhenPause();
            continue;
        }
        
        // is music is paused, resume music
        if(Mix_PausedMusic()){
            Mix_ResumeMusic();
        }
        
        if(life > 0){               // WHEN PAUSE = FALSE
            handleWhenPlay();
        }
                                            // GAME OVER
        else if(life == 0){
            handleWhenDie();
        }

        //Update screen
        SDL_RenderPresent( gRenderer );
        
    }
}

void BasicMode::handleWhenPlay(){
    genItem(fuel, heart);
    moveBlocksAndTraps();
    removeItemOutBoard();
    renderBlocksAndTraps();


    // PROCESSING HEART -->
    moveHeart(heart);
    renderHeart(heart);
    
    moveFuel(fuel);
    renderFuel(fuel);
    
    idNearBlock = findNearestBlock(dot, Blocks);
    idNearTrap  = findNearestTrap(dot, Traps);
    
    dot.autoMove(DENTA_Y, idNearBlock, idNearTrap, Blocks, Traps);
    
    moveBall(dot, idNearBlock);
    renderBall(dot);                               // MOVE AND RENDER BALL
    renderEnergyBar(dot);
    
    updateScoreAndDentaY(score);
    
    checkLifeBall(dot, heart, fuel, life);     // DIE OR EAT HEART
    // APPEAR AFTER 1 SECOND
}

void BasicMode::handleWhenPause(){
    renderBlocksAndTraps();
    renderHeart(heart);
    renderFuel(fuel);
    if(life){
        renderBall(dot);
        renderEnergyBar(dot);
    }
    else{
        gGameOver.render(54, 310, NULL);
    }

    OptionInGame.render();
    
    // pause music
    Mix_PauseMusic();
    if( Mix_Playing(FIRST_CHANNEL) || Mix_Playing(SECOND_CHANNEL) ){
        Mix_HaltChannel(FIRST_CHANNEL);
        Mix_HaltChannel(SECOND_CHANNEL);
    }
    
    SDL_RenderPresent( gRenderer );
}

void BasicMode::handleWhenDie(){
    if(cntTime++ < TIME_DELAY){
        moveBlocksAndTraps();
        removeItemOutBoard();
        renderBlocksAndTraps();

        moveHeart(heart);
        renderHeart(heart);
        
        moveFuel(fuel);
        renderFuel(fuel);
    }
    else{
        renderFuel(fuel);
        renderBlocksAndTraps();
        renderHeart(heart);
        
        gGameOver.render(54, 310, NULL);            // MAGIC
        OptionInGame.mCurrentState[PAUSE] = true;
        
        // pause music
        Mix_PauseMusic();
        if( Mix_Playing(FIRST_CHANNEL) || Mix_Playing(SECOND_CHANNEL) ){
            Mix_HaltChannel(FIRST_CHANNEL);
            Mix_HaltChannel(SECOND_CHANNEL);
        }
    }
}

void BasicMode::resetParameter(){
    Game::resetParameter();             // inherit from Game
    
    life  = 3;
    score = 0;
    
    dot.reset();

    heart.reset();

    fuel.reset();
}

BasicMode::BasicMode(){
    Block::dentaX = 1;
    Block::staticAboveDyn = 0;
    
    Trap::dentaX = 1;
    Trap::staticAboveDyn = 3;
    
    life = 3;
    score = 0;
    
    cntTime = 0;
    
    idNearBlock = 0;
    idNearTrap = 0;
}

#endif /* BasicMode_h */
