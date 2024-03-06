#ifndef ball_h
#define ball_h

class Dot
{
    public:
        //The dimensions of the dot
        static const int DOT_WIDTH = 20;
        static const int DOT_HEIGHT = 20;

        //Maximum axis velocity of the dot
        static const int DOT_VEL = 2;
        static const int DENTA_Y = 3;

        //Initializes the variables
        Dot();

        //Takes key presses and adjusts the dot's velocity
        void handleEvent( SDL_Event& e );

        //Moves the dot
        void move();

        //Shows the dot on the screen
        void render();
    
        int getX();
    
        int getY();

    private:
        //The X and Y offsets of the dot
        int mPosX, mPosY;

        //The velocity of the dot
        int mVelX;
        int mVelY;
};

void Dot::handleEvent( SDL_Event& e ){
    //If a key was pressed
    if( e.type == SDL_KEYDOWN && e.key.repeat == 0 ){
        //Adjust the velocity
        switch( e.key.keysym.sym ){
//            case SDLK_UP:       mVelY -= DOT_VEL; break;
//            case SDLK_DOWN:     mVelY += DOT_VEL; break;
            case SDLK_LEFT:     mVelX -= DOT_VEL; break;
            case SDLK_RIGHT:    mVelX += DOT_VEL; break;
        }
    }
    
    //If a key was released
    else if( e.type == SDL_KEYUP){
        //Adjust the velocity
        switch( e.key.keysym.sym ){
//            case SDLK_UP:       mVelY += DOT_VEL; break;
//            case SDLK_DOWN:     mVelY -= DOT_VEL; break;
            case SDLK_LEFT:     mVelX += DOT_VEL; break;
            case SDLK_RIGHT:    mVelX -= DOT_VEL; break;
        }
    }
}

void Dot::move()
{
    //Move the dot left or right
    mPosX += mVelX;

    //If the dot went too far to the left or right
    if( ( mPosX < lPIVOT ) || ( mPosX + DOT_WIDTH > rPIVOT ) ){
        //Move back
        mPosX -= 3*mVelX;
    }

    //Move the dot up or down
    mPosY += DENTA_Y;

    //If the dot went too far up or down
    if(mPosY + DOT_HEIGHT > FLOOR)
    {
        //Move back
        mPosY = CEILING + 50;
        mPosX = (SCREEN_WIDTH - DOT_WIDTH)/2;
    }
}

Dot::Dot()
{
    //Initialize the offsets
    mPosY = CEILING + 50;
    mPosX = (SCREEN_WIDTH - DOT_WIDTH)/2;

    //Initialize the velocity
    mVelX = 0;
    mVelY = 0;
}

int Dot::getX(){
    return mPosX;
}

int Dot::getY(){
    return mPosY;
}

//void Dot::render(){
//    //Show the dot
//    gDotTexture.render( mPosX, mPosY );
//}

#endif /* ball_h */
