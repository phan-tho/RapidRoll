#ifndef ball_h
#define ball_h

class Dot
{
    public:
        //The dimensions of the dot
        static const int DOT_WIDTH = 20;
        static const int DOT_HEIGHT = 20;
    
//        int veloY;
    
        //The X and Y offsets of the dot
        int mPosX, mPosY;
    
        int mVelX;

        //Maximum axis velocity of the dot
//        static const int DENTA_X = 3;
//        static const int DENTA_Y = 2;

        //Initializes the variables
        Dot();

        //Takes key presses and adjusts the dot's velocity
        void handleEvent( SDL_Event& e );

        //Moves the dot
        void move(const bool& up);

        //Shows the dot on the screen
        void render();
    
        int getX();
    
        int getY();

    private:
//        //The X and Y offsets of the dot
//        int mPosX, mPosY;

        //The velocity of the dot
//        int mVelX;
};

void Dot::handleEvent( SDL_Event& e ){
    //If a key was pressed
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

void Dot::move(const bool& up){
    //Move the dot left or right
    mPosX += mVelX;
    mPosY += (up ? (-DENTA_Y) : DENTA_Y);

    //If the dot went too far to the left or right
    if( ( mPosX < lPIVOT ) || ( mPosX + DOT_WIDTH > rPIVOT ) ){         // BOUND WHEN COLLIDE WALL
        mPosX -= 3*mVelX;
    }

    
    //       WHEN BALL OUT OF BOARD. THIS CASE IS TEMPORARY. AND WE NEED TO END LIFE OF BALL
     
    //If the dot went too far up or down
    if(mPosY + DOT_HEIGHT > FLOOR){
        mPosY = CEILING + 50;
        mPosX = (SCREEN_WIDTH - DOT_WIDTH)/2;
    }
}

Dot::Dot()
{
    //Initialize the offsets
    mPosY = CEILING + 60;                       // MAGIC
    mPosX = (SCREEN_WIDTH - DOT_WIDTH)/2;

    //Initialize the velocity
    mVelX = 0;
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
