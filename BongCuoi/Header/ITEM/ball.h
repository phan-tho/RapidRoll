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

        Ball();
        void reset();

        // CHANGE VELOCITY DEPEND TYPED KEY
        void handleEvent( SDL_Event& e, const int& DENTA_Y );

        //Moves the dot
        void move(const int& up, const int& DENTA_Y);

        //Shows the dot on the screen
        void render();
    
        int energy;
    private:
        enum StateEngine {
            NOT = 0,
            GOING_UP = 1,
        };
};

void Ball::handleEvent( SDL_Event& e, const int& DENTA_Y ){
    if( e.type == SDL_KEYDOWN && e.key.repeat == 0 ){
        //Adjust the velocity
        if(!energy)     vJetEngine = NOT;
        else{
            if( e.key.keysym.sym == SDLK_SPACE || e.key.keysym.sym == SDLK_g || e.key.keysym.sym == SDLK_UP){
                vJetEngine = GOING_UP;
            }
            else{
                vJetEngine = NOT;
            }
        }
        
        switch( e.key.keysym.sym ){
            case SDLK_LEFT:     
                mVelX -= (DENTA_Y + 1);
                break;                // VELOCITY WILL RETURN 0 WHEN KEY UP
            case SDLK_RIGHT:    
                mVelX += (DENTA_Y + 1);
                break;
        }
    }

    //If a key was released
    else if( e.type == SDL_KEYUP){
        //Adjust the velocity
        if( e.key.keysym.sym == SDLK_SPACE || e.key.keysym.sym == SDLK_g || e.key.keysym.sym == SDLK_b || e.key.keysym.sym == SDLK_UP){
            vJetEngine = NOT;
        }
        
        switch( e.key.keysym.sym ){
            case SDLK_LEFT:     mVelX = 0; break;                //
            case SDLK_RIGHT:    mVelX = 0; break;                // VELOCITY
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

Ball::Ball()
{
    //Initialize the offsets
    mPosY = CEILING + 120;                       // MAGIC
    mPosX = (SCREEN_WIDTH - BALL_WIDTH)/2;

    //Initialize the velocity
    mVelX = 0;
    vJetEngine = 0;
    
    energy = 0;
    
    currentState = DOWN;
}



/*
                    I TRY TO RUN THIS CODE. BUT WE HAVE ISSUE WITH GLOBAL VARIABLE DECLARE IN main.cpp
                    WE USE gDotTexture.render(x, y, NULL) instead
 */

//void Dot::render(){
//    //Show the dot
//    gDotTexture.render( mPosX, mPosY );
//}

#endif /* ball_h */
