#ifndef CheckCollide_h
#define CheckCollide_h

#include "ITEM/ITEM.h"
#include "def.h"

int checkCollideBlock(Dot& dot, const std::deque<Block>& Blocks, int pos);
bool checkCollideTrap(Dot& dot, const std::deque<Trap>& Traps);

bool checkCollideHeart(const Dot& dot, const Heart& heart);
bool checkCollideFuel(const Dot& dot, const Fuel& fuel);

int findBlockSameY(const Dot& dot, const std::deque<Block>& Blocks){
    if(Blocks.empty())          return -1;
    
    int l = 0, r = int(Blocks.size()) - 1;
    while(l <= r){
        int mid = (l + r)/2;
        int blockPos = (*(Blocks.begin() + mid)).PosY;
        
        if( (blockPos + (*Blocks.begin()).BLOCK_HEIGHT >= dot.mPosY + dot.DOT_HEIGHT) && (dot.mPosY + dot.DOT_HEIGHT >= blockPos) ){
            return mid;
        }
        else if (dot.mPosY + dot.DOT_HEIGHT < blockPos)         r = mid - 1;
        else                                                    l = mid + 1;
    }
    return -1;
}

int findNearestBlock(const Dot& dot, const std::deque<Block>& Blocks){
    if(Blocks.empty())          return -1;
    // id PosY of dot is greateer than PosY of last element in Blocks
//    if( dot.mPosY <= Blocks.back().PosY + Blocks.back().BLOCK_HEIGHT )           return -1;
    
    int l = 0, r = int(Blocks.size()) - 1;
    while(l < r){
        int mid = (l + r)/2;
        int blockPos = (*(Blocks.begin() + mid)).PosY;
        
        if( (blockPos + (*Blocks.begin()).BLOCK_HEIGHT >= dot.mPosY + dot.DOT_HEIGHT) && (dot.mPosY + dot.DOT_HEIGHT >= blockPos) ){
            return mid;
        }
        if (blockPos > dot.mPosY + dot.DOT_HEIGHT)         r = mid;
        else                                               l = mid + 1;
    }
    return l;
}

int checkCollideBlock(Dot& dot, const std::deque<Block>& Blocks, int pos){
//    pos = findBlockSameY(dot, Blocks);
    if(pos == -1)       return -20;                  // NOT COLLIDE WITH ANY BLOCK
    
    Block block = *(Blocks.begin() + pos);
    // check block pos Y collide or not
    int blockPosY = block.PosY;
    if( (blockPosY + block.BLOCK_HEIGHT < dot.mPosY) || (dot.mPosY + dot.DOT_HEIGHT < blockPosY) ){
        return -20;
    }                                       // NOT COLLIDE WITH ANY BLOCK
    
    // check block pos X collide or not
    int blockPosX = block.PosX;
    
    // COLLIDE -----------------------------------------------------------------------------------------------------------------------------------
    if( (dot.mPosX + dot.DOT_WIDTH*10/10 > blockPosX) && (dot.mPosX + dot.DOT_WIDTH*0/10 < blockPosX + (*Blocks.begin()).BLOCK_WIDTH) ){
        
        // AVOID BALL ISNOT CIRCLE (BLOCK FILL A PART OF BALL)
        dot.mPosY = block.PosY - dot.DOT_HEIGHT;
        
        if(block.dynamic){
            return (block.left ? -block.dentaX : block.dentaX);             // LEFT VELOCITY (CAN BE NEGATIVE)
        }
        
        return 0;               // COLLDIDE STATIC BLOCK
    }
    //---------------------------------------------------------------------------------------
    
    return -20;                      // NOT COLLIDE
}

bool checkCollideHeart(const Dot& dot, const Heart& heart){
    if( dot.mPosY + dot.DOT_HEIGHT < heart.PosY || dot.mPosY > heart.PosY + heart.HEART_HEIGHT)        return false;           // NOT SAME VERTICAL COORDINATE
    if( dot.mPosX + dot.DOT_WIDTH < heart.PosX || dot.mPosX > heart.PosX + heart.HEART_WIDTH)          return false;           // NOT SAME HORIZONAL COORDINATE
    
    return true;
}

bool checkCollideFuel(const Dot& dot, const Fuel& fuel){
    if( dot.mPosY + dot.DOT_HEIGHT < fuel.PosY || dot.mPosY > fuel.PosY + fuel.FUEL_HEIGHT)        return false;           // NOT SAME VERTICAL COORDINATE
    if( dot.mPosX + dot.DOT_WIDTH < fuel.PosX || dot.mPosX > fuel.PosX + fuel.FUEL_WIDTH)          return false;           // NOT SAME HORIZONAL COORDINATE
    
    return true;
}

int findNearestTrap(const Dot& dot, const std::deque<Trap>& Traps){
    if(Traps.empty())          return -1;
    // id PosY of dot is greateer than PosY of last element in Blocks
//    if( dot.mPosY <= Blocks.back().PosY + Blocks.back().BLOCK_HEIGHT )           return -1;
    
    int l = 0, r = int(Traps.size()) - 1;
    while(l < r){
        int mid = (l + r)/2;
        int trapPos = (*(Traps.begin() + mid)).PosY;
        
        if( (trapPos + (*Traps.begin()).TRAP_HEIGHT >= dot.mPosY + dot.DOT_HEIGHT) && (dot.mPosY + dot.DOT_HEIGHT >= trapPos) ){
            return mid;
        }
        if (trapPos > dot.mPosY + dot.DOT_HEIGHT)          r = mid;
        else                                               l = mid + 1;
    }
    return l;
}

bool checkCollideTrap(Dot& dot, const std::deque<Trap>& Traps){
    int pos = findNearestTrap(dot, Traps);
    if(pos == -1)       return false;
    
    Trap trap = *(Traps.begin() + pos);
    
    // check block pos Y collide or not
    int trapPosY = trap.PosY;
    if( (trapPosY + trap.TRAP_HEIGHT < dot.mPosY) || (dot.mPosY + dot.DOT_HEIGHT < trapPosY) ){
        return false;
    }                                       // NOT COLLIDE WITH ANY BLOCK
    
    int trapPosX = (*(Traps.begin() + pos)).PosX;
    if( (dot.mPosX + dot.DOT_WIDTH*10/10 >= trapPosX) && (dot.mPosX + dot.DOT_WIDTH*0/10 <= trapPosX + (*Traps.begin()).TRAP_WIDTH) ){
        
        // AVOID BALL ISNOT CIRCLE (BLOCK FILL A PART OF BALL)
        
        dot.mPosY = (*(Traps.begin() + pos)).PosY - dot.DOT_HEIGHT;
        return true;
    }
    
    return false;
}

#endif /* CheckCollide_h */
