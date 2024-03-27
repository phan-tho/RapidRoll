#ifndef CheckCollide_h
#define CheckCollide_h

#include "ITEM/ITEM.h"
#include "def.h"

int checkCollideBlock(Ball& ball, const std::deque<Block>& Blocks, const int& pos);
bool checkCollideTrap(Ball& ball, const std::deque<Trap>& Traps, const int& pos);

bool checkCollideHeart(const Ball& ball, const Heart& heart);
bool checkCollideFuel(const Ball& ball, const Fuel& fuel);

int findBlockSameY(const Ball& ball, const std::deque<Block>& Blocks){
    if(Blocks.empty())          return -1;
    
    int l = 0, r = int(Blocks.size()) - 1;
    while(l <= r){
        int mid = (l + r)/2;
        int blockPos = (*(Blocks.begin() + mid)).PosY;
        
        if( (blockPos + (*Blocks.begin()).BLOCK_HEIGHT >= ball.mPosY + ball.BALL_HEIGHT) && (ball.mPosY + ball.BALL_HEIGHT >= blockPos) ){
            return mid;
        }
        else if (ball.mPosY + ball.BALL_HEIGHT < blockPos)         r = mid - 1;
        else                                                      l = mid + 1;
    }
    return -1;
}

int findNearestBlock(const Ball& ball, const std::deque<Block>& Blocks){
    if(Blocks.empty())          return -1;
    // id PosY of dot is greateer than PosY of last element in Blocks
//    if( dot.mPosY <= Blocks.back().PosY + Blocks.back().BLOCK_HEIGHT )           return -1;
    
    int l = 0, r = int(Blocks.size()) - 1;
    while(l < r){
        int mid = (l + r)/2;
        int blockPos = (*(Blocks.begin() + mid)).PosY;
        
        if( (blockPos + (*Blocks.begin()).BLOCK_HEIGHT >= ball.mPosY + ball.BALL_HEIGHT) && (ball.mPosY + ball.BALL_HEIGHT >= blockPos) ){
            return mid;
        }
        if (blockPos > ball.mPosY + ball.BALL_HEIGHT)         r = mid;
        else                                               l = mid + 1;
    }
    return l;
}

int checkCollideBlock(Ball& ball, const std::deque<Block>& Blocks, const int& pos){
//    pos = findBlockSameY(dot, Blocks);
    if(pos == -1)       return -20;                  // NOT COLLIDE WITH ANY BLOCK
    
    Block block = *(Blocks.begin() + pos);
    // check block pos Y collide or not
    int blockPosY = block.PosY;
    if( (blockPosY + block.BLOCK_HEIGHT < ball.mPosY) || (ball.mPosY + ball.BALL_HEIGHT < blockPosY) ){
        return -20;
    }                                       // NOT COLLIDE WITH ANY BLOCK
    
    // check block pos X collide or not
    int blockPosX = block.PosX;
    
    // COLLIDE -----------------------------------------------------------------------------------------------------------------------------------
    if( (ball.mPosX + ball.BALL_WIDTH*10/10 > blockPosX) && (ball.mPosX + ball.BALL_WIDTH*0/10 < blockPosX + (*Blocks.begin()).BLOCK_WIDTH) ){
        
        // AVOID BALL ISNOT CIRCLE (BLOCK FILL A PART OF BALL)
        ball.mPosY = block.PosY - ball.BALL_HEIGHT;
        
        if(block.dynamic){
            return (block.left ? -block.dentaX : block.dentaX);             // LEFT VELOCITY (CAN BE NEGATIVE)
        }
        
        return 0;               // COLLDIDE STATIC BLOCK
    }
    //---------------------------------------------------------------------------------------
    
    return -20;                      // NOT COLLIDE
}

bool checkCollideHeart(const Ball& ball, const Heart& heart){
    if( ball.mPosY + ball.BALL_HEIGHT < heart.PosY || ball.mPosY > heart.PosY + heart.HEART_HEIGHT)        return false;           // NOT SAME VERTICAL COORDINATE
    if( ball.mPosX + ball.BALL_WIDTH < heart.PosX || ball.mPosX > heart.PosX + heart.HEART_WIDTH)          return false;           // NOT SAME HORIZONAL COORDINATE
    
    return true;
}

bool checkCollideFuel(const Ball& ball, const Fuel& fuel){
    if( ball.mPosY + ball.BALL_HEIGHT < fuel.PosY || ball.mPosY > fuel.PosY + fuel.FUEL_HEIGHT)        return false;           // NOT SAME VERTICAL COORDINATE
    if( ball.mPosX + ball.BALL_WIDTH < fuel.PosX || ball.mPosX > fuel.PosX + fuel.FUEL_WIDTH)          return false;           // NOT SAME HORIZONAL COORDINATE
    
    return true;
}

int findNearestTrap(const Ball& ball, const std::deque<Trap>& Traps){
    if(Traps.empty())          return -1;
    // id PosY of dot is greateer than PosY of last element in Blocks
//    if( dot.mPosY <= Blocks.back().PosY + Blocks.back().BLOCK_HEIGHT )           return -1;
    
    int l = 0, r = int(Traps.size()) - 1;
    while(l < r){
        int mid = (l + r)/2;
        int trapPos = (*(Traps.begin() + mid)).PosY;
        
        if( (trapPos + (*Traps.begin()).TRAP_HEIGHT >= ball.mPosY + ball.BALL_HEIGHT) && (ball.mPosY + ball.BALL_HEIGHT >= trapPos) ){
            return mid;
        }
        if (trapPos > ball.mPosY + ball.BALL_HEIGHT)          r = mid;
        else                                               l = mid + 1;
    }
    return l;
}

bool checkCollideTrap(Ball& ball, const std::deque<Trap>& Traps, const int& pos){
//    int pos = findNearestTrap(dot, Traps);
    if(pos == -1)       return false;
    
    Trap trap = *(Traps.begin() + pos);
    
    // check block pos Y collide or not
    int trapPosY = trap.PosY;
    if( (trapPosY + trap.TRAP_HEIGHT < ball.mPosY) || (ball.mPosY + ball.BALL_HEIGHT < trapPosY) ){
        return false;
    }                                       // NOT COLLIDE WITH ANY BLOCK
    
    int trapPosX = (*(Traps.begin() + pos)).PosX;
    if( (ball.mPosX + ball.BALL_WIDTH*10/10 >= trapPosX) && (ball.mPosX + ball.BALL_WIDTH*0/10 <= trapPosX + (*Traps.begin()).TRAP_WIDTH) ){
        
        // AVOID BALL ISNOT CIRCLE (BLOCK FILL A PART OF BALL)
        
        ball.mPosY = (*(Traps.begin() + pos)).PosY - ball.BALL_HEIGHT;
        return true;
    }
    
    return false;
}

#endif /* CheckCollide_h */
