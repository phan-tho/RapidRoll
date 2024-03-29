#ifndef Game_h
#define Game_h

#include "Pause.h"
#include "def.h"
#include "LTexture.h"
#include "defLTexture.h"
#include "defMusic.h"

//#include "Header/ITEM/ITEM.h"
//#include "Header/CheckCollide.h"

class Game{
    public:
        Game();
        
        void Play();
    
        void resetParameter();
        
        void renderLifeAndScore(const int& score, const int& life);
    
        // PROCESSING BLOCKS AND TRAPS -->
        void genItem(Fuel& fuel, Heart& heart);         // BLOCKS TRAPS HEART FUEL
        void moveBlocksAndTraps();
        void removeItemOutBoard();
        void renderBlocksAndTraps();
    
        // PROCESSING HEART -->
        void moveHeart(Heart& heart);
        void renderHeart(const Heart& heart);
        
        // PROCESSING FUEL -->
        void moveFuel(Fuel& fuel);
        void renderFuel(const Fuel& fuel);
        
        void moveBall(Ball& ball, const int& nearestPosBlock);
        void renderBall(Ball& ball);
    
        void renderEnergyBar(Ball& ball);
    
        void checkLifeBall(Ball& ball, Heart& heart, Fuel& fuel, int& life, const int& idNearTrap);     // DIE OR EAT HEART FUEL
    
        void updateScoreAndDentaY(int& score);
    protected:
        int DENTA_Y;
    
        Pause OptionInGame;
    
        std::deque<Block> Blocks;               // distance is 170
        std::deque<Trap>  Traps;
    
        const int TIME_DELAY = 90;                  // 1.5s
    
        // Musical
        enum Channel{
            FIRST_CHANNEL = 0,
            SECOND_CHANNEL = 1
        };
    
    private:
        const int BLOCK_ABOVE_TRAP = 2;             // 4  BLOCK ==> 1 TRAP
        const int BLOCK_ABOVE_HEART = 13;           // 15 BLOCK ==> 1 HEART
        const int BLOCK_ABOVE_FUEL = 18;            // AVOID = BLOCK_ABOVE_HEART

        int waitRevive;

        int cnt;
};

void Game::renderLifeAndScore(const int& score, const int& life){
    std::string str = 'X' + std::to_string(life);
    SDL_Color textColor = {0, 0, 0};
    
    gTextTexture.loadFromRenderedText(str, textColor);
    gTextTexture.render(60, 20, NULL);
    
    str = '0';
    auto leng = (std::to_string(score)).size();
    for(int i = 0; i < LENGTH_SCORE - leng; i++)          str += '0';
    str += std::to_string(score);
    gTextTexture.loadFromRenderedText(str, textColor);
    gTextTexture.render(130, 20, NULL);
}

void Game::genItem(Fuel& fuel, Heart& heart){
    if(cnt % ( (vGEN_BLOCK/DENTA_Y) *(BLOCK_ABOVE_TRAP + 1) ) == 0 && cnt){     // GEN TRAPS
        Trap trap;
        Traps.push_back(trap);
    }
    else if(cnt % (vGEN_BLOCK/DENTA_Y) == 0){                      // GEN BLOCK AND HEART
        Block block;
        Blocks.push_back(block);
        
        if( cnt % ( (vGEN_BLOCK/DENTA_Y)*(BLOCK_ABOVE_FUEL + 1) ) == 0 && cnt){
            fuel.assignPos(block.PosX + block.BLOCK_WIDTH/2 - fuel.FUEL_WIDTH/2, block.PosY - fuel.FUEL_HEIGHT);
            fuel.isEaten = false;                  // CAN DISPLAY AND EATEN BY BALL
            fuel.moveToLeft = 0;
            
            if(block.dynamic){
                fuel.moveToLeft += (block.left ? -block.dentaX : block.dentaX);        // PASS VELOCITY AND DIRECTLY TO HEART
            }
        }
        
        else if( cnt % ( (vGEN_BLOCK/DENTA_Y)*(BLOCK_ABOVE_HEART + 1) ) == 0 && cnt){
            heart.assignPos(block.PosX + block.BLOCK_WIDTH/2 - heart.HEART_WIDTH/2, block.PosY - heart.HEART_HEIGHT);
            heart.isEaten = false;                  // CAN DISPLAY AND EATEN BY BALL
            heart.moveToLeft = 0;
            
            if(block.dynamic){
                heart.moveToLeft += (block.left ? -block.dentaX : block.dentaX);        // PASS VELOCITY AND DIRECTLY TO HEART
            }
        }
    }
    cnt = (cnt + 1)%( (vGEN_BLOCK/DENTA_Y)*(BLOCK_ABOVE_TRAP + 1)*10 );         // AVOID CASE OVERFLOW NUM AND TRAP NOT APPEAR AT TENTH
}


