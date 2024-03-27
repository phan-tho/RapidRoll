#ifndef ball_h
#define ball_h

#include "Block.h"
#include "Trap.h"

class Dot
{
    public:
        static const int DOT_WIDTH = 20;
        static const int DOT_HEIGHT = 20;
    
        const int FIRE_WIDTH = 36;
        const int FIRE_HEIGHT = 46;
    
        const int ENERGY_BAR_WIDTH = 8;
        const int ENERGY_BAR_HEIGHT = 800;
        const int DISTANCE_BAR_BALL = 15;
    
        enum fireState{
            UP = 0,
            DOWN = 0,
            UP_LEFT = 12,
            UP_RIGHT = -12,
            DOWN_LEFT = 30,
            DOWN_RIGHT = -30
        };
        
        int currentState;
    
        //The X and Y offsets of the dot
        int mPosX, mPosY;
    
        int mVelX;
        int vJetEngine;

        Dot();
        void reset();

        // CHANGE VELOCITY DEPEND TYPED KEY
        void handleEvent( SDL_Event& e, const int& DENTA_Y );
    
        void autoMove(const int& DENTA_Y, int idNearBlock, int idNearTrap, const std::deque<Block>& Blocks, const std::deque<Trap>& Traps);

        //Moves the dot
        void move(const int& up, const int& DENTA_Y);

        //Shows the dot on the screen
        void render();
    
        int energy;
private:
    enum StateEngine {
        NOT = 0,
        GOING_UP = 1,
    };
    
    int nearestBlockCanReach(const int& DENTA_Y, int idInBlocks, const std::deque<Block>& Blocks);
};

void Dot::autoMove(const int& DENTA_Y, int idNearBlock, int idNearTrap, const std::deque<Block>& Blocks, const std::deque<Trap>& Traps){            // auto handle mVelX
    Block nearBlock = *(Blocks.begin() + idNearBlock);

    if(idNearTrap == -1)        return;
    Trap nearTrap = (*(Traps.begin() + idNearTrap));

    if( (nearBlock.PosY + nearBlock.BLOCK_HEIGHT >= mPosY + DOT_HEIGHT + 2) && (mPosY + DOT_HEIGHT + 2 >= nearBlock.PosY) &&       // if collide
       (mPosX + DOT_WIDTH*10/10 + 5>= nearBlock.PosX) && (mPosX + DOT_WIDTH*0/10 - 5 <= nearBlock.PosX + nearBlock.BLOCK_WIDTH) )
    {                                                                               // dot is above block

        // find nearest block can reach
        idNearBlock++;
        int nextIdCanReach = nearestBlockCanReach(DENTA_Y, idNearBlock, Blocks);
        
        if(nextIdCanReach == -1 || idNearBlock >= Blocks.end() - Blocks.begin() - 3){
            mVelX = 0;
            return;
        }             // cannot reach any block ==> mVelX = 0. Velocity of ball in dynamic block is computed in func checkCollide and move
        
        
        // move right or left
        if(nearBlock.PosX - lPIVOT < DOT_WIDTH*15/10){                  // collide with wall
            mVelX = DENTA_Y + 1;
            return;
        }
        else if(rPIVOT - nearBlock.PosX - nearBlock.BLOCK_WIDTH < DOT_WIDTH*15/10){
            mVelX = -DENTA_Y - 1;
            return;
        }
        
//            if(mVelX){ return; };
        
        Block nextBlock = *(Blocks.begin() + nextIdCanReach);
        
        if(nearBlock.PosX < nextBlock.PosX){
            mVelX = DENTA_Y + 1;
        }
        else{
            mVelX = -DENTA_Y - 1;
        }
        
        // AVOID TRAP
        if(nearBlock.PosX + 5 < nearTrap.PosX){                 // trap is longer than block 10 pixel
            int x = nearTrap.PosX + nearTrap.TRAP_WIDTH - nearBlock.PosX - nearBlock.BLOCK_WIDTH;
            x = fmin(x, nearTrap.TRAP_WIDTH - x + DOT_WIDTH) + 0;
            int y = nearTrap.PosY - nearBlock.PosY - nearBlock.BLOCK_HEIGHT;
            
            if(y < 2*x){ mVelX = -DENTA_Y - 1; }
        }
        else{
            int x = nearTrap.PosX + nearTrap.TRAP_WIDTH - nearBlock.PosX + DOT_WIDTH;
            x = fmin(nearBlock.PosX - nearTrap.PosX, x) + 10;
            int y = nearTrap.PosY - nearBlock.PosY - nearBlock.BLOCK_HEIGHT;
            
            if(y < 2*x){ mVelX = DENTA_Y + 1; }
        }
        
        // MOVE LEFT OR RIGHT WHEN HAVE ANY TARGET BLOCK
    }
    else{               // NOT COLLIDE WITH ANY BLOCK
        if( mPosY + DOT_HEIGHT > (*(Blocks.begin() + idNearBlock)).PosY )        idNearBlock++;
        
        int nextIdCanReach = nearestBlockCanReach(DENTA_Y, idNearBlock, Blocks);
        
        if(nextIdCanReach == -1){
            mVelX = 0;
            return;
        }
        
        // AVOID TRAP
        int xLeft = mPosX + DOT_WIDTH - nearTrap.PosX + 10;
        int xRight = nearTrap.PosX + nearTrap.TRAP_WIDTH - mPosX + 10;
        int y = nearTrap.PosY - mPosY - DOT_HEIGHT;
        if(xLeft < xRight){             // ball site at left with trap
            if(y < 2*xLeft){
                mVelX = -DENTA_Y - 1;
                return;
            }
        }
        else{                               // ball at right
            if(y < 2*xRight){
                mVelX = DENTA_Y + 1;
                return;
            }
        }
        
        
        Block nextBlock = *(Blocks.begin() + nextIdCanReach);
        // IF POSX OF DOT IS IN BLOCK
        if( (mPosX + DOT_WIDTH*10/10 >= nextBlock.PosX + 30) && (mPosX + DOT_WIDTH*0/10 + 30 <= nextBlock.PosX + nextBlock.BLOCK_WIDTH) ){
            mVelX = 0;
        }
        // posX of dot is at left of block
        else if(mPosX + DOT_WIDTH*10/10 < nextBlock.PosX + 30){                 // avoid ball in margin of block and fall down
            mVelX = DENTA_Y + 1;
        }
        // posX of dot is at right of block
        else{
            mVelX = -DENTA_Y - 1;
        }

        // MOVE LEFT, RIGHT OR STOP
    }
}

