#ifndef Game_h
#define Game_h

#include "Header/Pause.h"
#include "Header/def.h"
#include "Header/LTexture.h"
#include "Header/defLTexture.h"
#include "Header/defMusic.h"

//#include "Header/ITEM/ITEM.h"
//#include "Header/CheckCollide.h"

class Game{
    public:
        Game();
        
        void Play();
    
        void resetParameter();
        
        void renderLifeAndScore();
    
        // PROCESSING BLOCKS AND TRAPS -->
        void genItem();         // BLOCKS TRAPS HEART FUEL
        void moveBlocksAndTraps();
        void removeItemOutBoard();
        void renderBlocksAndTraps();
    
        // PROCESSING HEART -->
        void moveHeart();
        void renderHeart();
        
        // PROCESSING FUEL -->
        void moveFuel();
        void renderFuel();
        
        void moveBall();
        void renderBall();
    
        void renderEnergyBar();
    
        void checkLifeBall();     // DIE OR EAT HEART FUEL
    
        void updateScoreAndDentaY();
    private:
        const int BLOCK_ABOVE_TRAP = 3;             // 4  BLOCK ==> 1 TRAP
        const int BLOCK_ABOVE_HEART = 13;           // 15 BLOCK ==> 1 HEART
        const int BLOCK_ABOVE_FUEL = 18;            // AVOID = BLOCK_ABOVE_HEART

        const int TIME_DELAY = 90;                  // 1.5s
        int cntTime;
        int waitRevive;

        int cnt, life;
        int score;              // CALCULATE WHEN SOME BLOCK OR TRAP REMOVE
        int DENTA_Y;
    
        int nearestPosBlock;

        Dot dot;
        Heart heart;
        Fuel fuel;
    
        Pause OptionInGame;
        
        std::deque<Block> Blocks;               // distance is 170
        std::deque<Trap>  Traps;
    
        // Musical
    enum Channel{
            FIRST_CHANNEL = 0,
            SECOND_CHANNEL = 1
        };
};

