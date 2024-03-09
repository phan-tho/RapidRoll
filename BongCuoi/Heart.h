#ifndef Heart_h
#define Heart_h

class Heart{
    public:
        const int HEART_HEIGHT = 18;
        const int HEART_WIDTH  = 18;
        
        int PosX, PosY;
    
        bool isEaten = false;
    
        int moveToLeft;
    
        Heart();
        
        void move(const int& DENTA_Y);
        
//        void render();
    
        void assignPos(const int x, const int y);
};

Heart::Heart(){
    PosX = 0;
    PosY = 0;
    moveToLeft = 0;
}



void Heart::move(const int& DENTA_Y){
    PosY -= DENTA_Y;
    
    PosX += moveToLeft;             // PASS VELOCITY OF BLOCK TO HEART
                                    // HEART MOVE INDEPEND WITH BLOCK BELOW ITS
    
    if( ( PosX <= 64 ) || ( PosX >= 367 ) ){         // BOUND WHEN COLLIDE WALL
        PosX -= moveToLeft;                             // MAGIC NUMBERS
        moveToLeft = -moveToLeft;
    }
}

//void Heart::render(){
//    gHeart.render(PosX, PosY, NULL);
//}

void Heart::assignPos(const int x, const int y){
    PosX = x;
    PosY = y;
}

#endif /* Heart_h */