int Dot::nearestBlockCanReach(const int& DENTA_Y, int idInBlocks, const std::deque<Block>& Blocks){
    while(idInBlocks <= Blocks.end() - Blocks.begin() - 1){                // not a last block
        Block block = *(Blocks.begin() + idInBlocks);
        
        // if posX of dot is in posX of Block
        if( (mPosX + DOT_WIDTH*5/10 >= block.PosX) && (mPosX + DOT_WIDTH*5/10 <= block.PosX + block.BLOCK_WIDTH) ){
            return idInBlocks;
        }
        
        // posX of dot is at left of block
        else if(mPosX + DOT_WIDTH*5/10 < block.PosX){
            int x = block.PosX - mPosX - DOT_WIDTH;
            int y = block.PosY - mPosY - DOT_HEIGHT;
            if( y >= 2*x){
                return idInBlocks;
            }
            else{ idInBlocks++; }
        }
        
        // posX of dot is at right of block
        else{
            int x = mPosX - block.PosX - block.BLOCK_WIDTH;
            int y = block.PosY - mPosY - DOT_HEIGHT;
            if( y >= 2*x){
                return idInBlocks;
            }
            else{ idInBlocks++; }
        }
    }

    return -1;              // cannot move to any block
}

void Dot::handleEvent( SDL_Event& e, const int& DENTA_Y ){
    if( e.type == SDL_KEYDOWN && e.key.repeat == 0 ){
        //Adjust the velocity
        if(!energy)     vJetEngine = NOT;
        else{
            if( e.key.keysym.sym == SDLK_SPACE || e.key.keysym.sym == SDLK_g || e.key.keysym.sym == SDLK_UP){
                vJetEngine = GOING_UP;
            }
            else{
                vJetEngine = NOT;
            }
        }
        
        switch( e.key.keysym.sym ){
            case SDLK_LEFT:     
                mVelX -= (DENTA_Y + 1);
                break;                // VELOCITY WILL RETURN 0 WHEN KEY UP
            case SDLK_RIGHT:    
                mVelX += (DENTA_Y + 1);
                break;
        }
    }

    //If a key was released
    else if( e.type == SDL_KEYUP){
        //Adjust the velocity
        if( e.key.keysym.sym == SDLK_SPACE || e.key.keysym.sym == SDLK_g || e.key.keysym.sym == SDLK_b || e.key.keysym.sym == SDLK_UP){
            vJetEngine = NOT;
        }
        
        switch( e.key.keysym.sym ){
            case SDLK_LEFT:     mVelX = 0; break;                //
            case SDLK_RIGHT:    mVelX = 0; break;                // VELOCITY
        }
    }
}

void Dot::move(const int& upANDdyn, const int& DENTA_Y){
    if(!energy)     vJetEngine = NOT;
    
    if(upANDdyn == -20){             // NOT COLLIDE
        if(vJetEngine == GOING_UP){
            mPosY -= DENTA_Y/2;
            mPosX += mVelX*5/3;
            energy -= 3;
        }
        else{
            mPosY += DENTA_Y;
            mPosX += mVelX;
        }
        
        if(mVelX > 0)           currentState = DOWN_RIGHT;
        else if(mVelX < 0)      currentState = DOWN_LEFT;
        else                    currentState = DOWN;
    }
    else{                           // COLLIDE
        mPosY -= DENTA_Y;
        mPosX += mVelX + upANDdyn;
        if(mVelX + upANDdyn > 0)        currentState = UP_RIGHT;
        else if(mVelX + upANDdyn < 0)   currentState = UP_LEFT;
        else                            currentState = UP;
    }

    mPosX = fmin(mPosX, rPIVOT - DOT_WIDTH);
    mPosX = fmax(mPosX, lPIVOT);
}

void Dot::reset(){
    energy = 0;
    mPosY = CEILING + 120;                       // MAGIC
    mPosX = (SCREEN_WIDTH - DOT_WIDTH)/2;
    mVelX = 0;
}

Dot::Dot()
{
    //Initialize the offsets
    mPosY = CEILING + 120;                       // MAGIC
    mPosX = (SCREEN_WIDTH - DOT_WIDTH)/2;

    //Initialize the velocity
    mVelX = 0;
    vJetEngine = 0;
    
    energy = 0;
    
    currentState = DOWN;
}



/*
                    I TRY TO RUN THIS CODE. BUT WE HAVE ISSUE WITH GLOBAL VARIABLE DECLARE IN main.cpp
                    WE USE gDotTexture.render(x, y, NULL) instead
 */

//void Dot::render(){
//    //Show the dot
//    gDotTexture.render( mPosX, mPosY );
//}

#endif /* ball_h */
