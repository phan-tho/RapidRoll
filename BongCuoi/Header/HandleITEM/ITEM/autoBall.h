//  Created by Phan Tho on 27/03/2024.

#ifndef autoBall_h
#define autoBall_h

#include "ball.h"

class autoBall : public Ball{
    public:
        autoBall(std::string path) : Ball(path) { }
    
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
    Handle 2 state:
        Ball is in block
        Ball is free
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


// FIND NEAREST BLOCK CAN REACH
// ORIENT BALL TO REACH NEXT BLOCK

// FIND NEAREST TRAP CAN TRAP
// AVOID TRAP WHEN BALL IS IN DANGEROUS AREA OF TRAP
void autoBall::handleWhenAboveBlock(const int& DENTA_Y, int idNearBlock, int idNearTrap, const std::deque<Block>& Blocks, const std::deque<Trap>& Traps){
    Block block = *(Blocks.begin() + idNearBlock);

    Trap nearTrap = (*(Traps.begin() + idNearTrap));
    
    // find nearest block can reach
    idNearBlock++;
    int nextIdCanReach = nearestBlockCanReach(DENTA_Y, idNearBlock, Blocks);
    
    // cannot reach any block ==> mVelX = 0. Velocity of ball in dynamic block is computed in func checkCollide and move
    if(nextIdCanReach == -1 || idNearBlock >= Blocks.end() - Blocks.begin() - 3){
        mVelX = 0;
        return;
    }
    
    
    // move right or left
    // Avoid case ball cannot get out
    if(block.getX() - lPIVOT < BALL_WIDTH*15/10){                  // collide with wall
        mVelX = DENTA_Y + 1;
        return;
    }
    else if(rPIVOT - block.getX() - block.BLOCK_WIDTH < BALL_WIDTH*15/10){
        mVelX = -DENTA_Y - 1;
        return;
    }
    
    Block nextBlock = *(Blocks.begin() + nextIdCanReach);
    
    if(block.getX() < nextBlock.getX()){
        mVelX = DENTA_Y + 1;
    }
    else{
        mVelX = -DENTA_Y - 1;
    }
    
    // AVOID TRAP
    if(block.getX() + 5 < nearTrap.getX()){                 // trap is longer than block 10 pixel
        int x = nearTrap.getX() + nearTrap.TRAP_WIDTH - block.getX() - block.BLOCK_WIDTH;
        x = fmin(x, nearTrap.TRAP_WIDTH - x + BALL_WIDTH) + 0;
        int y = nearTrap.getY() - block.getY() - block.BLOCK_HEIGHT;
        
        if(y < 2*x){ mVelX = -DENTA_Y - 1; }
    }
    else{
        int x = nearTrap.getX() + nearTrap.TRAP_WIDTH - block.getX() + BALL_WIDTH;
        x = fmin(block.getX() - nearTrap.getX(), x) + 10;
        int y = nearTrap.getY() - block.getY() - block.BLOCK_HEIGHT;
        
        if(y < 2*x){ mVelX = DENTA_Y + 1; }
    }
    // MOVE LEFT OR RIGHT WHEN HAVE ANY TARGET BLOCK
}


// FIND NEAREST BLOCK CAN REACH
// ORIENT BALL TO NEXT BLOCK

// FIND NEAREST TRAP CAN TRAP
// AVOID WHEN BALL IS IN DANGER AREA
void autoBall::handleWhenFree(const int& DENTA_Y, int idNearBlock, int idNearTrap, const std::deque<Block>& Blocks, const std::deque<Trap>& Traps){
    Trap nearTrap = (*(Traps.begin() + idNearTrap));
    
    if( mPosY + BALL_HEIGHT > (*(Blocks.begin() + idNearBlock)).getY() )        idNearBlock++;
    
    int nextIdCanReach = nearestBlockCanReach(DENTA_Y, idNearBlock, Blocks);
    
    if(nextIdCanReach == -1){
        mVelX = 0;
        return;
    }
    
    // AVOID TRAP
    int xLeft = mPosX + BALL_WIDTH - nearTrap.getX() + 10;
    int xRight = nearTrap.getX() + nearTrap.TRAP_WIDTH - mPosX + 10;
    int y = nearTrap.getY() - mPosY - BALL_HEIGHT;
    
    if(y < 2*fmin(xRight, xLeft)){
        if(xRight < xLeft)      mVelX = DENTA_Y + 1;
        else                    mVelX = -DENTA_Y - 1;
        return;
    }
    
    
    Block nextBlock = *(Blocks.begin() + nextIdCanReach);
    // IF POSX OF DOT IS IN BLOCK
    if( (mPosX + BALL_WIDTH*10/10 >= nextBlock.getX() + 30) && (mPosX + BALL_WIDTH*0/10 + 30 <= nextBlock.getX() + nextBlock.BLOCK_WIDTH) ){
        mVelX = 0;
    }
    // posX of dot is at left of block
    else if(mPosX + BALL_WIDTH*10/10 < nextBlock.getX() + 30){                 // avoid ball in margin of block and fall down
        mVelX = DENTA_Y + 1;
    }
    // posX of dot is at right of block
    else{
        mVelX = -DENTA_Y - 1;
    }

    // MOVE LEFT, RIGHT OR STOP
}


// FIND NEAREST BLOCK CAN REACH
// IF HAVENOT RETURN -1
int autoBall::nearestBlockCanReach(const int& DENTA_Y, int idInBlocks, const std::deque<Block>& Blocks){
    while(idInBlocks <= Blocks.end() - Blocks.begin() - 1){                // not a last block
        Block block = *(Blocks.begin() + idInBlocks);
        
        // if posX of dot is in posX of Block
        if( (mPosX + BALL_WIDTH*5/10 >= block.getX()) && (mPosX + BALL_WIDTH*5/10 <= block.getX() + block.BLOCK_WIDTH) ){
            return idInBlocks;
        }
        
        // posX of dot is at left of block
        else if(mPosX + BALL_WIDTH*5/10 < block.getX()){
            int x = block.getX() - mPosX - BALL_WIDTH;
            int y = block.getY() - mPosY - BALL_HEIGHT;
            if( y >= 2*x){
                return idInBlocks;
            }
            else{ idInBlocks++; }
        }
        
        // posX of dot is at right of block
        else{
            int x = mPosX - block.getX() - block.BLOCK_WIDTH;
            int y = block.getY() - mPosY - BALL_HEIGHT;
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
    return  ( (block.getY() + block.BLOCK_HEIGHT >= mPosY + BALL_HEIGHT + 2) &&
            (mPosY + BALL_HEIGHT + 2 >= block.getY()) &&
            (mPosX + BALL_WIDTH*10/10 + 5>= block.getX()) &&
            (mPosX + BALL_WIDTH*0/10 - 5 <= block.getX() + block.BLOCK_WIDTH) );
}

#endif /* autoBall_h */
