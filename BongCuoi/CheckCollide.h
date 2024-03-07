#ifndef CheckCollide_h
#define CheckCollide_h

bool checkCollideBlock(Dot& dot);
bool checkCollideTrap(Dot& dot);
bool checkCollideHeart(Dot& dot, Heart& heart);

bool checkCollideBlock(Dot& dot){
    int l = 0, r = Blocks.size() - 1;
    int pos = -1;
    while(l <= r){
        int mid = (l + r)/2;
        int blockPos = (*(Blocks.begin() + mid)).PosY;
        
        if( (blockPos + (*Blocks.begin()).BLOCK_HEIGHT >= dot.mPosY + dot.DOT_HEIGHT) && (dot.mPosY + dot.DOT_HEIGHT >= blockPos) ){
            pos = mid;
            break;
        }
        else if (dot.mPosY + dot.DOT_HEIGHT < blockPos)         r = mid - 1;
        else                                                    l = mid + 1;
    }
    if(pos == -1)       return false;
    
    int blockPosX = (*(Blocks.begin() + pos)).PosX;
    if( (dot.mPosX + dot.DOT_WIDTH/2 >= blockPosX) && (dot.mPosX + dot.DOT_WIDTH/2 <= blockPosX + (*Blocks.begin()).BLOCK_WIDTH) ){
        
        // AVOID BALL ISNOT CIRCLE (BLOCK FILL A PART OF BALL)
        
        dot.mPosY = (*(Blocks.begin() + pos)).PosY - dot.DOT_HEIGHT;
        return true;
    }
    
    return false;
}

bool checkCollideHeart(Dot& dot, Heart& heart){
    if( dot.mPosY + dot.DOT_HEIGHT < heart.PosY || dot.mPosY > heart.PosY + heart.HEART_HEIGHT)        return false;           // NOT SAME VERTICAL COORDINATE
    if( dot.mPosX + dot.DOT_WIDTH < heart.PosX || dot.mPosX > heart.PosX + heart.HEART_WIDTH)          return false;           // NOT SAME HORIZONAL COORDINATE
    
    return true;;
}

bool checkCollideTrap(Dot& dot){
    int l = 0, r = Traps.size() - 1;
    int pos = -1;
    while(l <= r){
        int mid = (l + r)/2;
        int trapPos = (*(Traps.begin() + mid)).PosY;
        
        if( (trapPos + (*Traps.begin()).TRAP_HEIGHT >= dot.mPosY + dot.DOT_HEIGHT) && (dot.mPosY + dot.DOT_HEIGHT >= trapPos) ){
            pos = mid;
            break;
        }
        else if (dot.mPosY + dot.DOT_HEIGHT < trapPos)          r = mid - 1;
        else                                                    l = mid + 1;
    }
    if(pos == -1)       return false;
    
    int trapPosX = (*(Traps.begin() + pos)).PosX;
    if( (dot.mPosX + dot.DOT_WIDTH/2 >= trapPosX) && (dot.mPosX + dot.DOT_WIDTH/2 <= trapPosX + (*Traps.begin()).TRAP_WIDTH) ){
        
        // AVOID BALL ISNOT CIRCLE (BLOCK FILL A PART OF BALL)
        
        dot.mPosY = (*(Traps.begin() + pos)).PosY - dot.DOT_HEIGHT;
        return true;
    }
    
    return false;
}

#endif /* CheckCollide_h */