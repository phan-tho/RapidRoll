//  Created by Phan Tho on 27/03/2024.

#ifndef ShootingMode_h
#define ShootingMode_h

#include "Game.h"
#include "HandleITEM/ITEM/ballWithGun.h"
#include "HandleITEM/ITEM/autoBall.h"
#include "HandleITEM/ITEM/Bullet.h"
#include "Music/Music.h"

class ShootingMode : public Game{
public:
    ShootingMode(std::string path);
    
    void Play();
    
protected:
    bool handleEvent(SDL_Event e);     // return true to quit
    
    // PROCESSING
    // GEN, MOVE ITEM AND CHECK COLLDE
    // AUTO MOVE
    void handleWhenPlay();
    
    // RENDER ITEM IN SCREEN
    void handleWhenPause();
    
    // RENDER ITEM WITHIN 1.5S AND RENDER GAMEOVER
    // HANDLE MUSIC
    // RENDER HIGH SCORE
    void handleWhenDie();
    
    // RESET ALL PARAMETER WHEN REPLAY
    void resetParameter();
    
    // 2 BALL BOUND WHEN COLLIDE
    void ballBoundWhenCollide(Ball& ball1, Ball& ball2);
    
    // DESTOR MEDIA WHEN NOT USE
    void close();
    
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
};


void ShootingMode::Play(){
//    Play Background Music
    music.backGrMusic();

    bool quit = false;
    SDL_Event e;

    while( !quit ){
        while( SDL_PollEvent( &e ) != 0 ){
            quit = handleEvent(e);
        }
        
        if(OptionInGame.mCurrentState[REPLAY]){                     // RESET PARAMETER WHEN REPLACE
            resetParameter();
            OptionInGame.mCurrentState[PAUSE] = false;
            OptionInGame.mCurrentState[REPLAY] = false;
            
//            PLAY ONLY BACKGROUND MUSIC
            music.backGrMusic();
        }
        
        SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
        SDL_RenderClear( gRenderer );
        
        mBackground.render(0, 0, NULL);             // BACKGROUND
        OptionInGame.render();                      // OPTION PAUSE PLAY REPLAY EXIT
        
        renderLifeAndScore(score, playerBall.life);
        // render guide when start game
        guide.guide(playerBall, 1, "J", "H", "K");
        guide.guide(enemyBall, 0, "S", "A", "D");
        
        if(OptionInGame.mCurrentState[PAUSE]){          // PAUSE
            handleWhenPause();
            continue;
        }
        
        // is music is paused, resume music
        music.resumeBackGrMusic();
        
        if(playerBall.life > 0){               // WHEN PAUSE = FALSE
            handleWhenPlay();
        }
                                            // GAME OVER
        else if(playerBall.life == 0){
            handleWhenDie();
        }

//        Update screen
        SDL_RenderPresent( gRenderer );
        
    }
}

// HANDLE EVENT
// ORIENT BALL
// SHOOT BULLET
// BUTTON AUTO, PAUSE, PLAY, REPLAY, EXIT IS TAPPED
bool ShootingMode::handleEvent(SDL_Event e){
    if( e.type == SDL_QUIT){
        music.whenTappedButton();
        close();
        return true;
    }
    
    // HANDLE VELOCITY OF BALL AND GENERATE BULLET WHEN CLICK MOUSE
    playerBall.handleEvent(e, DENTA_Y);
    playerBall.genBullet(&e);
    enemyBall.handleEvent(e, DENTA_Y);
    
    if( OptionInGame.handleTapped(&e) ){
        music.whenTappedButton();
    }
    
    if (OptionInGame.mCurrentState[EXIT]){
        close();
        return true;
    }        // EXIT
    return false;
}

// GENERATE BLOCK, TRAP, HEART, FUEL
// MOVE ITEM
// HANDLE EVENT FROM KEYBOARD AND MOUSE
// MOVE PLAYER BALL AND ENEMY BALL
// HANDLE BULLET
// CHECK LIFE OF 2 BALLS
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
    
    if(OptionInGame.isAuto){
        enemyBall.autoMove(DENTA_Y, idNearBlock, idNearTrap, Blocks, Traps);
    }
    
    // HANDLE ENEMY BALL
    moveBall(enemyBall, idNearBlock);
    enemyBall.render();
    enemyBall.renderEnergyBar();
    
    // HANDLE PLAYER BALL
    moveBall(playerBall, findNearestBlock(playerBall, Blocks));
    if( playerBall.handleBullet(enemyBall, Traps, Blocks, score) )      music.whenKill();
    ballBoundWhenCollide(playerBall, enemyBall);
    
    playerBall.render();
    playerBall.renderGun();
    playerBall.renderEnergyBar();
    
    // HANDLE INFOR BAR
//    updateScoreAndDentaY(score);
    
    // UPDATE LIFE
    checkLifeBall(enemyBall, heart, fuel, enemyBall.life, idNearTrap);     // DIE OR EAT HEART
    checkLifeBall(playerBall, heart, fuel, playerBall.life, findNearestTrap(playerBall, Traps));
    // APPEAR AFTER 1 SECOND
}

// REDNDER ALL ITEM
// HANDLE EVENT OF 3 BUTTONS: PLAY, REPLAY, EXIT
void ShootingMode::handleWhenPause(){
    renderBlocksAndTraps();
    renderHeart(heart);
    renderFuel(fuel);
    if(playerBall.life){
        playerBall.render();
        playerBall.renderGun(true);
        playerBall.renderEnergyBar();
        
        enemyBall.render();
        enemyBall.renderEnergyBar();
    }
    else{
//        mGameOver.render(54, 310, NULL);
        renderGameOver(score, SHOOTING_MODE);
    }

    OptionInGame.render();
    
    // pause music
    music.pauseBackGrMusic();
    music.whenNotMove();
    
    SDL_RenderPresent( gRenderer );
}

// HANDLE ITEM 1.5S
// PAUSE AND RENDER THEM
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
        
//        mGameOver.render(54, 310, NULL);            // MAGIC
        renderGameOver(score, SHOOTING_MODE);
        OptionInGame.mCurrentState[PAUSE] = true;
        
        // pause music
        music.pauseBackGrMusic();
        music.whenNotMove();
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
    
    playerBall.life = 10;
    score = 0;
    
    playerBall.reset();
    playerBall.mPosX -= 50;
    enemyBall.reset();
    enemyBall.mPosX  += 50;

    heart.reset();

    fuel.reset();
}

void ShootingMode::close(){
    Game::close();
    
    playerBall.close();
    enemyBall.close();
    
    music.close();
    OptionInGame.close();
}

ShootingMode::ShootingMode(std::string path) : playerBall(path), enemyBall("sBall110.png") {
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
    
    OptionInGame.isAuto = 1;
    
    playerBall.moveUp = SDLK_j;
    playerBall.moveLeft = SDLK_h;
    playerBall.moveRight = SDLK_k;
    
    enemyBall.moveUp = SDLK_s;
    enemyBall.moveLeft = SDLK_a;
    enemyBall.moveRight = SDLK_d;
}

#endif /* ShootingMode_h */
