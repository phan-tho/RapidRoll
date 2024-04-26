#ifndef Game_h
#define Game_h

#include "Button.h"
#include "def.h"
#include "LTexture.h"
#include "Music/Music.h"
#include "Guide.h"

//#include "Header/ITEM/ITEM.h"
//#include "Header/CheckCollide.h"
LTexture gTextTexture;

class Game{
public:
    Game();
    
    void Play();

    // PROCESSING BLOCKS AND TRAPS -->
    void genItem(Fuel& fuel, Heart& heart);         // BLOCKS TRAPS HEART FUEL
    void moveBlocksAndTraps();
    void removeItemOutBoard();
    void renderBlocksAndTraps();

    // PROCESSING HEART -->
    void moveHeart(Heart& heart);
    void renderHeart(const Heart& heart);
    
    // PROCESSING FUEL -->
    void moveFuel(Fuel& fuel);
    void renderFuel(const Fuel& fuel);
    
    void moveBall(Ball& ball, const int& nearestPosBlock);
    void renderBall(Ball& ball);

    void renderEnergyBar(Ball& ball);

    void checkLifeBall(Ball& ball, Heart& heart, Fuel& fuel, int& life, const int& idNearTrap);     // DIE OR EAT HEART FUEL
    
    void updateScoreAndDentaY(int& score);
    
    // Reset para when replay
    void resetParameter();
    
    void renderLifeAndScore(const int& score, const int& life);
    
    // RENDER MESSAGE GAME OVER AND HIGH SCORE
    void renderGameOver(const int& score, const int& MODE);

    void close();
protected:
    int DENTA_Y;

    Pause OptionInGame;

    std::deque<Block> Blocks;               // distance is 170
    std::deque<Trap>  Traps;

    const int TIME_DELAY = 90;                  // 1.5s

    Music music;
    Guide guide;

    LTexture mBackground;

    LTexture mGameOver;

    LTexture mBlock;
    LTexture mHeart;
    LTexture mTrap;

    LTexture mFuel;

    std::vector<std::string> data;

    enum idData{
        USER_NAME,
        BASIC_MODE,
        SHOOTING_MODE
    };

private:
    const int BLOCK_ABOVE_TRAP = 2;             // 3  BLOCK ==> 1 TRAP
    const int BLOCK_ABOVE_HEART = 13;           // 15 BLOCK ==> 1 HEART
    const int BLOCK_ABOVE_FUEL = 18;            // AVOID = BLOCK_ABOVE_HEART

    const int MENU_OVER_HEIGHT = 106;
    const int MENU_OVER_WIDTH  = 320;

    int waitRevive;

    int cnt;
};

