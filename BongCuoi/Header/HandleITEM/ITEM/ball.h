#ifndef ball_h
#define ball_h

#include "Block.h"
#include "Trap.h"

class Ball{
    public:
        static const int BALL_WIDTH = 20;
        static const int BALL_HEIGHT = 20;
    
        const int FIRE_WIDTH = 36;
        const int FIRE_HEIGHT = 46;
    
        const int ENERGY_BAR_WIDTH = 8;
        const int ENERGY_BAR_HEIGHT = 800;
        const int DISTANCE_BAR_BALL = 15;
    
        SDL_Keycode moveUp = SDLK_s;
        SDL_Keycode moveLeft = SDLK_a;
        SDL_Keycode moveRight = SDLK_d;
//        const SDL_Keycode moveUp;
//        const SDL_Keycode moveLeft;
//        const SDL_Keycode moveRight;
    
        enum fireState{
            UP = 0,
            DOWN = 0,
            UP_LEFT = 12,
            UP_RIGHT = -12,
            DOWN_LEFT = 30,
            DOWN_RIGHT = -30
        };
        
        int currentState;
    
        //The X and Y offsets of the dot
        int mPosX, mPosY;
    
        int mVelX;
        int vJetEngine;

//        Ball(SDL_KeyCode moveUp, SDL_Keycode moveRight, SDL_Keycode MoveLeft);
        Ball();
        void reset();

        // CHANGE VELOCITY DEPEND TYPED KEY
        void handleEvent( const SDL_Event& e, const int& DENTA_Y);

        //Moves the dot
        void move(const int& up, const int& DENTA_Y);

        //Shows the dot on the screen
        void render();
    
        void renderEnergyBar();
    
        void close();
    
        int energy, life;
    private:
        LTexture BallTexture;
        LTexture FireTexture[6];
        LTexture energyBarTexture;
    
        int waitRevive, cnt;
        const int TIME_DELAY = 90;
    
        enum StateEngine {
            NOT = 0,
            GOING_UP = 1,
        };
};

void Ball::handleEvent( const SDL_Event& e, const int& DENTA_Y ){
    if( e.type == SDL_KEYDOWN && e.key.repeat == 0){
        // Adjust the velocity
        if(!energy)     vJetEngine = NOT;
        else{
            if( e.key.keysym.sym == moveUp){
                vJetEngine = GOING_UP;
            }
            else{
                vJetEngine = NOT;
            }
        }
        
        if(e.key.keysym.sym == moveLeft){         // move left
            mVelX -= (DENTA_Y + 1);
        }
        else if(e.key.keysym.sym == moveRight){     // move right
            mVelX += (DENTA_Y + 1);
        }
    }

    //If a key was released
    if( e.type == SDL_KEYUP){
        //Adjust the velocity
        if( e.key.keysym.sym == moveUp){
            vJetEngine = NOT;
        }
        
        if(e.key.keysym.sym == moveLeft || e.key.keysym.sym == moveRight){
            mVelX = 0;
        }
    }
}

void Ball::move(const int& upANDdyn, const int& DENTA_Y){
    if(!energy)     vJetEngine = NOT;
    
    if(upANDdyn == -20){             // NOT COLLIDE
        if(vJetEngine == GOING_UP){
            mPosY -= DENTA_Y/2;
            mPosX += mVelX*5/3;
            energy -= 3;
        }
        else{
            mPosY += DENTA_Y;
            mPosX += mVelX;
        }
        
        if(mVelX > 0)           currentState = DOWN_RIGHT;
        else if(mVelX < 0)      currentState = DOWN_LEFT;
        else                    currentState = DOWN;
    }
    else{                           // COLLIDE
        mPosY -= DENTA_Y;
        mPosX += mVelX + upANDdyn;
        if(mVelX + upANDdyn > 0)        currentState = UP_RIGHT;
        else if(mVelX + upANDdyn < 0)   currentState = UP_LEFT;
        else                            currentState = UP;
    }

    mPosX = fmin(mPosX, rPIVOT - BALL_WIDTH);
    mPosX = fmax(mPosX, lPIVOT);
}

void Ball::reset(){
    energy = 0;
    mPosY = CEILING + 120;                       // MAGIC
    mPosX = (SCREEN_WIDTH - BALL_WIDTH)/2;
    mVelX = 0;
}

//Ball::Ball(SDL_KeyCode moveUp, SDL_Keycode moveRight, SDL_Keycode MoveLeft) : moveUp(moveUp), moveLeft(MoveLeft), moveRight(moveRight) {
Ball::Ball(){
    //Initialize the offsets
    mPosY = CEILING + 120;                       // MAGIC
    mPosX = (SCREEN_WIDTH - BALL_WIDTH)/2;

    //Initialize the velocity
    mVelX = 0;
    vJetEngine = 0;
    waitRevive = 0; cnt = 0;
    
    energy = 0;
    life = 10;
    
    currentState = DOWN;
    
    BallTexture.loadFromFile("Ball.png");
    
    FireTexture[0].loadFromFile("fire1.png");
    FireTexture[1].loadFromFile("fire2.png");
    FireTexture[2].loadFromFile("fire3.png");
    FireTexture[3].loadFromFile("fire4.png");
    FireTexture[4].loadFromFile("fire5.png");
    FireTexture[5].loadFromFile("fire6.png");
    
    energyBarTexture.loadFromFile("energyBar.png");
}

void Ball::close(){
    BallTexture.freeFire();
    for(int i = 0; i < 6; i++)      FireTexture[i].freeFire();
    energyBarTexture.freeFire();
}

void Ball::render(){
    if(waitRevive == 0){
        BallTexture.render(mPosX, mPosY, NULL);
        
        int xFire = mPosX - (FIRE_WIDTH - BALL_WIDTH)/2;
        int yFire = mPosY - FIRE_HEIGHT + BALL_HEIGHT;
        
        SDL_Point centre = { FIRE_WIDTH/2, FIRE_HEIGHT - BALL_HEIGHT/2 };
        FireTexture[(cnt/9)%6].renderFlip( xFire , yFire, NULL, currentState, &centre , SDL_FLIP_NONE);
        cnt = (cnt + 1)%54;
    }
    else{
        if((waitRevive/20)&1){              // dot appear and disappear
            BallTexture.render(mPosX, mPosY, NULL);
            
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

void Ball::renderEnergyBar(){
    if(energy >= 80){
        SDL_Rect splitBar = { 0, ENERGY_BAR_HEIGHT/10 - energy/10 + ENERGY_BAR_WIDTH/2, ENERGY_BAR_WIDTH, energy/10 - ENERGY_BAR_WIDTH/2 };         // x y w h
        int x = mPosX + BALL_WIDTH + DISTANCE_BAR_BALL;
        int y = mPosY + BALL_HEIGHT - energy/10;
        energyBarTexture.render( x, y + ENERGY_BAR_WIDTH/2, &splitBar);
        
        splitBar = { 0, 0, ENERGY_BAR_WIDTH, ENERGY_BAR_WIDTH/2 };
        energyBarTexture.render(x, y, &splitBar);
    }
}

#endif /* ball_h */
