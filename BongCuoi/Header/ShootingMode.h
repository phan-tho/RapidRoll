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
    
    void ballBoundWhenCollide(Ball& ball1, Ball& ball2);
    
private:
    BallWithGun playerBall;
    autoBall    enemyBall;
    
    Heart heart;
    Fuel fuel;
    
    int score;
    // SCORE INCREASE WHEN MOVE
    
    int idNearBlock;
    int idNearTrap;
    
    int cntTime;
    // TIME_DELAY = 90
    
    int reloadTime;
    // reload Time = 0 ==> can fire
    
    bool isAuto;
    
    void close();
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
                close();
            }
            
            // HANDLE VELOCITY OF BALL AND GENERATE BULLET WHEN CLICK MOUSE
            playerBall.handleEvent(e, DENTA_Y);
            playerBall.genBullet(&e);
            if(!isAuto)     enemyBall.handleEvent(e, DENTA_Y);
            
            OptionInGame.handleEvent( &e );
            
            if (OptionInGame.mCurrentState[EXIT]){
                quit = true;
                close();
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
        renderLifeAndScore(score, playerBall.life);
        
        if(OptionInGame.mCurrentState[PAUSE]){          // PAUSE
            handleWhenPause();
            continue;
        }
        
//        // is music is paused, resume music
//        if(Mix_PausedMusic()){
//            Mix_ResumeMusic();
//        }
        
        if(playerBall.life > 0){               // WHEN PAUSE = FALSE
            handleWhenPlay();
        }
                                            // GAME OVER
        else if(playerBall.life == 0){
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
    if(isAuto){
        enemyBall.autoMove(DENTA_Y, idNearBlock, idNearTrap, Blocks, Traps);
    }
    
    // HANDLE ENEMY BALL
    moveBall(enemyBall, idNearBlock);
    enemyBall.render();
    enemyBall.renderEnergyBar();
    
    // HANDLE PLAYER BALL
    moveBall(playerBall, findNearestBlock(playerBall, Blocks));
    playerBall.handleBullet(enemyBall, Traps, Blocks, score);
    ballBoundWhenCollide(playerBall, enemyBall);
    
    playerBall.renderBall();
    playerBall.renderGun();
    playerBall.renderEnergyBar();
    
    // HANDLE INFOR BAR
//    updateScoreAndDentaY(score);
    
    // UPDATE LIFE
    checkLifeBall(enemyBall, heart, fuel, enemyBall.life, idNearTrap);     // DIE OR EAT HEART
    checkLifeBall(playerBall, heart, fuel, playerBall.life, findNearestTrap(playerBall, Traps));
    // APPEAR AFTER 1 SECOND
}

void ShootingMode::handleWhenPause(){
    renderBlocksAndTraps();
    renderHeart(heart);
    renderFuel(fuel);
    if(playerBall.life){
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

void ShootingMode::ballBoundWhenCollide(Ball &ball1, Ball &ball2){
    if( ball1.mPosX > ball2.mPosX - ball1.BALL_WIDTH  &&
        ball1.mPosX < ball2.mPosX + ball2.BALL_WIDTH  &&
        ball1.mPosY > ball2.mPosY - ball1.BALL_HEIGHT &&
        ball1.mPosY < ball2.mPosY + ball2.BALL_HEIGHT ){
        if(ball2.mPosX > ball1.mPosX){
            ball1.mPosX -= DENTA_Y + 1;
            ball2.mPosX += DENTA_Y + 1;
        }
        else{
            ball1.mPosX += DENTA_Y + 1;
            ball2.mPosX -= DENTA_Y + 1;
        }
        if(ball1.mPosY > ball2.mPosY){
            ball1.mPosY += DENTA_Y;
            ball2.mPosY -= DENTA_Y;
        }
        else{
            ball1.mPosY -= DENTA_Y;
            ball2.mPosY += DENTA_Y;
        }
    }
}

void ShootingMode::resetParameter(){
    Game::resetParameter();             // inherit from Game
    
    playerBall.life = 3;
    score = 0;
    
    playerBall.reset();
    playerBall.mPosX -= 50;
    enemyBall.reset();
    enemyBall.mPosX  += 50;

    heart.reset();

    fuel.reset();
}

void ShootingMode::close(){
    playerBall.close();
    enemyBall.close();
}

ShootingMode::ShootingMode(){
    Block::dentaX = 1;
    Block::staticAboveDyn = 4;
    
    Trap::dentaX = 1;
    Trap::staticAboveDyn = 3;
    
    enemyBall.life = 100;
    score = 0;
    
    cntTime = 0;
    reloadTime = 0;
    
    idNearBlock = 0;
    idNearTrap = 0;
    
    playerBall.mPosX -= 50;
    enemyBall.mPosX  += 50;
    
    isAuto = 1;
    
    playerBall.moveUp = SDLK_s;
    playerBall.moveLeft = SDLK_a;
    playerBall.moveRight = SDLK_d;
    
    enemyBall.moveUp = SDLK_s;
    enemyBall.moveLeft = SDLK_a;
    enemyBall.moveRight = SDLK_d;
}

#endif /* ShootingMode_h */
