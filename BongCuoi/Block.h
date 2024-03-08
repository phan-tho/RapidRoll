#ifndef Block_h
#define Block_h

class Block{
    public:
        const int BLOCK_HEIGHT = 10;
        const int BLOCK_WIDTH  = 65;
    
        int PosX, PosY;
    
        int staticAboveDyn;
    
        bool dynamic, left;     // Going left
    
        Block();
        
        void move(const int& DENTA_Y);
//        void render();
        const int dentaX = 1;
};

Block::Block(){
    PosY = FLOOR;
    PosX = rand()%(rPIVOT - BLOCK_WIDTH - lPIVOT + 1) + lPIVOT;
    
    staticAboveDyn = 3;
    dynamic = (rand()%staticAboveDyn == 0);
    left = (PosX - lPIVOT >= rPIVOT - PosX - BLOCK_WIDTH);
}

void Block::move(const int& DENTA_Y){             // MOVE UP
    PosY -= DENTA_Y;
    
    if(dynamic){
        PosX += (left ? -dentaX : dentaX);
        if(PosX <= lPIVOT){
            PosX = lPIVOT;
            left = false;
        }
        else if(PosX + BLOCK_WIDTH >= rPIVOT){
            PosX = rPIVOT - BLOCK_WIDTH;
            left = true;
        }
    }
}

//void Block::render(){
//    gBlock.render(PosX, PosY, NULL);
//}

#endif /* Block_h */
