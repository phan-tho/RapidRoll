#ifndef Trap_h
#define Trap_h

class Trap{
    public:
    Trap();

    const int TRAP_HEIGHT = 10;
    const int TRAP_WIDTH  = 75;

    // AS CONST BUT WE CAN ASSIGN ONCE
    static int dentaX, staticAboveDyn;

    int getX() const { return PosX; }
    int getY() const { return PosY; }

    bool dynamic, left;
    
    void move(const int& DENTA_Y);
private:
    int PosX, PosY;
};

int Trap::dentaX;
int Trap::staticAboveDyn;

Trap::Trap(){                       // WE AVOID HEART ABOVE TRAP IN main.cpp
    PosY = FLOOR;
    PosX = rand()%(rPIVOT - TRAP_WIDTH - lPIVOT + 1) + lPIVOT;
    
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

