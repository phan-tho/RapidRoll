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
    
//    Heart heartOnBlock((PosX + BLOCK_WIDTH)/2, PosY - BLOCK_HEIGHT);
}

void Block::move(){
    PosY -= Denta_Y;
}

//void Block::render(){
//    gBlock.render(PosX, PosY, NULL);
//}

#endif /* Block_h */