void Game::moveBlocksAndTraps(){
    for(auto it = Blocks.begin(); it != Blocks.end(); it++)          (*it).move(DENTA_Y);     // MOVE ALL BLOCKS                 // Increase DENTA_Y ==> More option
    for(auto it = Traps.begin() ; it != Traps.end() ; it++)          (*it).move(DENTA_Y);     // MOVE ALL TRAPS}
}
    
void Game::removeItemOutBoard(){
    if( !Blocks.empty() && ( *Blocks.begin() ).PosY <= CEILING ){
        Blocks.pop_front();
    }
    if( !Traps.empty()  && ( *Traps.begin()  ).PosY <= CEILING ){
        Traps.pop_front();
    }
}

void Game::renderBlocksAndTraps(){
    for(auto it = Blocks.begin(); it != Blocks.end(); it++){                            // RENDER ALL BLOCKS
        gBlock.render((*it).PosX, (*it).PosY, NULL);
    }
    
    for(auto it = Traps.begin(); it != Traps.end(); it++){                              // RENDER ALL TRAPS
        gTrap.render((*it).PosX, (*it).PosY, NULL);
    }
}

void Game::moveHeart(Heart& heart){
    heart.move(DENTA_Y);
}

void Game::renderHeart(const Heart& heart){
    if(heart.PosY >= CEILING && !heart.isEaten){
        gHeart.render(heart.PosX, heart.PosY, NULL);
    }
}

void Game::moveFuel(Fuel& fuel){
    fuel.move(DENTA_Y);
}

void Game::renderFuel(const Fuel& fuel){
    if(fuel.PosY >= CEILING && !fuel.isEaten){
        gFuel.render(fuel.PosX, fuel.PosY, NULL);
    }
    else if(fuel.PosY + fuel.FUEL_HEIGHT > CEILING){
        SDL_Rect xywh = { 0, CEILING - fuel.PosY, fuel.FUEL_WIDTH, fuel.FUEL_HEIGHT - CEILING + fuel.PosY};
        gFuel.render(fuel.PosX, CEILING, &xywh);
    }
}


void Game::moveBall(Ball& ball, const int& nearestPosBlock){
    ball.move( checkCollideBlock(ball, Blocks, nearestPosBlock), DENTA_Y);                                 // ball
    
//    // play gMusicWhenMove when ball.mVelX != 0
//    if(ball.mVelX != 0){                                                                 // PLAY AND PAUSE MUSIC WHEN MOVE
//        // play when music is paused
//        if(!Mix_Playing(FIRST_CHANNEL)){
//            Mix_PlayChannel(FIRST_CHANNEL, gMusicWhenMove, -1);
//        }
//    }
//    else{
//        // PAUSE MUSIC IN CHANNEL 0 if this channel is playing
//        if(Mix_Playing(FIRST_CHANNEL)){
//            Mix_HaltChannel(FIRST_CHANNEL);
//            // play gTailFireMove
//            Mix_PlayChannel(SECOND_CHANNEL, gTailFireMove, -1);
//        }
//    }
}

void Game::renderBall(Ball& ball){
    ball.render();
}

void Game::renderEnergyBar(Ball& ball){
    ball.renderEnergyBar();
}

void Game::updateScoreAndDentaY(int& score){
    score += DENTA_Y;
    DENTA_Y += (score%20000 == 0);
}

void Game::checkLifeBall(Ball& ball, Heart& heart, Fuel& fuel, int& life, const int& idNearTrap){
    
    if( checkCollideTrap(ball, Traps, idNearTrap) || ball.mPosY <= CEILING || ball.mPosY + ball.BALL_HEIGHT >= FLOOR){
//        dot = Dot();
        ball.mVelX = 0;
        ball.currentState = ball.DOWN;
        
        // BALL WILL APPEAR ABOVE LOWEST BLOCK FROM CEILING---------------------------------------------
        if( !Blocks.empty() ){
            ball.mPosY = Blocks.back().PosY - ball.BALL_HEIGHT;
            ball.mPosX = Blocks.back().PosX + (Blocks.back().BLOCK_WIDTH - ball.BALL_WIDTH)/2;
        }
        life--;
        waitRevive = 1;
        // ---------------------------------------------------------------------------------------- DIED
        
    }
    else if( checkCollideHeart(ball, heart) ){
        heart.isEaten = true;
        heart.PosX = 0;
        heart.PosY = 0;
        life++;
    }
    
    else if( checkCollideFuel(ball, fuel) ){
        fuel.isEaten = true;
        fuel.PosX = 0;
        fuel.PosY = 0;
        ball.energy = (fmin(500, ball.energy + 160));
    }
}

void Game::resetParameter(){
    cnt = 0;

    DENTA_Y = 2;
    waitRevive = 0;

    Blocks.clear();
    Traps.clear();
}

Game::Game(){
    cnt = 0;

    DENTA_Y = 2;
    waitRevive = 0;
}

#endif /* Game_h */
