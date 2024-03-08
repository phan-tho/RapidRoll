#ifndef Trap_h
#define Trap_h

class Trap{
    public:
        const int TRAP_HEIGHT = 10;
        const int TRAP_WIDTH  = 75;
        
        int PosX, PosY;
    
        bool dynamic, left;
    
        Trap();
        
        void move(const int& DENTA_Y);
        
//        int dentaX = 1, staticAboveDyn = 1;
        static int dentaX, staticAboveDyn;
    // AS CONST BUT WE CAN ASSIGN ONCE
};

int Trap::dentaX;
int Trap::staticAboveDyn;

Trap::Trap(){                       // WE AVOID HEART ABOVE TRAP IN main.cpp
    PosY = FLOOR;
    PosX = rand()%(rPIVOT - TRAP_WIDTH - lPIVOT + 1) + lPIVOT;
    
    staticAboveDyn = 4;
    dynamic = (rand()%staticAboveDyn == 0);
    left = (PosX - lPIVOT >= rPIVOT - PosX - TRAP_WIDTH);
}

void Trap::move(const int& DENTA_Y){
    PosY -= DENTA_Y;
    
    if(dynamic){
        PosX += (left ? -dentaX : dentaX);
        if(PosX <= lPIVOT){
            PosX = lPIVOT;
            left = false;
        }
        else if(PosX + TRAP_WIDTH >= rPIVOT){
            PosX = rPIVOT - TRAP_WIDTH;
            left = true;
        }
    }
}

#endif /* Trap_h */

