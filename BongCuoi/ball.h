#ifndef ball_h
#define ball_h

class Dot
{
    public:
        static const int DOT_WIDTH = 20;
        static const int DOT_HEIGHT = 20;
    
        const int FIRE_WIDTH = 36;
        const int FIRE_HEIGHT = 46;
    
        enum fireState{
            UP = 0,
            DOWN = 0,
            UP_LEFT = 6,
            UP_RIGHT = -6,
            DOWN_LEFT = 30,
            DOWN_RIGHT = -30
        };
        
        int currentState;
    
        //The X and Y offsets of the dot
        int mPosX, mPosY;
    
        int mVelX;

        Dot();

        // CHANGE VELOCITY DEPEND TYPED KEY
        void handleEvent( SDL_Event& e );

        //Moves the dot
        void move(const int& up, const int& DENTA_Y);

        //Shows the dot on the screen
        void render();
    
        int getX();
    
        int getY();
};

void Dot::handleEvent( SDL_Event& e ){
    if( e.type == SDL_KEYDOWN && e.key.repeat == 0 ){
        //Adjust the velocity
        switch( e.key.keysym.sym ){
            case SDLK_LEFT:     mVelX -= DENTA_X; break;                // VELOCITY WILL RETURN 0 WHEN KEY UP
            case SDLK_RIGHT:    mVelX += DENTA_X; break;
        }
    }
    
    //If a key was released
    else if( e.type == SDL_KEYUP){
        //Adjust the velocity
        switch( e.key.keysym.sym ){
            case SDLK_LEFT:     mVelX = 0; break;
            case SDLK_RIGHT:    mVelX = 0; break;
        }
    }
}

void Dot::move(const int& upANDdyn, const int& DENTA_Y){
    
    if(upANDdyn == -20){             // NOT COLLIDE
        mPosY += DENTA_Y;
        mPosX += mVelX;
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
    
    
    //If the dot went too far to the left or right
    if( ( mPosX < lPIVOT ) || ( mPosX + DOT_WIDTH > rPIVOT ) ){         // BOUND WHEN COLLIDE WALL
        mPosX -= mVelX;
    }
}

Dot::Dot()
{
    //Initialize the offsets
    mPosY = CEILING + 120;                       // MAGIC
    mPosX = (SCREEN_WIDTH - DOT_WIDTH)/2;

    //Initialize the velocity
    mVelX = 0;
    
//    currentState = DOWN;
}

int Dot::getX(){
    return mPosX;
}

int Dot::getY(){
    return mPosY;
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
