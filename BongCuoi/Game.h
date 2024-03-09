#ifndef Game_h
#define Game_h

class Game{
    private:
        const int BLOCK_ABOVE_TRAP = 3;             // 4  BLOCK ==> 1 TRAP
        const int BLOCK_ABOVE_HEART = 30;           // 15 BLOCK ==> 1 HEART
    
        const int TIME_DELAY = 90;                  // 1.5s
        int cntTime = 0;
        int waitRevive;
    
        int cnt, life;
        int score;              // CALCULATE WHEN SOME BLOCK OR TRAP REMOVE
        int DENTA_Y;

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
        void checkLifeBall();     // DIE OR EAT HEART
    
        void updateScoreAndDentaY();
        
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
            // APPEAR AFTER 1 SECOND
        }
                                            // GAME OVER
        else{
            if(cntTime++ < TIME_DELAY){
                moveBlocksAndTraps();
                removeItemOutBoard();
                renderBlocksAndTraps();
        
                moveAndRenderHeart();
            }
            else{
                renderBlocksAndTraps();
                
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
    for(int i = 0; i < LENGTH_SCORE - leng; i++)          str += '0';
    str += std::to_string(score);
    gTextTexture.loadFromRenderedText(str, textColor);
    gTextTexture.render(210, 20, NULL);
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
            heart.isEaten = false;                  // CAN DISPLAY AND EATEN BY BALL
            heart.moveToLeft = 0;
            
            if(block.dynamic){
                heart.moveToLeft += (block.left ? -block.dentaX : block.dentaX);        // PASS VELOCITY AND DIRECTLY TO HEART
            }
        }
    }
    cnt = (cnt + 1)%( (vGEN_BLOCK/DENTA_Y)*(BLOCK_ABOVE_TRAP + 1)*10 );         // AVOID CASE OVERFLOW NUM AND TRAP NOT APPEAR AT TENTH
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
    
    if(waitRevive == 0){
        gDotTexture.render(dot.getX(), dot.getY(), NULL);
        
        int xFire = dot.mPosX - (dot.FIRE_WIDTH - dot.DOT_WIDTH)/2;
        int yFire = dot.mPosY - dot.FIRE_HEIGHT + dot.DOT_HEIGHT;
        
        SDL_Point centre = { dot.FIRE_WIDTH/2, dot.FIRE_HEIGHT - dot.DOT_HEIGHT/2 };
        gFire[(cnt/9)%6].renderFlip( xFire , yFire, NULL, dot.currentState, &centre , SDL_FLIP_NONE);
    }
    else{
        if((waitRevive/20)&1){
            gDotTexture.render(dot.getX(), dot.getY(), NULL);
            
            int xFire = dot.mPosX - (dot.FIRE_WIDTH - dot.DOT_WIDTH)/2;
            int yFire = dot.mPosY - dot.FIRE_HEIGHT + dot.DOT_HEIGHT;
            
            SDL_Point centre = { dot.FIRE_WIDTH/2, dot.FIRE_HEIGHT - dot.DOT_HEIGHT/2 };
            gFire[(cnt/9)%6].renderFlip( xFire , yFire, NULL, dot.currentState, &centre , SDL_FLIP_NONE);
        }
        waitRevive++;
        if(waitRevive >= TIME_DELAY){
            waitRevive = 0;
        }
    }
}

void Game::updateScoreAndDentaY(){
    score += DENTA_Y;
    DENTA_Y += (score%20000 == 0);
}

void Game::checkLifeBall(){
    
    if( checkCollideTrap(dot, Traps) || dot.mPosY <= CEILING || dot.mPosY + dot.DOT_HEIGHT >= FLOOR){
//        dot = Dot();
        dot.mVelX = 0;
        dot.currentState = dot.DOWN;
        
        // BALL WILL APPEAR ABOVE LOWEST BLOCK FROM CEILING---------------------------------------------
        if( !Blocks.empty() ){
            dot.mPosY = Blocks.back().PosY - dot.DOT_HEIGHT;
            dot.mPosX = Blocks.back().PosX + (Blocks.back().BLOCK_WIDTH - dot.DOT_WIDTH)/2;
        }
        life--;
        waitRevive = 1;
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
    waitRevive = 0;
    
    Block::dentaX = 1;
    Block::staticAboveDyn = 5;
    
    Trap::dentaX = 1;
    Trap::staticAboveDyn = 3;
}

#endif /* Game_h */
