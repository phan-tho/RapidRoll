//  Created by Phan Tho on 27/03/2024.

#ifndef ballWithGun_h
#define ballWithGun_h

#include "ball.h"
#include "list"
#include "Bullet.h"
#include <cmath>

class BallWithGun : public Ball{
public:
    BallWithGun(std::string path);
    
    // GET COORDINATE OF MOUSE
    // DETERMINE DIRECTON OF GUN AND RENDER
    void renderGun(const bool& isPause = 0);
    
    // GEN LOADED BULLET WHEN MOUSE IS CLIKED
    void genBullet(SDL_Event* e);
    
    // STORE BULLET IN LIST
    // ERASE WHEN COLLIDE WITH BALL, WALL, BLOCK, TRAP
    // RETURN TRUE WHEN BALL IS KILLED
    bool handleBullet(Ball& ball, const std::deque<Trap>& Traps, const std::deque<Block>& Blocks, int& score);
    
    void close();
    // clear LTexture
private:
    const int GUN_WIDTH = 41;
    const int GUN_HEIGHT = 16;
    
    const int HITLE_WIDTH  = 30;
    const int HITLE_HEIGHT = 33;
    
    const int TIME_LOAD_BULLET = 90;
    int waitLoad;
    
    bool mousePressed;
    
    double angle;
    
    std::list<Bullet> Bullets;

    LTexture GunTextTure;
    
    int PosMouseX, PosMouseY;
    SDL_Point centre;
    
    Mix_Chunk* mMusicWhenShoot;
};

// GEN BULLET WHEN TAPPED BUTTON AND BUULLET IS LOADED
void BallWithGun::genBullet(SDL_Event* e){
    if(e->type == SDL_MOUSEBUTTONDOWN)      mousePressed = true;
    else if (e->type == SDL_MOUSEBUTTONUP)  mousePressed = false;
    if(mousePressed && !waitLoad){
        Bullet bullet(mPosX + GUN_WIDTH*cos(angle*M_PI/180), mPosY + GUN_WIDTH*sin(angle*M_PI/180), angle);
        Bullets.push_back(bullet);
        waitLoad = TIME_LOAD_BULLET;
        
        Mix_PlayChannel(2, mMusicWhenShoot, 0);
    }
}

// CHECK COLLIDE WITH BLOCK, TRAP, WALL BALL
// RETURN TRUE WHEN BALL IS KILLED
bool BallWithGun::handleBullet(Ball& ball, const std::deque<Trap>& Traps, const std::deque<Block>& Blocks, int& score){
    if(Bullets.empty())     return false;
    
    auto it = Bullets.begin();
    
    while(it != Bullets.end()) {
        // MOVE
        it->move();
        
        // COLLIDE WITH BLOCK OR TRAP OR WALL
        int state = it->checkCollide(Traps, Blocks);
        
        if( state == it->NOT_COLLIDE ){
            (*it).render();
            it++;
        }
        else if( state == it->COLLIDE || it->isCollideBall(ball) ){
            if(it->isCollideBall(ball)){                // ball is killed
                ball.reset();
                life++;
                score++;
                // music when die
                return true;
                
                ball.mPosY = CEILING + 120;                       // MAGIC
                ball.mPosX = (SCREEN_WIDTH - BALL_WIDTH)/2 + 50;
            }
            it = Bullets.erase(it);
        }
    }
    return false;
}


void BallWithGun::renderGun(const bool& isPause){
    waitLoad = fmax(waitLoad - 1, 0);
    if(isPause == false){
        SDL_GetMouseState(&PosMouseX, &PosMouseY);
    }
    
    int x = PosMouseX - mPosX - BALL_WIDTH/2;
    int y = PosMouseY - mPosY - BALL_HEIGHT/2;
    
    angle = std::atan2(y, x)*180/M_PI;
    x = mPosX + BALL_WIDTH/2;
    y = mPosY + (BALL_HEIGHT - GUN_HEIGHT)/2;
    centre = { 0, GUN_HEIGHT/2 };
    GunTextTure.renderFlip(x, y, NULL, angle, &centre, SDL_FLIP_NONE);
}


void BallWithGun::close(){
    Ball::close();
    
    GunTextTure.freeFire();
    Bullet::BulletTexture.freeFire();
    
    Mix_FreeChunk(mMusicWhenShoot);
    mMusicWhenShoot = NULL;
}

BallWithGun::BallWithGun(std::string path) : Ball(path){
    angle = 0;
    
    PosMouseX = 0;
    PosMouseY = 0;
    centre = {0, 0};
    
    waitLoad = 0;
    mousePressed = false;
    
    GunTextTure.loadFromFile("SortGun.png");
    
    Bullet::BulletTexture.loadFromFile("Bullet.png");
    
    mMusicWhenShoot = Mix_LoadWAV("SingleBullet.wav");
}

#endif /* ballWithGun_h */
