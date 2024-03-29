//  Created by Phan Tho on 27/03/2024.

#ifndef ShootingMode_h
#define ShootingMode_h

#include "Game.h"
#include "ITEM/ballWithGun.h"
#include "ITEM/Bullet.h"
//#include <cmath>

class ShootingMode : public Game{
public:
    ShootingMode();
    
    void Play();
    
    // PROCESSING 
    void handleWhenPlay();
    // GEN, MOVE ITEM AND CHECK COLLDE
    // AUTO MOVE
    
    void handleWhenPause();
    
    void handleWhenDie();
    // RENDER ITEM WITHIN 1.5S AND RENDER GAMEOVER. HANDLE MUSIC
    
private:
    const int GUN_WIDTH = 41;
    const int GUN_HEIGHT = 16;
    
    BallWithGun ball;
    Heart heart;
    Fuel fuel;
    
    int score;
    // SCORE INCREASE WHEN MOVE
    int life;
    // DIE WHEN LIFE = 0
    
    int idNearBlock;
    int idNearTrap;
    
    int cntTime;
    
    int reloadTime;
    // reload Time = 0 ==> can fire
    
};

void ShootingMode::Play(){
    bool quit = false;
    SDL_Event e;

    while( !quit ){
        while( SDL_PollEvent( &e ) != 0 ){
            if( e.type == SDL_QUIT || e.key.keysym.sym == SDLK_x){
                close();
                quit = true;
            }
            
            //Handle input for the ball
            ball.handleEvent(e, DENTA_Y, SDLK_g, SDLK_a, SDLK_d);
            OptionInGame.handleEvent(&e);
            ball.genBullet(&e);
        }
    
        SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
        SDL_RenderClear( gRenderer );
        
        gBackground.render(0, 0, NULL);             // BACKGROUND
        renderLifeAndScore(score, life);
        
        OptionInGame.render();
        
        if(OptionInGame.mCurrentState[PAUSE]){          // PAUSE
            handleWhenPause();
            continue;
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


void ShootingMode::handleWhenPlay(){
    genItem(fuel, heart);
    moveBlocksAndTraps();
    removeItemOutBoard();
    renderBlocksAndTraps();

    // PROCESSING HEART -->
    moveHeart(heart);
    renderHeart(heart);
    
    moveFuel(fuel);
    renderFuel(fuel);
    
    // FIND ID IN DEQUE OF NEAREST TRAP AND BLOCK
    idNearBlock = findNearestBlock(ball, Blocks);
    idNearTrap  = findNearestTrap(ball, Traps);
    
    ball.handleBullet(Traps, Blocks);
    
    // HANDLE BALL
    moveBall(ball, idNearBlock);
    // RENDER BALL
    
    
    ball.renderBall();
    ball.renderGun();
    
    // HANDLE INFOR BAR
    updateScoreAndDentaY(score);
    
    // UPDATE LIFE
    checkLifeBall(ball, heart, fuel, life, idNearTrap);     // DIE OR EAT HEART
    // APPEAR AFTER 1 SECOND
}

void ShootingMode::handleWhenPause(){
    renderBlocksAndTraps();
    renderHeart(heart);
    renderFuel(fuel);
    if(life){
        ball.renderBall();
        ball.renderGun();
    }
    else{
        gGameOver.render(54, 310, NULL);
    }
    
    ball.handleBullet(Traps, Blocks);

    OptionInGame.render();
    
    SDL_RenderPresent( gRenderer );
}

void ShootingMode::handleWhenDie(){
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

ShootingMode::ShootingMode(){
    Block::dentaX = 1;
    Block::staticAboveDyn = 4;
    
    Trap::dentaX = 1;
    Trap::staticAboveDyn = 3;
    
    life = 3;
    score = 0;
    
    cntTime = 0;
    reloadTime = 0;
    
    idNearBlock = 0;
    idNearTrap = 0;
    
}

#endif /* ShootingMode_h */
