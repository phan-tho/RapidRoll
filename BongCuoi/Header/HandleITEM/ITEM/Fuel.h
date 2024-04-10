#ifndef Fuel_h
#define Fuel_h

class Fuel{
public:
    const int FUEL_WIDTH = 32;
    const int FUEL_HEIGHT = 40;
    
    int PosX, PosY;

    bool isEaten;

    int moveToLeft;
    
    Fuel();
    
    void reset();
    
    void move(const int& DENTA_Y);
    

    void assignPos(const int x, const int y);
};

Fuel::Fuel(){
    PosX = 0;
    PosY = 0;
    moveToLeft = 0;
    isEaten = false;
}

void Fuel::reset(){
    PosX = 0;
    PosY = 0;
    moveToLeft = 0;
}

void Fuel::move(const int& DENTA_Y){
    PosY -= DENTA_Y;
    
    PosX += moveToLeft;             // PASS VELOCITY OF BLOCK TO HEART
                                    // HEART MOVE INDEPEND WITH BLOCK BELOW ITS
    
    if( ( PosX <= 57 ) || ( PosX >= 341 ) ){         // BOUND WHEN COLLIDE WALL
        PosX -= moveToLeft;                             // MAGIC NUMBERS
        moveToLeft = -moveToLeft;
    }
}


void Fuel::assignPos(const int x, const int y){
    PosX = x;
    PosY = y;
}


#endif /* Fuel_h */
