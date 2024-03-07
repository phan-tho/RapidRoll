#ifndef Block_h
#define Block_h

class Block{
    public:
        const int BLOCK_HEIGHT = 10;
        const int BLOCK_WIDTH  = 65;
        
        int PosX, PosY;
    
        Block();
        
        void move();
        
//        void render();
};

Block::Block(){
    PosY = FLOOR;
    PosX = rand()%(rPIVOT - BLOCK_WIDTH - lPIVOT + 1) + lPIVOT;
}

void Block::move(){             // MOVE UP
    PosY -= DENTA_Y;
}

//void Block::render(){
//    gBlock.render(PosX, PosY, NULL);
//}

#endif /* Block_h */
