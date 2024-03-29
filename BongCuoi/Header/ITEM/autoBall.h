//  Created by Phan Tho on 27/03/2024.

#ifndef autoBall_h
#define autoBall_h

#include "ball.h"

class autoBall : public Ball{
    public:
        // handle Velocity of ball
        void autoMove(const int& DENTA_Y, int idNearBlock, int idNearTrap, const std::deque<Block>& Blocks, const std::deque<Trap>& Traps);
    
        // FUNC CLOSE IS INHERITED FROM BALL
        
    private:
        int nearestBlockCanReach(const int& DENTA_Y, int idInBlocks, const std::deque<Block>& Blocks);
    
        // 2 STATE        ABOVE BLOCK || FREE
        bool isAboveBlock(const int& idNearBlock, const std::deque<Block>& Blocks);

        /*
            Find nearest block can reach
            Handle V according to PosX of block
            Avoid trap
        */
        void handleWhenAboveBlock(const int& DENTA_Y, int idNearBlock, int idNearTrap, const std::deque<Block>& Blocks, const std::deque<Trap>& Traps);
        void handleWhenFree(const int& DENTA_Y, int idNearBlock, int idNearTrap, const std::deque<Block>& Blocks, const std::deque<Trap>& Traps);
};


/*
    Agr:
        DENTA_Y + 1 : Horizinal velocity of ball
        Id of nearest block and trap in deque
    
*/

void autoBall::autoMove(const int& DENTA_Y, int idNearBlock, int idNearTrap, const std::deque<Block>& Blocks, const std::deque<Trap>& Traps){            // auto handle mVelX
    if(idNearTrap == -1)        return;
    
    if(isAboveBlock(idNearBlock, Blocks)){
        handleWhenAboveBlock(DENTA_Y, idNearBlock, idNearTrap, Blocks, Traps);
    }
    else{               // NOT COLLIDE WITH ANY BLOCK
        handleWhenFree(DENTA_Y, idNearBlock, idNearTrap, Blocks, Traps);
    }
}



void autoBall::handleWhenAboveBlock(const int& DENTA_Y, int idNearBlock, int idNearTrap, const std::deque<Block>& Blocks, const std::deque<Trap>& Traps){
    Block block = *(Blocks.begin() + idNearBlock);

    Trap nearTrap = (*(Traps.begin() + idNearTrap));
    
    // find nearest block can reach
    idNearBlock++;
    int nextIdCanReach = nearestBlockCanReach(DENTA_Y, idNearBlock, Blocks);
    
    if(nextIdCanReach == -1 || idNearBlock >= Blocks.end() - Blocks.begin() - 3){
        mVelX = 0;
        return;
    }             // cannot reach any block ==> mVelX = 0. Velocity of ball in dynamic block is computed in func checkCollide and move
    
    
    // move right or left
    if(block.PosX - lPIVOT < BALL_WIDTH*15/10){                  // collide with wall
        mVelX = DENTA_Y + 1;
        return;
    }
    else if(rPIVOT - block.PosX - block.BLOCK_WIDTH < BALL_WIDTH*15/10){
        mVelX = -DENTA_Y - 1;
        return;
    }
    
//            if(mVelX){ return; };
    
    Block nextBlock = *(Blocks.begin() + nextIdCanReach);
    
    if(block.PosX < nextBlock.PosX){
        mVelX = DENTA_Y + 1;
    }
    else{
        mVelX = -DENTA_Y - 1;
    }
    
    // AVOID TRAP
    if(block.PosX + 5 < nearTrap.PosX){                 // trap is longer than block 10 pixel
        int x = nearTrap.PosX + nearTrap.TRAP_WIDTH - block.PosX - block.BLOCK_WIDTH;
        x = fmin(x, nearTrap.TRAP_WIDTH - x + BALL_WIDTH) + 0;
        int y = nearTrap.PosY - block.PosY - block.BLOCK_HEIGHT;
        
        if(y < 2*x){ mVelX = -DENTA_Y - 1; }
    }
    else{
        int x = nearTrap.PosX + nearTrap.TRAP_WIDTH - block.PosX + BALL_WIDTH;
        x = fmin(block.PosX - nearTrap.PosX, x) + 10;
        int y = nearTrap.PosY - block.PosY - block.BLOCK_HEIGHT;
        
        if(y < 2*x){ mVelX = DENTA_Y + 1; }
    }
    // MOVE LEFT OR RIGHT WHEN HAVE ANY TARGET BLOCK
}

