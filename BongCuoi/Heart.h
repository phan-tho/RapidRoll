#ifndef Heart_h
#define Heart_h

class Heart{
    public:
        const int HEART_HEIGHT = 18;
        const int HEART_WIDTH  = 18;
        
        int PosX, PosY;
    
        Heart();
        
        void move();
        
//        void render();
    
        void assignPos(const int x, const int y);
};

Heart::Heart(){
    PosX = 0;
    PosY = 0;
}



void Heart::move(){
    PosY -= DENTA_Y;
}

//void Heart::render(){
//    gHeart.render(PosX, PosY, NULL);
//}

void Heart::assignPos(const int x, const int y){
    PosX = x;
    PosY = y;
}

#endif /* Heart_h */
