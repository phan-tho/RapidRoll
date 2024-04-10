//
// BY PHAN THO
// CREATE 09/03/2024

#ifndef Pause_h
#define Pause_h

#include "def.h"

class Pause {
public:
    Pause();
    
    bool mCurrentState[TOTAL_BUTTON_IN_GAME];
    
    bool isAuto;
    
    SDL_Point mCurrentPos[TOTAL_BUTTON_IN_GAME];
    
    void render();
    
    bool handleTapped(SDL_Event* e);
    
    void close();
protected:
    bool isInButton(const int& button);
    
private:
    const int BUTTON_WIDTH = 140;
    const int BUTTON_HEIGHT = 40;
    
    const int BUTTON_AUTO_HEIGHT = 47;
    const int BUTTON_AUTO_WIDTH  = 140;
    
    const int POS_X_BT_AUTO  = 144;
    const int POS_Y_BT_AUTO  = 685;
    
    int DENTA_Y = 1;
    
    const SDL_Point CORRECTLY_POS[TOTAL_BUTTON_IN_GAME] = { { 275, 16 },
                                                            { 275, 16 },
                                                            { 275, 61},
                                                            { 275, 108} };
    
    LTexture mButtonControlGame[TOTAL_BUTTON_IN_GAME];
    LTexture mButtonAuto[2];
    
    int isZoomOut;
    int x, y;
};

bool Pause::handleTapped(SDL_Event* e){
    // pause when type p
    if(e->type == SDL_KEYDOWN && e->key.keysym.sym == SDLK_p){
        mCurrentState[PAUSE] ^= 1;
        return true;
    }
    
    if(e->type != SDL_MOUSEBUTTONDOWN && e->type != SDL_MOUSEMOTION){
        return false;
    }
    
    //Get mouse pos
//    int x, y;
    SDL_GetMouseState( &x, &y );
        
    if( mCurrentState[PAUSE] ){                 // PROCESSING 3 BUTTON
        // IN BUTTON PLAY
        if ( isInButton(PLAY) ){
            if( e->type == SDL_MOUSEBUTTONDOWN ){
                mCurrentState[PAUSE] = false;
                isZoomOut = PAUSE;
                return true;
            }
            else{
                isZoomOut = PLAY;
            }
        // IN BUTTON REPLAY
        }
        else if ( isInButton(REPLAY) ){
            if( e->type == SDL_MOUSEBUTTONDOWN ){
                mCurrentState[REPLAY] = true;
                return true;
            }
            else{
                isZoomOut = REPLAY;
            }
        }
        // IN BUTTON EXIT
        else if( isInButton(EXIT) ){
            if( e->type == SDL_MOUSEBUTTONDOWN ){
                mCurrentState[EXIT] = true;
                return true;
            }
            else{
                isZoomOut = EXIT;
            }
        }
        else    isZoomOut = NONE;
    }
    else{           // PAUSE IS FALSE
        if ( isInButton(PAUSE) ){
            if( e->type == SDL_MOUSEBUTTONDOWN ){
                mCurrentState[PAUSE] = true;
                isZoomOut = PLAY;
                return true;
            }
            else{
                isZoomOut = PAUSE;
            }
        }
        else        isZoomOut = NONE;
    }
    
    if ( x >= POS_X_BT_AUTO && x <= POS_X_BT_AUTO + BUTTON_AUTO_WIDTH && y >= POS_Y_BT_AUTO && y <= POS_Y_BT_AUTO + BUTTON_AUTO_HEIGHT ){
        if(e->type == SDL_MOUSEBUTTONDOWN){
            isAuto ^= 1;
            return true;
        }
        isZoomOut = AUTO;
    }
    return false;
}

bool Pause::isInButton(const int& button){
    return (x >= CORRECTLY_POS[button].x &&
            x <=  CORRECTLY_POS[button].x + BUTTON_WIDTH &&
            y >= CORRECTLY_POS[button].y &&
            y <= CORRECTLY_POS[button].y + BUTTON_HEIGHT);
}

