//
// BY PHAN THO
// CREATE 09/03/2024
//
#ifndef Pause_h
#define Pause_h

#include "def.h"

//#include "defLTexture.h"

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
    
    int isZoomOut;
};

void Pause::handleEvent(SDL_Event* e){
    // pause when type p
    if(e->type == SDL_KEYDOWN && e->key.keysym.sym == SDLK_p){
        mCurrentState[PAUSE] ^= 1;
    }
    
    if(e->type != SDL_MOUSEBUTTONDOWN && e->type != SDL_MOUSEMOTION){
        return;
    }
    
    //Get mouse pos
    int x, y;
    SDL_GetMouseState( &x, &y );
        
    if( mCurrentState[PAUSE] ){                 // PROCESSING 3 BUTTON
        // IN BUTTON PLAY
        if ( x >= CORRECTLY_POS[PLAY].x && x <=  CORRECTLY_POS[PLAY].x + BUTTON_WIDTH && y >= CORRECTLY_POS[PLAY].y && y <= CORRECTLY_POS[PLAY].y + BUTTON_HEIGHT){
            if( e->type == SDL_MOUSEBUTTONDOWN ){
                mCurrentState[PAUSE] = false;
                isZoomOut = PAUSE;
            }
            else{
                isZoomOut = PLAY;
            }
        // IN BUTTON REPLAY
        }
        else if ( x >= CORRECTLY_POS[REPLAY].x && x <=  CORRECTLY_POS[REPLAY].x + BUTTON_WIDTH && y >= CORRECTLY_POS[REPLAY].y && y <= CORRECTLY_POS[REPLAY].y + BUTTON_HEIGHT){
            if( e->type == SDL_MOUSEBUTTONDOWN ){
                mCurrentState[REPLAY] = true;
            }
            else{
                isZoomOut = REPLAY;
            }
        }
        // IN BUTTON EXIT
        else if( x >= CORRECTLY_POS[EXIT].x && x <=  CORRECTLY_POS[EXIT].x + BUTTON_WIDTH && y >= CORRECTLY_POS[EXIT].y && y <= CORRECTLY_POS[EXIT].y+ BUTTON_HEIGHT){
            if( e->type == SDL_MOUSEBUTTONDOWN ){
                mCurrentState[EXIT] = true;
            }
            else{
                isZoomOut = EXIT;
            }
        }
        else    isZoomOut = NONE;
        
    }
    else{           // PAUSE IS FALSE
        if ( x >= CORRECTLY_POS[PAUSE].x && x <=  CORRECTLY_POS[PAUSE].x + BUTTON_WIDTH && y >= CORRECTLY_POS[PAUSE].y && y <= BUTTON_HEIGHT){
            if( e->type == SDL_MOUSEBUTTONDOWN ){
                mCurrentState[PAUSE] = true;
                isZoomOut = PLAY;
            }
            else{
                isZoomOut = PAUSE;
            }
        }
        else        isZoomOut = NONE;
        
    }
}

Pause::Pause(){
    for(int i = PAUSE; i <= EXIT; i++){
        mCurrentState[i] = false;
        
        mCurrentPos[i]  = { 275, 16 };
    }
    isZoomOut = NONE;
}

void Pause::render(){
    if( mCurrentState[PAUSE] ){             // PAUSE IS PRESSED
//        gButtonControlGame[EXIT].render(mCurrentPos[EXIT].x, mCurrentPos[EXIT].y, NULL);
//        gButtonControlGame[REPLAY].render(mCurrentPos[REPLAY].x, mCurrentPos[REPLAY].y, NULL);
//        gButtonControlGame[PLAY].render(mCurrentPos[PLAY].x, mCurrentPos[PLAY].y, NULL);
        
        mCurrentPos[EXIT].y   += 2*(mCurrentPos[EXIT].y < CORRECTLY_POS[EXIT].y ? DENTA_Y : 0);
        mCurrentPos[REPLAY].y += (mCurrentPos[REPLAY].y < CORRECTLY_POS[REPLAY].y ? DENTA_Y : 0);
        mCurrentPos[PLAY].y   += (mCurrentPos[PLAY].y < CORRECTLY_POS[PLAY].y ? DENTA_Y : 0);
        
        if(mCurrentPos[EXIT].y == CORRECTLY_POS[EXIT].y){
            for(int i = EXIT; i >= PLAY; i--){
                if(i == isZoomOut){
                    gButtonControlGame[i].renderCustomSize(CORRECTLY_POS[i].x - BUTTON_WIDTH/10, CORRECTLY_POS[i].y - BUTTON_HEIGHT/10, 1.2);
                }
                else{
                    gButtonControlGame[i].render(CORRECTLY_POS[i].x, CORRECTLY_POS[i].y, NULL);
                }
            }
            // FOR EXIT REPLAY PLAY
        }
        else{
            gButtonControlGame[EXIT].render(mCurrentPos[EXIT].x, mCurrentPos[EXIT].y, NULL);
            gButtonControlGame[REPLAY].render(mCurrentPos[REPLAY].x, mCurrentPos[REPLAY].y, NULL);
            gButtonControlGame[PLAY].render(mCurrentPos[PLAY].x, mCurrentPos[PLAY].y, NULL);
        }
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
        
        if(isZoomOut == PAUSE){
            gButtonControlGame[PAUSE].renderCustomSize(CORRECTLY_POS[PAUSE].x - BUTTON_WIDTH/10, CORRECTLY_POS[PAUSE].y - BUTTON_HEIGHT/10, 1.2);
        }
        else{
            gButtonControlGame[PAUSE].render(CORRECTLY_POS[PAUSE].x, CORRECTLY_POS[PAUSE].y, NULL);
        }
    }
}

#endif /* Pause_h */
