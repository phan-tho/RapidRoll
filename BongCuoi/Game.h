#ifndef Game_h
#define Game_h

class Game{
    private:
//        const int BLOCK_ABOVE_TRAP = 3;             // 4  BLOCK ==> 1 TRAP
//        const int BLOCK_ABOVE_HEART = 30;           // 15 BLOCK ==> 1 HEART
    
        int cnt, life;
        int score;              // CALCULATE WHEN SOME BLOCK OR TRAP REMOVE
        int DENTA_Y;
//        int waitRevive;
//    
//        const int TIME_REVIVE = 90;                 // TIME TO BACK GAME. FPS ≈ 60 ==> 1.5S

        Dot dot;
        Heart heart;
        
        std::deque<Block> Blocks;
        std::deque<Trap>  Traps;
    
    public:
        Game();
        
        void Play();
        
        void renderLifeAndScore();
    
        // PROCESSING BLOCKS AND TRAPS -->
        void genBlocksTrapsHeart();
        
        void moveBlocksAndTraps();
        
        void removeItemOutBoard();
        
        void renderBlocksAndTraps();
        
    
        // PROCESSING HEART -->
        void moveAndRenderHeart();
        
        void moveAndRenderBall();
    
        void updateScoreAndDentaY();
        
        void checkLifeBall();     // DIE OR EAT HEART
        
};

void Game::Play(){
    bool quit = false;
    SDL_Event e;
    
    while( !quit ){
        while( SDL_PollEvent( &e ) != 0 ){
            if( e.type == SDL_QUIT || e.key.keysym.sym == SDLK_x)        quit = true;
            
            //Handle input for the dot
            dot.handleEvent( e );
        }
        
        SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
        SDL_RenderClear( gRenderer );
        
        //Render objects
        gBackground.render(0, 0, NULL);             // BACKGROUND
        renderLifeAndScore();
        
        if(life > 0){
            genBlocksTrapsHeart();
            
            moveBlocksAndTraps();
            
            removeItemOutBoard();
            
            renderBlocksAndTraps();
            
        
            // PROCESSING HEART -->
            moveAndRenderHeart();
            
            moveAndRenderBall();
            
            updateScoreAndDentaY();
            
            checkLifeBall();     // DIE OR EAT HEART
        }

                                                // GAME OVER
        
        else{
            if(!Blocks.empty() || !Traps.empty()){
                moveBlocksAndTraps();
                
                removeItemOutBoard();
                
                renderBlocksAndTraps();
                
                moveAndRenderHeart();
            }
            else{
                gGameOver.render(54, 310, NULL);            // MAGIC NUMBERS
            }
        }

        //Update screen
        SDL_RenderPresent( gRenderer );
        
    }
}

void Game::renderLifeAndScore(){
    std::string str = 'X' + std::to_string(life);
    SDL_Color textColor = {0, 0, 0};
    
    gTextTexture.loadFromRenderedText(str, textColor);
    gTextTexture.render(60, 20, NULL);
    
    str = '0';
    auto leng = (std::to_string(score)).size();
    for(int i = 0; i < 10 - leng; i++)          str += '0';
    str += std::to_string(score);
    gTextTexture.loadFromRenderedText(str, textColor);
    gTextTexture.render(170, 20, NULL);
}

void Game::genBlocksTrapsHeart(){
    if(cnt % ( (vGEN_BLOCK/DENTA_Y) *(BLOCK_ABOVE_TRAP + 1) ) == 0 && cnt){     // GEN TRAPS
        Trap trap;
        Traps.push_back(trap);
    }
    else if(cnt % (vGEN_BLOCK/DENTA_Y) == 0){                      // GEN BLOCK AND HEART
        Block block;
        Blocks.push_back(block);
        if( cnt % ( (vGEN_BLOCK/DENTA_Y)*(BLOCK_ABOVE_HEART + 1) ) == 0 && cnt){
            heart.assignPos(block.PosX + block.BLOCK_WIDTH/2 - heart.HEART_WIDTH/2, block.PosY - heart.HEART_HEIGHT);
            heart.isEaten = false;
            heart.moveToLeft = 0;
            
            if(block.dynamic){
                heart.moveToLeft += (block.left ? -block.dentaX : block.dentaX);
            }
        }
    }
    cnt++;
}


void Game::moveBlocksAndTraps(){
    for(auto it = Blocks.begin(); it != Blocks.end(); it++)          (*it).move(DENTA_Y);     // MOVE ALL BLOCKS                 // Increase DENTA_Y ==> More option
    for(auto it = Traps.begin() ; it != Traps.end() ; it++)          (*it).move(DENTA_Y);     // MOVE ALL TRAPS}
}
    
void Game::removeItemOutBoard(){
    if( !Blocks.empty() && ( *Blocks.begin() ).PosY <= CEILING ){
        Blocks.pop_front();
    }
    if( !Traps.empty()  && ( *Traps.begin()  ).PosY <= CEILING ){
        Traps.pop_front();
    }
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
    heart.move(DENTA_Y);
    if(heart.PosY >= CEILING && !heart.isEaten){
        gHeart.render(heart.PosX, heart.PosY, NULL);
    }
}

void Game::moveAndRenderBall(){
    dot.move( checkCollideBlock(dot, Blocks), DENTA_Y);                                 // DOT
    gDotTexture.render(dot.getX(), dot.getY(), NULL);
}

void Game::updateScoreAndDentaY(){
    score += DENTA_Y;
    DENTA_Y += (score%15000 == 0);
}

void Game::checkLifeBall(){
    
    if( checkCollideTrap(dot, Traps) || dot.mPosY <= CEILING || dot.mPosY + dot.DOT_HEIGHT >= FLOOR){
        dot = Dot();
        
        // BALL WILL APPEAR ABOVE LOWEST BLOCK FROM CEILING---------------------------------------------
        if( !Blocks.empty() ){
            dot.mPosY = Blocks.back().PosY - dot.DOT_HEIGHT;
            dot.mPosX = Blocks.back().PosX + (Blocks.back().BLOCK_WIDTH - dot.DOT_WIDTH)/2;
        }
        life--;
        // ---------------------------------------------------------------------------------------- DIED
        
    }
    else if( checkCollideHeart(dot, heart) ){
        heart.isEaten = true;
        heart.PosX = 0;
        heart.PosY = 0;
        life++;
    }
}

Game::Game(){
    cnt = 0;
    life = 3;
    score = 0;
    DENTA_Y = 2;
    
    Block::dentaX = 1;
    Block::staticAboveDyn = 5;
    
    Trap::dentaX = 1;
    Trap::staticAboveDyn = 5;
}

#endif /* Game_h */