void autoBall::handleWhenFree(const int& DENTA_Y, int idNearBlock, int idNearTrap, const std::deque<Block>& Blocks, const std::deque<Trap>& Traps){
    Trap nearTrap = (*(Traps.begin() + idNearTrap));
    
    if( mPosY + BALL_HEIGHT > (*(Blocks.begin() + idNearBlock)).PosY )        idNearBlock++;
    
    int nextIdCanReach = nearestBlockCanReach(DENTA_Y, idNearBlock, Blocks);
    
    if(nextIdCanReach == -1){
        mVelX = 0;
        return;
    }
    
    // AVOID TRAP
    int xLeft = mPosX + BALL_WIDTH - nearTrap.PosX + 10;
    int xRight = nearTrap.PosX + nearTrap.TRAP_WIDTH - mPosX + 10;
    int y = nearTrap.PosY - mPosY - BALL_HEIGHT;
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
    if( (mPosX + BALL_WIDTH*10/10 >= nextBlock.PosX + 30) && (mPosX + BALL_WIDTH*0/10 + 30 <= nextBlock.PosX + nextBlock.BLOCK_WIDTH) ){
        mVelX = 0;
    }
    // posX of dot is at left of block
    else if(mPosX + BALL_WIDTH*10/10 < nextBlock.PosX + 30){                 // avoid ball in margin of block and fall down
        mVelX = DENTA_Y + 1;
    }
    // posX of dot is at right of block
    else{
        mVelX = -DENTA_Y - 1;
    }

    // MOVE LEFT, RIGHT OR STOP
}

int autoBall::nearestBlockCanReach(const int& DENTA_Y, int idInBlocks, const std::deque<Block>& Blocks){
    while(idInBlocks <= Blocks.end() - Blocks.begin() - 1){                // not a last block
        Block block = *(Blocks.begin() + idInBlocks);
        
        // if posX of dot is in posX of Block
        if( (mPosX + BALL_WIDTH*5/10 >= block.PosX) && (mPosX + BALL_WIDTH*5/10 <= block.PosX + block.BLOCK_WIDTH) ){
            return idInBlocks;
        }
        
        // posX of dot is at left of block
        else if(mPosX + BALL_WIDTH*5/10 < block.PosX){
            int x = block.PosX - mPosX - BALL_WIDTH;
            int y = block.PosY - mPosY - BALL_HEIGHT;
            if( y >= 2*x){
                return idInBlocks;
            }
            else{ idInBlocks++; }
        }
        
        // posX of dot is at right of block
        else{
            int x = mPosX - block.PosX - block.BLOCK_WIDTH;
            int y = block.PosY - mPosY - BALL_HEIGHT;
            if( y >= 2*x){
                return idInBlocks;
            }
            else{ idInBlocks++; }
        }
    }

    return -1;              // cannot move to any block
}

bool autoBall::isAboveBlock(const int& idNearBlock, const std::deque<Block>& Blocks){
    Block block = *(Blocks.begin() + idNearBlock);
    return ( (block.PosY + block.BLOCK_HEIGHT >= mPosY + BALL_HEIGHT + 2) && (mPosY + BALL_HEIGHT + 2 >= block.PosY) &&
       (mPosX + BALL_WIDTH*10/10 + 5>= block.PosX) && (mPosX + BALL_WIDTH*0/10 - 5 <= block.PosX + block.BLOCK_WIDTH) );
}

#endif /* autoBall_h */