Pause::Pause(){
    for(int i = PAUSE; i <= EXIT; i++){
        mCurrentState[i] = false;
        
        mCurrentPos[i]  = { 275, 16 };
    }
    isZoomOut = NONE;
    isAuto = 0;
    
    mButtonControlGame[PAUSE].loadFromFile("Pause.png");
    mButtonControlGame[PLAY].loadFromFile("Play.png");
    mButtonControlGame[REPLAY].loadFromFile("Replay.png");
    mButtonControlGame[EXIT].loadFromFile("Exit.png");
    
    mButtonAuto[0].loadFromFile("OffAuto.png");
    mButtonAuto[1].loadFromFile("OnAuto.png");
}

void Pause::close(){
    mButtonControlGame[0].freeFire();
    mButtonControlGame[1].freeFire();
    mButtonControlGame[2].freeFire();
    mButtonControlGame[3].freeFire();
    
    mButtonAuto[0].freeFire();
    mButtonAuto[1].freeFire();
}

void Pause::render(){
    if( mCurrentState[PAUSE] ){             // PAUSE IS PRESSED
        mCurrentPos[EXIT].y   += 2*(mCurrentPos[EXIT].y < CORRECTLY_POS[EXIT].y ? DENTA_Y : 0);
        mCurrentPos[REPLAY].y += (mCurrentPos[REPLAY].y < CORRECTLY_POS[REPLAY].y ? DENTA_Y : 0);
        mCurrentPos[PLAY].y   += (mCurrentPos[PLAY].y < CORRECTLY_POS[PLAY].y ? DENTA_Y : 0);
        
        if(mCurrentPos[EXIT].y == CORRECTLY_POS[EXIT].y){
            for(int i = EXIT; i >= PLAY; i--){
                if(i == isZoomOut){
                    mButtonControlGame[i].renderCustomSize(CORRECTLY_POS[i].x - BUTTON_WIDTH/10, CORRECTLY_POS[i].y - BUTTON_HEIGHT/10, 1.2);
                }
                else{
                    mButtonControlGame[i].render(CORRECTLY_POS[i].x, CORRECTLY_POS[i].y, NULL);
                }
            }
            // FOR EXIT REPLAY PLAY
        }
        else{
            mButtonControlGame[EXIT].render(mCurrentPos[EXIT].x, mCurrentPos[EXIT].y, NULL);
            mButtonControlGame[REPLAY].render(mCurrentPos[REPLAY].x, mCurrentPos[REPLAY].y, NULL);
            mButtonControlGame[PLAY].render(mCurrentPos[PLAY].x, mCurrentPos[PLAY].y, NULL);
        }
    }
    else{
        if(mCurrentPos[EXIT].y > CORRECTLY_POS[PAUSE].y){
            mCurrentPos[EXIT].y -= 2*DENTA_Y;
            mButtonControlGame[EXIT].render(mCurrentPos[EXIT].x, mCurrentPos[EXIT].y, NULL);
        }
        if(mCurrentPos[REPLAY].y > CORRECTLY_POS[PAUSE].y){
            mCurrentPos[REPLAY].y -= DENTA_Y;
            mButtonControlGame[REPLAY].render(mCurrentPos[REPLAY].x, mCurrentPos[REPLAY].y, NULL);
        }
        
        if(isZoomOut == PAUSE){
            mButtonControlGame[PAUSE].renderCustomSize(CORRECTLY_POS[PAUSE].x - BUTTON_WIDTH/10, CORRECTLY_POS[PAUSE].y - BUTTON_HEIGHT/10, 1.2);
        }
        else{
            mButtonControlGame[PAUSE].render(CORRECTLY_POS[PAUSE].x, CORRECTLY_POS[PAUSE].y, NULL);
        }
    }
    
    // RENDER BUTTON AUTO
    if(isZoomOut == AUTO){
        mButtonAuto[isAuto].renderCustomSize(POS_X_BT_AUTO - BUTTON_AUTO_WIDTH/10, POS_Y_BT_AUTO - BUTTON_AUTO_HEIGHT/10, 1.2);
    }
    else{
        mButtonAuto[isAuto].render(POS_X_BT_AUTO, POS_Y_BT_AUTO, NULL);
    }
}

#endif /* Pause_h */
