//  Created by Phan Tho on 27/03/2024.

#ifndef Bullet_h
#define Bullet_h

#include <cmath>

class Bullet{
public:
    Bullet(int PosX, int PosY, double angle);
    
    void move();
    
    void render();
    
    int checkCollide(const std::deque<Trap>& Traps, const std::deque<Block>& Blocks);
    
    bool isCollideBlock(const Block& block);
    bool isCollideTrap(const Trap& trap);
    bool isCollideBall(const Ball& ball);
    
    static LTexture BulletTexture;
    
    enum State{
        NOT_COLLIDE,
        KILL_BALL,
        COLLIDE
    };
    
    float getX(){ return PosX; }
    float getY(){ return PosY; }
    double getAngle(){ return angle; }
private:
    const float VEL_BULLET = 8;
    
    const int BULLET_HEIGHT = 20;
    const int BULLET_WIDTH  = 20;
    
    float PosX, PosY;
    double angle;
};

LTexture Bullet::BulletTexture;


Bullet::Bullet(int PosX, int PosY, double angle){
    this->PosX = PosX;
    this->PosY = PosY;
    this->angle = angle;
}

void Bullet::move(){
    PosX += VEL_BULLET*cos(angle*M_PI/180);
    PosY += VEL_BULLET*sin(angle*M_PI/180);
}

void Bullet::render(){
    BulletTexture.renderFlip(int(PosX), int(PosY), NULL, angle, NULL, SDL_FLIP_NONE);
}

int Bullet::checkCollide(const std::deque<Trap>& Traps, const std::deque<Block>& Blocks){
    if(!Traps.empty()){
        for(auto trap : Traps){
            if(isCollideTrap(trap))     return COLLIDE;
        }
    }
    
    if(!Blocks.empty()){
        for(auto block : Blocks){
            if(isCollideBlock(block))   return COLLIDE;
        }
    }
    
    // COLIDE WITH WALL
    if(PosX <= lPIVOT || PosX >= rPIVOT - BULLET_WIDTH ||
       PosY <= CEILING || PosY >= FLOOR - BULLET_HEIGHT ){
        return COLLIDE;
    }
    
    return NOT_COLLIDE;
}

bool Bullet::isCollideBlock(const Block& block){
    return ( PosX >= block.PosX - BULLET_WIDTH      &&
             PosX <= block.PosX + block.BLOCK_WIDTH &&
             PosY >= block.PosY - BULLET_HEIGHT     &&
             PosY <= block.PosY + block.BLOCK_HEIGHT
             );
}

bool Bullet::isCollideTrap(const Trap& trap){
    return ( PosX >= trap.PosX - BULLET_WIDTH      &&
             PosX <= trap.PosX + trap.TRAP_WIDTH   &&
             PosY >= trap.PosY - BULLET_HEIGHT     &&
             PosY <= trap.PosY + trap.TRAP_HEIGHT
             );
}

bool Bullet::isCollideBall(const Ball &ball){
    return ( PosX >= ball.mPosX - BULLET_WIDTH     &&
             PosX <= ball.mPosX + ball.BALL_WIDTH  &&
             PosY >= ball.mPosY - BULLET_HEIGHT    &&
             PosY <= ball.mPosY + ball.BALL_HEIGHT
             );
}

#endif /* Bullet_h */
