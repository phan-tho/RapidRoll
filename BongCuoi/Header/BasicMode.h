//  Created by Phan Tho on 25/03/2024.


#ifndef BasicMode_h
#define BasicMode_h

#include "Game.h"
//#include "HandleITEM/ITEM/autoBall.h"

class BasicMode: public Game{
public:
    BasicMode(std::string path);
    
    void Play();
    
protected:
    
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
    
    void close();
private:
    autoBall ball;
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
    
    LTexture AutoTexture[2];
    
    const int BUTTON_AUTO_WIDTH = 140;
    const int BUTTON_AUTO_HEIGHT = 47;
    
    const int POS_X_BUTTON_AUTO = 163;
    const int POS_Y_BUTTON_AUTO = 685;
};

void BasicMode::Play(){
//    Play Background Music
    music.backGrMusic();

    bool quit = false;
    SDL_Event e;

    while( !quit ){
        while( SDL_PollEvent( &e ) != 0 ){
            if( e.type == SDL_QUIT){
                quit = true;
                music.whenTappedButton();
                close();
            }
            
            //Handle input for the ball
            ball.handleEvent(e, DENTA_Y);
            if( OptionInGame.handleTapped(&e) ){          // TAP ANY BUTTON
                music.whenTappedButton();
            }
            
            if (OptionInGame.mCurrentState[EXIT]){
                quit = true;
                close();       // free LTexture
            }        // EXIT
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
        renderLifeAndScore(score, life);
        guide.guide(ball, 0, "s", "a", "d");
        
        if(OptionInGame.mCurrentState[PAUSE]){          // PAUSE
            handleWhenPause();
            continue;
        }
        
        // is music is paused, resume music
        music.resumeBackGrMusic();
        
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


/*
    Generate and remove block, trap, heart, fuel
    Handle ball and auto ball
 */
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
    
    // FIND ID IN DEQUE OF NEAREST TRAP AND BLOCK
    idNearBlock = findNearestBlock(ball, Blocks);
    idNearTrap  = findNearestTrap(ball, Traps);
    
    // AUTO MOVE
    if( OptionInGame.isAuto )    ball.autoMove(DENTA_Y, idNearBlock, idNearTrap, Blocks, Traps);
    
    // HANDLE BALL
    moveBall(ball, idNearBlock);
    renderBall(ball);                               // MOVE AND RENDER BALL
    renderEnergyBar(ball);
    
    // HANDLE INFOR BAR
    updateScoreAndDentaY(score);
    
    // UPDATE LIFE
    checkLifeBall(ball, heart, fuel, life, idNearTrap);     // DIE OR EAT HEART
    // APPEAR AFTER 1 SECOND
}


// Render everything
// Handle music
void BasicMode::handleWhenPause(){
    renderBlocksAndTraps();
    renderHeart(heart);
    renderFuel(fuel);
    if(life){
        renderBall(ball);
        renderEnergyBar(ball);
    }
    else{
//        mGameOver.render(54, 310, NULL);
        renderGameOver(score, BASIC_MODE);
    }

    OptionInGame.render();
    
//     pause music
    music.pauseBackGrMusic();
    music.whenNotMove();
    
    SDL_RenderPresent( gRenderer );
}


// Move within small time
// Render game over and highest score
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
        
        renderGameOver(score, BASIC_MODE);
        
//        mGameOver.render(54, 310, NULL);            // MAGIC
        // 320 x 106
        
        OptionInGame.mCurrentState[PAUSE] = true;
        
//        // pause music
        music.pauseBackGrMusic();
        music.whenNotMove();
    }
}


// Init item when game is replay
void BasicMode::resetParameter(){
    Game::resetParameter();             // inherit from Game
    
    life  = 3;
    score = 0;
    
    ball.reset();

    heart.reset();

    fuel.reset();
}

// close media
void BasicMode::close(){
    Game::close();
    
    AutoTexture[1].freeFire();
    AutoTexture[0].freeFire();
    ball.close();
    music.close();
    OptionInGame.close();
}



BasicMode::BasicMode(std::string path) : ball(path) {
    AutoTexture[1].loadFromFile("OnAuto.png");
    AutoTexture[0].loadFromFile("OffAuto.png");
    
    Block::dentaX = 1;
    Block::staticAboveDyn = 4;
    
    Trap::dentaX = 1;
    Trap::staticAboveDyn = 3;
    
    life = 3;
    score = 0;
    
    cntTime = 0;
    
    idNearBlock = 0;
    idNearTrap = 0;
    
    OptionInGame.isAuto = 0;
    
    ball.moveUp = SDLK_s;
    ball.moveLeft = SDLK_a;
    ball.moveRight = SDLK_d;
}

#endif /* BasicMode_h */
