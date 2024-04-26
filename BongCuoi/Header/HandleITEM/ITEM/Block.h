#ifndef Block_h
#define Block_h

class Block{
public:
    const int BLOCK_HEIGHT = 10;
    const int BLOCK_WIDTH  = 65;

    int getX() const { return PosX; }
    int getY() const { return PosY; }

    bool dynamic, left;     // Going left

    Block();
    
    void move(const int& DENTA_Y);

    static int dentaX, staticAboveDyn;
private:
    int PosX, PosY;
};

int Block::dentaX;
int Block::staticAboveDyn;

Block::Block(){
    PosY = FLOOR;
    PosX = rand()%(rPIVOT - BLOCK_WIDTH - lPIVOT + 1) + lPIVOT;
    
    dynamic = (rand()%staticAboveDyn == 0);
    left = (getX() - lPIVOT >= rPIVOT - getX() - BLOCK_WIDTH);
}

void Block::move(const int& DENTA_Y){             // MOVE UP
    PosY -= DENTA_Y;
    
    if(dynamic){
        PosX += (left ? -dentaX : dentaX);
        if(PosX <= lPIVOT){
            PosX = lPIVOT;
            left = false;
        }
        else if(getX() + BLOCK_WIDTH >= rPIVOT){
            PosX = rPIVOT - BLOCK_WIDTH;
            left = true;
        }
    }
}

#endif /* Block_h */