void Game::renderLifeAndScore(const int& score, const int& life){
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

void Game::renderGameOver(const int& score, const int& MODE){
    mGameOver.render((SCREEN_WIDTH - MENU_OVER_WIDTH)/2, (SCREEN_HEIGHT - MENU_OVER_HEIGHT)/2 - 30, NULL);
    
    SDL_Color textColor = {255, 255, 55};
    std::string textScore = data[USER_NAME] + "'s score " +  std::to_string(score);
    
    //    TTF_SetFontSize(gFont, 10);
    gTextTexture.loadFromRenderedText(textScore, textColor);
    gTextTexture.renderCustomSize(60, 410, 0.8);
    
    // UPDATE HIGHEST SCORE
    if(score > std::stoi(data[MODE])){
        data[MODE] = std::to_string(score);
    }
    textScore = data[USER_NAME] + "'s highest score " + data[MODE];
    
    std::ofstream out("YouCantSeeMe.txt");
    if(!out.is_open())      std::cout << "Deo mo duoc file\n";
    out << data[0] << "\n" << data[1] << "\n" << data[2];
    out.close();

    gTextTexture.loadFromRenderedText(textScore, textColor);
    gTextTexture.renderCustomSize(60, 440, 0.8);
}

void Game::genItem(Fuel& fuel, Heart& heart){
    if(cnt % ( (vGEN_BLOCK/DENTA_Y) *(BLOCK_ABOVE_TRAP + 1) ) == 0 && cnt){     // GEN TRAPS
        Trap trap;
        Traps.push_back(trap);
    }
    else if(cnt % (vGEN_BLOCK/DENTA_Y) == 0){                      // GEN BLOCK AND HEART
        Block block;
        Blocks.push_back(block);
        
        if( cnt % ( (vGEN_BLOCK/DENTA_Y)*(BLOCK_ABOVE_FUEL + 1) ) == 0 && cnt){
            fuel.assignPos(block.getX() + block.BLOCK_WIDTH/2 - fuel.FUEL_WIDTH/2, block.getY() - fuel.FUEL_HEIGHT);
            fuel.isEaten = false;                  // CAN DISPLAY AND EATEN BY BALL
            fuel.moveToLeft = 0;
            
            if(block.dynamic){
                fuel.moveToLeft += (block.left ? -block.dentaX : block.dentaX);        // PASS VELOCITY AND DIRECTLY TO HEART
            }
        }
        
        else if( cnt % ( (vGEN_BLOCK/DENTA_Y)*(BLOCK_ABOVE_HEART + 1) ) == 0 && cnt){
            heart.assignPos(block.getX() + block.BLOCK_WIDTH/2 - heart.HEART_WIDTH/2, block.getY() - heart.HEART_HEIGHT);
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
    for(auto it = Blocks.begin(); it != Blocks.end(); it++)          it->move(DENTA_Y);     // MOVE ALL BLOCKS                 // Increase DENTA_Y ==> More option
    for(auto it = Traps.begin() ; it != Traps.end() ; it++)          it->move(DENTA_Y);     // MOVE ALL TRAPS}
}
    
void Game::removeItemOutBoard(){
    if( !Blocks.empty() && ( *Blocks.begin() ).getY() <= CEILING ){
        Blocks.pop_front();
    }
    if( !Traps.empty()  && ( *Traps.begin()  ).getY() <= CEILING ){
        Traps.pop_front();
    }
}

void Game::renderBlocksAndTraps(){
    for(auto it = Blocks.begin(); it != Blocks.end(); it++){                            // RENDER ALL BLOCKS
        mBlock.render(it->getX(), it->getY(), NULL);
    }
    
    for(auto it = Traps.begin(); it != Traps.end(); it++){                              // RENDER ALL TRAPS
        mTrap.render(it->getX(), it->getY(), NULL);
    }
}

void Game::moveHeart(Heart& heart){
    heart.move(DENTA_Y);
}

void Game::renderHeart(const Heart& heart){
    if(heart.PosY >= CEILING && !heart.isEaten){
        mHeart.render(heart.PosX, heart.PosY, NULL);
    }
}

void Game::moveFuel(Fuel& fuel){
    fuel.move(DENTA_Y);
}

void Game::renderFuel(const Fuel& fuel){
    if(fuel.PosY >= CEILING && !fuel.isEaten){
        mFuel.render(fuel.PosX, fuel.PosY, NULL);
    }
    else if(fuel.PosY + fuel.FUEL_HEIGHT > CEILING){
        SDL_Rect xywh = { 0, CEILING - fuel.PosY, fuel.FUEL_WIDTH, fuel.FUEL_HEIGHT - CEILING + fuel.PosY};
        mFuel.render(fuel.PosX, CEILING, &xywh);
    }
}


void Game::moveBall(Ball& ball, const int& nearestPosBlock){
    ball.move( checkCollideBlock(ball, Blocks, nearestPosBlock), DENTA_Y);                                 // ball
    if( ball.mVelX ){
        music.whenMove();
    }
    else{
        music.whenNotMove();
    }
}

void Game::renderBall(Ball& ball){
    ball.render();
}

void Game::renderEnergyBar(Ball& ball){
    ball.renderEnergyBar();
}

void Game::updateScoreAndDentaY(int& score){
    score += DENTA_Y;
    DENTA_Y += (score%20000 == 0);
}

void Game::checkLifeBall(Ball& ball, Heart& heart, Fuel& fuel, int& life, const int& idNearTrap){
    if( checkCollideTrap(ball, Traps, idNearTrap) || ball.mPosY <= CEILING || ball.mPosY + ball.BALL_HEIGHT >= FLOOR){
        ball.mVelX = 0;
        ball.currentState = ball.DOWN;
        
        // BALL WILL APPEAR ABOVE LOWEST BLOCK FROM CEILING---------------------------------------------
        if( !Blocks.empty() ){
            ball.mPosY = Blocks.back().getY() - ball.BALL_HEIGHT;
            ball.mPosX = Blocks.back().getX() + (Blocks.back().BLOCK_WIDTH - ball.BALL_WIDTH)/2;
        }
        life--;
        waitRevive = 1;
        music.whenDie();        // MUSIC WHEN DIE
        // ---------------------------------------------------------------------------------------- DIED
        
    }
    else if( checkCollideHeart(ball, heart) ){
        heart.isEaten = true;
        heart.PosX = 0;
        heart.PosY = 0;
        life++;
        music.whenEatHeartFuel();   // MUSIC WHEN EAT
    }
    
    else if( checkCollideFuel(ball, fuel) ){
        fuel.isEaten = true;
        fuel.PosX = 0;
        fuel.PosY = 0;
        ball.energy = (fmin(500, ball.energy + 160));
        music.whenEatHeartFuel();   // MUSIC WHEN EAT
    }
}

void Game::resetParameter(){
    cnt = 0;

    DENTA_Y = 2;
    waitRevive = 0;

    Blocks.clear();
    Traps.clear();
}

void Game::close(){
    mBackground.freeFire();

    mGameOver.freeFire();
    
    mBlock.freeFire();
    mHeart.freeFire();
    mTrap.freeFire();
    
    mFuel.freeFire();
    
    std::ofstream out("YouCantSeeMe.txt");
    if(!out.is_open())      std::cout << "open file error at Game->close\n";
    out << data[USER_NAME] << "\n" << data[BASIC_MODE] << "\n" << data[SHOOTING_MODE] << "\n";
    out.close();
}

Game::Game(){
    std::ifstream in("YouCantSeeMe.txt");
    if(!in.is_open())   std::cout << "open file error at Game(init)\n";
    std::string str;
    while(getline(in, str)){
        data.push_back(str);
    }
    in.close();
    
    
    cnt = 0;

    DENTA_Y = 2;
    waitRevive = 0;
    
    mBackground.loadFromFile("BackGr.png");
    
    mGameOver.loadFromFile("GameOver.png");
    
    mBlock.loadFromFile("Block.png");
    mHeart.loadFromFile("tim.png");
    mTrap.loadFromFile("trap.png");
    
    mFuel.loadFromFile("fuel.png");
}

#endif /* Game_h */
