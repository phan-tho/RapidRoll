//  Created by Phan Tho on 27/03/2024.

#ifndef ballWithGun_h
#define ballWithGun_h

#include "ball.h"
#include "list"
#include "Bullet.h"
#include <cmath>

class BallWithGun : public Ball{
public:
    BallWithGun();
    
    void renderBall();
    
    void renderGun(const bool& isPause = 0);
    
    void genBullet(SDL_Event* e);
    
//    void handleEvent( const SDL_Event& e, const int& DENTA_Y, const SDL_Keycode& moveUp, const SDL_Keycode& moveLeft, const SDL_Keycode& moveRight );
    
    void handleBullet(const std::deque<Trap>& Traps, const std::deque<Block>& Blocks);
    
    void close();
    // clear LTexture
    
private:
    const int GUN_WIDTH = 41;
    const int GUN_HEIGHT = 16;
    
    double angle;
    
    std::list<Bullet> Bullets;
    
    LTexture BallGunTexture;
    LTexture GunTextTure;
    
    int PosMouseX, PosMouseY;
    SDL_Point centre;
};

void BallWithGun::genBullet(SDL_Event* e){
    if(e->type == SDL_MOUSEBUTTONDOWN){
        Bullet bullet(mPosX + GUN_WIDTH*cos(angle*M_PI/180), mPosY + GUN_WIDTH*sin(angle*M_PI/180), angle);
        Bullets.push_back(bullet);
    }
}

//void BallWithGun::handleEvent( const SDL_Event& e, const int& DENTA_Y, const SDL_Keycode& moveUp, const SDL_Keycode& moveLeft, const SDL_Keycode& moveRight ){
//    Ball::handleEvent(e, DENTA_Y, moveUp, moveLeft, moveRight);
//    
//    if(e.type == SDL_MOUSEBUTTONDOWN){
//        Bullet bullet(mPosX, mPosY, angle);
//        Bullets.push_back(bullet);
//    }
//}

void BallWithGun::handleBullet(const std::deque<Trap>& Traps, const std::deque<Block>& Blocks){
    if(Bullets.empty())     return;
    
    auto it = Bullets.begin();
    
    while(it != Bullets.end()) {
        // MOVE
        it->move();
        
        int state = it->checkCollide(Traps, Blocks);
        
        if( state == it->NOT_COLLIDE ){
            (*it).render();
            it++;
        }
        else if( state == it->COLLIDE ){
            it = Bullets.erase(it);
        }
    }
}

void BallWithGun::renderGun(const bool& isPause){
    if(isPause == false){
        SDL_GetMouseState(&PosMouseX, &PosMouseY);
    }
    
    int x = PosMouseX - mPosX - BALL_WIDTH/2;
    int y = PosMouseY - mPosY - BALL_HEIGHT/2;
    
    angle = std::atan2(y, x)*180/M_PI;
    x = mPosX + BALL_WIDTH/2;
    y = mPosY;
    centre = { 0, BALL_HEIGHT/2 };
    GunTextTure.renderFlip(x, y, NULL, angle, &centre, SDL_FLIP_NONE);
}

void BallWithGun::renderBall(){
    BallGunTexture.render(mPosX - 5, mPosY - 10, NULL);
}

void BallWithGun::close(){
//    Ball::close();
    
    BallGunTexture.freeFire();
    GunTextTure.freeFire();
    Bullet::BulletTexture.freeFire();
}

BallWithGun::BallWithGun(){
    Ball::close();
    
    angle = 0;
    
    PosMouseX = 0;
    PosMouseY = 0;
    centre = {0, 0};
    
    BallGunTexture.loadFromFile("Hitle.png");
    GunTextTure.loadFromFile("SortGun.png");
    
    Bullet::BulletTexture.loadFromFile("Bullet.png");
}

#endif /* ballWithGun_h */
