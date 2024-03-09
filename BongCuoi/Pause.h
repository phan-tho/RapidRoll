//
// BY PHAN THO
// CREATE 09/03/2024
//
#ifndef Pause_h
#define Pause_h

class Pause {
public:
    
    Pause();
    
    bool mCurrentState[TOTAL_BUTTON_IN_GAME];
    
    SDL_Point mCurrentPos[TOTAL_BUTTON_IN_GAME];
    
    void render();
    
    void handleEvent(SDL_Event* e);
private:
    const int BUTTON_WIDTH = 140;
    const int BUTTON_HEIGHT = 40;
    
    int DENTA_Y = 1;
    
    const SDL_Point CORRECTLY_POS[TOTAL_BUTTON_IN_GAME] = { { 275, 16 },
                                                            { 275, 16 },
                                                            { 275, 61},
                                                            { 275, 108} };
};

void Pause::handleEvent(SDL_Event* e){
    if(e->type == SDL_MOUSEBUTTONDOWN){
        //Get mouse position
        int x, y;
        SDL_GetMouseState( &x, &y );
        
        if( mCurrentState[PAUSE] ){
            if ( x >= CORRECTLY_POS[PLAY].x && x <=  CORRECTLY_POS[PLAY].x + BUTTON_WIDTH && y >= CORRECTLY_POS[PLAY].y && y <= CORRECTLY_POS[PLAY].y + BUTTON_HEIGHT){
                mCurrentState[PAUSE] = false;
//                mCurrentPos[PAUSE]  = { 275, 16 };
//                mCurrentPos[PLAY]   = { 275, 16 };
//                mCurrentPos[REPLAY] = { 275, 18 };
//                mCurrentPos[EXIT]   = { 275, 20 };
            }
            else if ( x >= CORRECTLY_POS[REPLAY].x && x <=  CORRECTLY_POS[REPLAY].x + BUTTON_WIDTH && y >= CORRECTLY_POS[REPLAY].y && y <= CORRECTLY_POS[REPLAY].y + BUTTON_HEIGHT){
                mCurrentState[REPLAY] = true;
            }
            else if( x >= CORRECTLY_POS[EXIT].x && x <=  CORRECTLY_POS[EXIT].x + BUTTON_WIDTH && y >= CORRECTLY_POS[EXIT].y && y <= CORRECTLY_POS[EXIT].y+ BUTTON_HEIGHT){
                mCurrentState[EXIT] = true;
            }
        }
        else{
            mCurrentState[PAUSE] = ( x >= CORRECTLY_POS[PAUSE].x && x <=  CORRECTLY_POS[PAUSE].x + BUTTON_WIDTH && y >= CORRECTLY_POS[PAUSE].y && y <= BUTTON_HEIGHT);
        }
    }
}

Pause::Pause(){
    mCurrentState[0] = false;
    mCurrentState[1] = false;
    mCurrentState[2] = false;
    mCurrentState[3] = false;
    
    mCurrentPos[PAUSE]  = { 275, 16 };
    mCurrentPos[PLAY]   = { 275, 16 };
    mCurrentPos[REPLAY] = { 275, 16 };
    mCurrentPos[EXIT]   = { 275, 26 };
}

void Pause::render(){
    if( mCurrentState[PAUSE] ){
        gButtonControlGame[EXIT].render(mCurrentPos[EXIT].x, mCurrentPos[EXIT].y, NULL);
        gButtonControlGame[REPLAY].render(mCurrentPos[REPLAY].x, mCurrentPos[REPLAY].y, NULL);
        gButtonControlGame[PLAY].render(mCurrentPos[PLAY].x, mCurrentPos[PLAY].y, NULL);
        
        mCurrentPos[EXIT].y   += 2*(mCurrentPos[EXIT].y < CORRECTLY_POS[EXIT].y ? DENTA_Y : 0);
        mCurrentPos[REPLAY].y += (mCurrentPos[REPLAY].y < CORRECTLY_POS[REPLAY].y ? DENTA_Y : 0);
        mCurrentPos[PLAY].y   += (mCurrentPos[PLAY].y < CORRECTLY_POS[PLAY].y ? DENTA_Y : 0);
    }
    else{
        if(mCurrentPos[EXIT].y > CORRECTLY_POS[PAUSE].y){
            mCurrentPos[EXIT].y -= 2*DENTA_Y;
            gButtonControlGame[EXIT].render(mCurrentPos[EXIT].x, mCurrentPos[EXIT].y, NULL);
        }
        if(mCurrentPos[REPLAY].y > CORRECTLY_POS[PAUSE].y){
            mCurrentPos[REPLAY].y -= DENTA_Y;
            gButtonControlGame[REPLAY].render(mCurrentPos[REPLAY].x, mCurrentPos[REPLAY].y, NULL);
        }
        
        gButtonControlGame[PAUSE].render(CORRECTLY_POS[PAUSE].x, CORRECTLY_POS[PAUSE].y, NULL);
    }
}

#endif /* Pause_h */
