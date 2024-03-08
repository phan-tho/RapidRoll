#ifndef Trap_h
#define Trap_h

class Trap{
    public:
        const int TRAP_HEIGHT = 10;
        const int TRAP_WIDTH  = 75;
        
        int PosX, PosY;
    
        Trap();
        
        void move(const int& DENTA_Y);
        
//        void render();
};

Trap::Trap(){                       // WE AVOID HEART ABOVE TRAP IN main.cpp
    PosY = FLOOR;
    PosX = rand()%(rPIVOT - TRAP_WIDTH - lPIVOT + 1) + lPIVOT;
}

void Trap::move(const int& DENTA_Y){
    PosY -= DENTA_Y;
}

#endif /* Trap_h */