void Game::Play(){
    // Play Background Music
//    Mix_PlayMusic(gBackGrMusic, -1);

    bool quit = false;
    SDL_Event e;
    
    while( !quit ){
        while( SDL_PollEvent( &e ) != 0 ){
            if( e.type == SDL_QUIT || e.key.keysym.sym == SDLK_x)        quit = true;
//            // Play gMusicWhenMove when keydown
//            if( e.type == SDL_KEYDOWN && e.key.repeat == 0 ){
//                if( e.key.keysym.sym == SDLK_LEFT || e.key.keysym.sym == SDLK_RIGHT){
//                    Mix_PlayChannel(-1, gMusicWhenMove, 0);
//                }
//            }
            
            //Handle input for the dot
            dot.handleEvent( e, DENTA_Y );
            OptionInGame.handleEvent( &e );
            
            if (OptionInGame.mCurrentState[EXIT]){   quit = true;   }        // EXIT
        }
        
        if(OptionInGame.mCurrentState[REPLAY]){                     // RESET PARAMETER WHEN REPLACE
            resetParameter();
            OptionInGame.mCurrentState[PAUSE] = false;
            OptionInGame.mCurrentState[REPLAY] = false;
            
            // PLAY ONLY BACKGROUND MUSIC
            Mix_PlayMusic(gBackGrMusic, -1);
        }
        
        SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
        SDL_RenderClear( gRenderer );
        
        gBackground.render(0, 0, NULL);             // BACKGROUND
        OptionInGame.render();                      // OPTION PAUSE PLAY REPLAY EXIT
        renderLifeAndScore();
        
        if(OptionInGame.mCurrentState[PAUSE]){          // PAUSE
            renderBlocksAndTraps();
            renderHeart();
            renderFuel();
            if(life){
                renderBall();
                renderEnergyBar();
            }
            else{
                gGameOver.render(54, 310, NULL);
            }
    
            OptionInGame.render();
            
            // pause music
            Mix_PauseMusic();
            Mix_HaltChannel(FIRST_CHANNEL);
            Mix_HaltChannel(SECOND_CHANNEL);
            
            SDL_RenderPresent( gRenderer );
            continue;
        }
        else{
            // is music is paused, resume music
            if(Mix_PausedMusic()){
                Mix_ResumeMusic();
            }
        }
        
        if(life > 0){               // WHEN PAUSE = FALSE
            genItem();
            moveBlocksAndTraps();
            removeItemOutBoard();
            renderBlocksAndTraps();
        
        
            // PROCESSING HEART -->
            moveHeart();
            renderHeart();
            
            moveFuel();
            renderFuel();
            
            moveBall();
            renderBall();
            renderEnergyBar();
            
            updateScoreAndDentaY();
            
            checkLifeBall();     // DIE OR EAT HEART
            // APPEAR AFTER 1 SECOND
        }
                                            // GAME OVER
        else if(life == 0){
            if(cntTime++ < TIME_DELAY){
                moveBlocksAndTraps();
                removeItemOutBoard();
                renderBlocksAndTraps();
        
                moveHeart();
                renderHeart();
                
                moveFuel();
                renderFuel();
            }
            else{
                renderFuel();
                renderBlocksAndTraps();
                renderHeart();
                
                gGameOver.render(54, 310, NULL);            // MAGIC
                OptionInGame.mCurrentState[PAUSE] = true;
                
                // pause music
                Mix_PauseMusic();
                Mix_HaltChannel(FIRST_CHANNEL);
                Mix_HaltChannel(SECOND_CHANNEL);
                
//                std::cout << Blocks.front().PosX - (*(Blocks.begin() + 1)).PosX;
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
    gTextTexture.render(130, 20, NULL);
}

void Game::genItem(){
    if(cnt % ( (vGEN_BLOCK/DENTA_Y) *(BLOCK_ABOVE_TRAP + 1) ) == 0 && cnt){     // GEN TRAPS
        Trap trap;
        Traps.push_back(trap);
    }
    else if(cnt % (vGEN_BLOCK/DENTA_Y) == 0){                      // GEN BLOCK AND HEART
        Block block;
        Blocks.push_back(block);
        
        if( cnt % ( (vGEN_BLOCK/DENTA_Y)*(BLOCK_ABOVE_FUEL + 1) ) == 0 && cnt){
            fuel.assignPos(block.PosX + block.BLOCK_WIDTH/2 - fuel.FUEL_WIDTH/2, block.PosY - fuel.FUEL_HEIGHT);
            fuel.isEaten = false;                  // CAN DISPLAY AND EATEN BY BALL
            fuel.moveToLeft = 0;
            
            if(block.dynamic){
                fuel.moveToLeft += (block.left ? -block.dentaX : block.dentaX);        // PASS VELOCITY AND DIRECTLY TO HEART
            }
        }
        
        else if( cnt % ( (vGEN_BLOCK/DENTA_Y)*(BLOCK_ABOVE_HEART + 1) ) == 0 && cnt){
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

void Game::moveHeart(){
    heart.move(DENTA_Y);
}

void Game::renderHeart(){
    if(heart.PosY >= CEILING && !heart.isEaten){
        gHeart.render(heart.PosX, heart.PosY, NULL);
    }
}

void Game::moveFuel(){
    fuel.move(DENTA_Y);
}

void Game::renderFuel(){
    if(fuel.PosY >= CEILING && !fuel.isEaten){
        gFuel.render(fuel.PosX, fuel.PosY, NULL);
    }
    else if(fuel.PosY + fuel.FUEL_HEIGHT > CEILING){
        SDL_Rect xywh = { 0, CEILING - fuel.PosY, fuel.FUEL_WIDTH, fuel.FUEL_HEIGHT - CEILING + fuel.PosY};
        gFuel.render(fuel.PosX, CEILING, &xywh);
    }
}


void Game::moveBall(){
    nearestPosBlock = findBlockSameY(dot, Blocks);
    dot.move( checkCollideBlock(dot, Blocks, nearestPosBlock ), DENTA_Y);                                 // DOT
    
    // play gMusicWhenMove when dot.mVelX != 0
//    if(dot.mVelX != 0){
//        // play when music is paused
//        if(!Mix_Playing(FIRST_CHANNEL)){
//            Mix_PlayChannel(FIRST_CHANNEL, gMusicWhenMove, -1);
//        }
//    }
//    else{
//        // PAUSE MUSIC IN CHANNEL 0 if this channel is playing
//        if(Mix_Playing(FIRST_CHANNEL)){
//            Mix_HaltChannel(FIRST_CHANNEL);
//            // play gTailFireMove
//            Mix_PlayChannel(SECOND_CHANNEL, gTailFireMove, -1);
//        }
//    }
}

void Game::renderBall(){
    if(waitRevive == 0){
        gDotTexture.render(dot.getX(), dot.getY(), NULL);
        
        int xFire = dot.mPosX - (dot.FIRE_WIDTH - dot.DOT_WIDTH)/2;
        int yFire = dot.mPosY - dot.FIRE_HEIGHT + dot.DOT_HEIGHT;
        
        SDL_Point centre = { dot.FIRE_WIDTH/2, dot.FIRE_HEIGHT - dot.DOT_HEIGHT/2 };
        gFire[(cnt/9)%6].renderFlip( xFire , yFire, NULL, dot.currentState, &centre , SDL_FLIP_NONE);
    }
    else{
        if((waitRevive/20)&1){              // dot appear and disappear
            gDotTexture.render(dot.getX(), dot.getY(), NULL);
            
//            int xFire = dot.mPosX - (dot.FIRE_WIDTH - dot.DOT_WIDTH)/2;                   // render fire
//            int yFire = dot.mPosY - dot.FIRE_HEIGHT + dot.DOT_HEIGHT;
//            
//            SDL_Point centre = { dot.FIRE_WIDTH/2, dot.FIRE_HEIGHT - dot.DOT_HEIGHT/2 };
//            gFire[(cnt/9)%6].renderFlip( xFire , yFire, NULL, dot.currentState, &centre , SDL_FLIP_NONE);
        }
        waitRevive++;
        if(waitRevive >= TIME_DELAY){
            waitRevive = 0;
        }
    }
}

void Game::renderEnergyBar(){
    if(dot.energy >= 80){
        SDL_Rect splitBar = { 0, dot.ENERGY_BAR_HEIGHT/10 - dot.energy/10 + dot.ENERGY_BAR_WIDTH/2, dot.ENERGY_BAR_WIDTH, dot.energy/10 - dot.ENERGY_BAR_WIDTH/2 };         // x y w h
        int x = dot.mPosX + dot.DOT_WIDTH + dot.DISTANCE_BAR_BALL;
        int y = dot.mPosY + dot.DOT_HEIGHT - dot.energy/10;
        gEnergyBar.render( x, y + dot.ENERGY_BAR_WIDTH/2, &splitBar);
        
        splitBar = { 0, 0, dot.ENERGY_BAR_WIDTH, dot.ENERGY_BAR_WIDTH/2 };
        gEnergyBar.render(x, y, &splitBar);
    }
//    else if(dot.energy > 0){
//        SDL_Rect splitBar = { 0, dot.ENERGY_BAR_HEIGHT/10 - dot.energy/10 + dot.energy/20, dot.ENERGY_BAR_WIDTH, dot.energy/10 - dot.energy/20 };         // x y w h
//        int x = dot.mPosX + dot.DOT_WIDTH + dot.DISTANCE_BAR_BALL;
//        int y = dot.mPosY + dot.DOT_HEIGHT - dot.energy/10;
//        gEnergyBar.render( x, y + dot.energy/2, &splitBar);
//        
//        splitBar = { 0, 0, dot.ENERGY_BAR_WIDTH, dot.energy/20 };
//        gEnergyBar.render(x, y, &splitBar);
//    }
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
    
    else if( checkCollideFuel(dot, fuel) ){
        fuel.isEaten = true;
        fuel.PosX = 0;
        fuel.PosY = 0;
        dot.energy = (fmin(500, dot.energy + 160));
    }
}

void Game::resetParameter(){
    cnt = 0;
    life = 3;
    score = 0;
    DENTA_Y = 2;
    waitRevive = 0;
    dot.energy = 0;
    
    dot.mPosY = CEILING + 120;                       // MAGIC
    dot.mPosX = (SCREEN_WIDTH - dot.DOT_WIDTH)/2;
    dot.mVelX = 0;
    
    heart.PosX = 0;
    heart.PosY = 0;
    heart.moveToLeft = 0;
    
    fuel.PosX = 0;
    fuel.PosY = 0;
    fuel.moveToLeft = 0;
    
    Blocks.clear();
    Traps.clear();
}

Game::Game(){
    cnt = 0;
    life = 3;
    score = 0;
    DENTA_Y = 2;
    waitRevive = 0;
    cntTime = 0;
    
    Block::dentaX = 1;
    Block::staticAboveDyn = 4;
    
    Trap::dentaX = 1;
    Trap::staticAboveDyn = 3;
    
//    nearestPosBlock = 0;
}

#endif /* Game_h */
