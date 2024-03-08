#ifndef Game_h
#define Game_h

class Game{
    private:
        int cnt;
        int life;

        Dot dot;
        Heart heart;
        
        std::deque<Block> Blocks;
        std::deque<Trap>  Traps;
    
    public:
        Game();
        
        void Play();
        
        
        // PROCESSING BLOCKS AND TRAPS -->
        void genBlocksTrapsHeart();
        
        void moveBlocksAndTraps();
        
        void removeItemOutBoard();
        
        void renderBlocksAndTraps();
        
    
        // PROCESSING HEART -->
        void moveAndRenderHeart();
        
        
        void moveAndRenderBall();
        
        void checkLifeBall();     // DIE OR EAT HEART
        
};


void Game::genBlocksTrapsHeart(){
    if(cnt % (vGEN_BLOCK*(BLOCK_ABOVE_TRAP + 1) ) == 0 && cnt){     // GEN TRAPS
        Trap trap;
        Traps.push_back(trap);
    }
    else if(cnt % vGEN_BLOCK == 0){                      // GEN BLOCK AND HEART
        Block block;
        Blocks.push_back(block);
        if( cnt % ( vGEN_BLOCK*(BLOCK_ABOVE_HEART + 1) ) == 0 && cnt){
            heart.assignPos(block.PosX + block.BLOCK_HEIGHT/2 + heart.HEART_WIDTH, block.PosY - heart.HEART_HEIGHT);
            heart.isEaten = false;
        }
    }
    cnt++;
}

void Game::moveBlocksAndTraps(){
    for(auto it = Blocks.begin(); it != Blocks.end(); it++)          (*it).move();     // MOVE ALL BLOCKS
    for(auto it = Traps.begin() ; it != Traps.end() ; it++)          (*it).move();     // MOVE ALL TRAPS
}

void Game::removeItemOutBoard(){
    if( !Blocks.empty() && ( *Blocks.begin() ).PosY <= CEILING )           Blocks.pop_front();
    if( !Traps.empty()  && ( *Traps.begin()  ).PosY <= CEILING )           Traps.pop_front();
}

void Game::renderBlocksAndTraps(){
    for(auto it = Blocks.begin(); it != Blocks.end(); it++){                            // RENDER ALL BLOCKS
        gBlock.render((*it).PosX, (*it).PosY, NULL);
    }
    
    for(auto it = Traps.begin(); it != Traps.end(); it++){                              // RENDER ALL TRAPS
        gTrap.render((*it).PosX, (*it).PosY, NULL);
    }
}

void Game::moveAndRenderHeart(){
    heart.move();
    if(heart.PosY >= CEILING && !heart.isEaten){
        gHeart.render(heart.PosX, heart.PosY, NULL);
    }
}

void Game::moveAndRenderBall(){
    dot.move( checkCollideBlock(dot, Blocks) );                                 // DOT
    gDotTexture.render(dot.getX(), dot.getY(), NULL);
}

void Game::checkLifeBall(){
    int dentaLife = 0;
    
    if( checkCollideTrap(dot, Traps) || dot.mPosY <= CEILING || dot.mPosY + dot.DOT_HEIGHT >= FLOOR){
        dot = Dot();
        
        // BALL WILL APPEAR ABOVE LOWEST BLOCK FROM CEILING---------------------------------------------
        if( !Blocks.empty() ){
            dot.mPosY = Blocks.back().PosY - dot.DOT_HEIGHT;
            dot.mPosX = Blocks.back().PosX + (Blocks.back().BLOCK_WIDTH - dot.DOT_WIDTH)/2;
        }
        dentaLife = -1;
        // ---------------------------------------------------------------------------------------- DIED
        
    }
    else if( checkCollideHeart(dot, heart) ){
        heart.isEaten = true;
        heart.PosX = 0;
        heart.PosY = 0;
        dentaLife = 1;
    }
    
    life += dentaLife;
}

Game::Game(){
    cnt = 0;
    life = 3;
}

#endif /* Game_h */
