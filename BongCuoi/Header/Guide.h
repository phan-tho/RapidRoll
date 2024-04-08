//  Created by Phan Tho on 08/04/2024.

#ifndef Guide_h
#define Guide_h

#include "LTexture.h"

class Guide{
    int cnt;
    const int TIME_APPEAR = 360;
    
    const int ARROW_HEIGHT = 15;
    const int ARROW_WIDTH  = 15;
    
    const int MOUSE_HEIGHT = 40;
    const int MOUSE_WIDTH  = 34;
    
    const int DISTANCE = 10;
    
    enum DIRECT{
        UP,
        LEFT,
        RIGHT,
        FIRE,
        TOTAL
    };
    
    const SDL_Color textColor = {255, 155, 0};
    
    LTexture mArrow;
    LTexture mMouse;
    LTexture mTextTexture[TOTAL];
    
public:
    Guide();
    
    ~Guide();
    
    void guide(const Ball& ball, const bool& haveGun, const std::string& up, const std::string& left, const std::string& right);
};

void Guide::guide(const Ball &ball, const bool &haveGun, const std::string &up, const std::string &left, const std::string &right){
    if(cnt++ >= TIME_APPEAR)        return;
//    cnt = 1;
    mArrow.render(ball.mPosX + (ball.BALL_WIDTH - ARROW_WIDTH)/2, ball.mPosY - ARROW_HEIGHT - DISTANCE, NULL);     // up
    mArrow.renderFlip(ball.mPosX - DISTANCE - ARROW_WIDTH, ball.mPosY + (ball.BALL_HEIGHT - ARROW_HEIGHT)/2, NULL, -90, NULL, SDL_FLIP_NONE);     // left
    mArrow.renderFlip(ball.mPosX + ball.BALL_WIDTH + DISTANCE, ball.mPosY + (ball.BALL_HEIGHT - ARROW_HEIGHT)/2, NULL, 90, NULL, SDL_FLIP_NONE);  // right
    
//    std::string key = up;
    if(cnt == 1){
        mTextTexture[UP].loadFromRenderedText(up, textColor);
        mTextTexture[LEFT].loadFromRenderedText(left, textColor);
        mTextTexture[RIGHT].loadFromRenderedText(right, textColor);
        mTextTexture[FIRE].loadFromRenderedText("SHOOT", textColor);
    }
    mTextTexture[UP].renderCustomSize(ball.mPosX + ball.BALL_WIDTH, ball.mPosY - ARROW_HEIGHT - DISTANCE - 10, 0.6);        // up
    mTextTexture[LEFT].renderCustomSize(ball.mPosX - DISTANCE - ARROW_WIDTH, ball.mPosY - 15, 0.6);   // left
    mTextTexture[RIGHT].renderCustomSize(ball.mPosX + ball.BALL_WIDTH + DISTANCE, ball.mPosY - 15, 0.6);     // right
    
    if(haveGun){
        mMouse.render(ball.mPosX + 40, ball.mPosY - 20, NULL);
        mTextTexture[FIRE].renderCustomSize(ball.mPosX + 45, ball.mPosY - 30, 0.4);
    }
}

Guide::~Guide(){
    mArrow.freeFire();
    mMouse.freeFire();
    for(int i = 0; i < TOTAL; i++)    mTextTexture[i].freeFire();
}

Guide::Guide(){
    cnt = 0;
    mArrow.loadFromFile("Arrow.png");
    mMouse.loadFromFile("Mouse.png");
}

#endif /* Guide_h */
