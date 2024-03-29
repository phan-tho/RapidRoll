//  Created by Phan Tho on 27/03/2024.

#ifndef ShootingMode_h
#define ShootingMode_h

#include "Game.h"
#include "ITEM/ballWithGun.h"
#include "ITEM/autoBall.h"
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
    
    void resetParameter();
    
private:
    BallWithGun playerBall;
    autoBall    enemyBall;
    
    Heart heart;
    Fuel fuel;
    
    int score;
    // SCORE INCREASE WHEN MOVE
    int life;
    // DIE WHEN LIFE = 0
    
    int idNearBlock;
    int idNearTrap;
    
    int cntTime;
    // TIME_DELAY = 90
    
    int reloadTime;
    // reload Time = 0 ==> can fire
    
};


void ShootingMode::Play(){
////    Play Background Music
//    Mix_PlayMusic(gBackGrMusic, -1);

    bool quit = false;
    SDL_Event e;

    while( !quit ){
        while( SDL_PollEvent( &e ) != 0 ){
            if( e.type == SDL_QUIT || e.key.keysym.sym == SDLK_x){
                quit = true;
//                ball.close();
                playerBall.close();     // FREE TEXTURE
                enemyBall.close();
            }
            
            
            // HANDLE VELOCITY OF BALL AND GENERATE BULLET WHEN CLICK MOUSE
            playerBall.handleEvent(e, DENTA_Y, SDLK_s, SDLK_a, SDLK_d);
            playerBall.genBullet(&e);
            
            OptionInGame.handleEvent( &e );
            
            if (OptionInGame.mCurrentState[EXIT]){
                quit = true;
                playerBall.close();
                enemyBall.close();
            }        // EXIT
        }
        
        if(OptionInGame.mCurrentState[REPLAY]){                     // RESET PARAMETER WHEN REPLACE
            resetParameter();
            OptionInGame.mCurrentState[PAUSE] = false;
            OptionInGame.mCurrentState[REPLAY] = false;
            
////            PLAY ONLY BACKGROUND MUSIC
//            Mix_PlayMusic(gBackGrMusic, -1);
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
        
//        // is music is paused, resume music
//        if(Mix_PausedMusic()){
//            Mix_ResumeMusic();
//        }
        
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
    
    // AUTO MOVE ENEMY BALL
    // FIND ID IN DEQUE OF NEAREST TRAP AND BLOCK
    idNearBlock = findNearestBlock(enemyBall, Blocks);
    idNearTrap  = findNearestTrap(enemyBall, Traps);
    
    // ball.autoMove(DENTA_Y, idNearBlock, idNearTrap, Blocks, Traps);
    enemyBall.autoMove(DENTA_Y, idNearBlock, idNearTrap, Blocks, Traps);
    
    // HANDLE ENEMY BALL
    moveBall(enemyBall, idNearBlock);
    enemyBall.render();
    enemyBall.renderEnergyBar();
//    renderBall(ball);                               // MOVE AND RENDER BALL
//    renderEnergyBar(ball);
    
    // HANDLE PLAYER BALL
    moveBall(playerBall, findNearestBlock(playerBall, Blocks));
    playerBall.handleBullet(Traps, Blocks);
    playerBall.renderBall();
    playerBall.renderGun();
    playerBall.renderEnergyBar();
    
    // HANDLE INFOR BAR
    updateScoreAndDentaY(score);
    
    // UPDATE LIFE
    checkLifeBall(enemyBall, heart, fuel, life, idNearTrap);     // DIE OR EAT HEART
    checkLifeBall(playerBall, heart, fuel, life, findNearestTrap(playerBall, Traps));
    // APPEAR AFTER 1 SECOND
}

void ShootingMode::handleWhenPause(){
    renderBlocksAndTraps();
    renderHeart(heart);
    renderFuel(fuel);
    if(life){
//        renderBall(ball);
//        renderEnergyBar(ball);
        playerBall.renderBall();
        playerBall.renderGun(true);
        playerBall.renderEnergyBar();
        
        enemyBall.render();
        enemyBall.renderEnergyBar();
    }
    else{
        gGameOver.render(54, 310, NULL);
    }

    OptionInGame.render();
    
//    // pause music
//    Mix_PauseMusic();
//    if( Mix_Playing(FIRST_CHANNEL) || Mix_Playing(SECOND_CHANNEL) ){
//        Mix_HaltChannel(FIRST_CHANNEL);
//        Mix_HaltChannel(SECOND_CHANNEL);
//    }
    
    SDL_RenderPresent( gRenderer );
}

void ShootingMode::handleWhenDie(){
    if(cntTime++ < TIME_DELAY){
        moveBlocksAndTraps();
        removeItemOutBoard();
        renderBlocksAndTraps();

        moveHeart(heart);
        renderHeart(heart);
        
        enemyBall.render();
        enemyBall.renderEnergyBar();
        
        moveFuel(fuel);
        renderFuel(fuel);
    }
    else{
        renderFuel(fuel);
        renderBlocksAndTraps();
        renderHeart(heart);
        
        enemyBall.render();
        enemyBall.renderEnergyBar();
        
        gGameOver.render(54, 310, NULL);            // MAGIC
        OptionInGame.mCurrentState[PAUSE] = true;
        
//        // pause music
//        Mix_PauseMusic();
//        if( Mix_Playing(FIRST_CHANNEL) || Mix_Playing(SECOND_CHANNEL) ){
//            Mix_HaltChannel(FIRST_CHANNEL);
//            Mix_HaltChannel(SECOND_CHANNEL);
//        }
    }
}

void ShootingMode::resetParameter(){
    Game::resetParameter();             // inherit from Game
    
    life  = 3;
    score = 0;
    
    playerBall.reset();
    enemyBall.reset();

    heart.reset();

    fuel.reset();
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
