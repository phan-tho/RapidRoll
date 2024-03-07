#ifndef Trap_h
#define Trap_h

class Trap{
    public:
        const int TRAP_HEIGHT = 10;
        const int TRAP_WIDTH  = 75;
        
        int PosX, PosY;
    
        Trap();
        
        void move();
        
//        void render();
};

Trap::Trap(){
    PosY = FLOOR;
    PosX = rand()%(rPIVOT - TRAP_WIDTH - lPIVOT + 1) + lPIVOT;
    
//    Heart heartOnBlock((PosX + BLOCK_WIDTH)/2, PosY - BLOCK_HEIGHT);
}

void Trap::move(){
    PosY -= Denta_Y;
}

#endif /* Trap_h */

