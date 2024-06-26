//  Created by Phan Tho on 10/04/2024.

#ifndef ChooseBall_h
#define ChooseBall_h

class ChooseBall{
public:
    ChooseBall();
    
    // Handle when button left or right is clicked
    void handleEvent(SDL_Event* e);
    
    // Render left, right arrow and selected ball
    void render();
    
    void init();
    
    void close();
    
    // Get path of image
    // Load only 1 ball image when play. Save of memory
    std::string getPath();
    
private:
    const int BALL_WIDTH  = 110;
    const int BALL_HEIGHT = 100;
    
    const int FIRE_WIDTH = 144;
    const int FIRE_HEIGHT = 184;
    
    const int ARROW_HEIGHT = 50;
    const int ARROW_WIDTH  = 50;
    
    const int DISTANCE = 15;
    
    const SDL_Point POS_BALL = {240, 220};
    
    int xMouse, yMouse;
    int cnt = 0;
    
    int selectedBall;
    
    enum TypeButton{
        LEFT,
        RIGHT,
        TOTAL_BUTTON
    };
    
    enum TypeBall{
        RONALDO,
        BI_DA,
        FOOT_BALL,
        VOLLEY_BALL,
        HITLE,
        LOGO_UET,
        FIRE_BALL,
        TOTAL_BALL
    };
    
    const SDL_Point POS_ARROW[2] = { { POS_BALL.x - DISTANCE - ARROW_WIDTH, POS_BALL.y + (BALL_HEIGHT - ARROW_HEIGHT)/2 },
                                     { POS_BALL.x + BALL_WIDTH + DISTANCE, POS_BALL.y + (BALL_HEIGHT - ARROW_HEIGHT)/2 } };
    
    const std::string pathFire[6] = { "fire1.png", "fire2.png", "fire3.png", "fire4.png", "fire5.png", "fire6.png"};
    const std::string pathBall[TOTAL_BALL] = { "AnhLiem.png", "BiDa.png", "FootBall.png", "VolleyBall.png", "Hitle.png", "LogoUet.png", "Ball110.png" };
    
    bool isClicked(int button);
    
    LTexture ball[TOTAL_BALL];
    LTexture Fire[6];
    
    LTexture ButtonChange;
};


// RENDER ARROW, BALL
void ChooseBall::render(){
    // render ball in logo
    
    SDL_Point centre = { 72, 140 };
    Fire[(cnt/9)%6].renderFlip(255, 5, NULL, 15, &centre, SDL_FLIP_NONE, 2.4);
    
    // render ball
    if(selectedBall != FIRE_BALL){
        ball[selectedBall].render(POS_BALL.x, POS_BALL.y, NULL);
        cnt++;
    }
    else{
        int xFire = POS_BALL.x - (FIRE_WIDTH - BALL_WIDTH)/2;
        int yFire = POS_BALL.y - FIRE_HEIGHT + BALL_HEIGHT;
        Fire[(cnt/9)%6].renderCustomSize(xFire, yFire, 4);
        cnt = (cnt + 1)%54;
    }
    
    // render button
    ButtonChange.renderFlip(POS_ARROW[LEFT].x, POS_ARROW[LEFT].y, NULL, -90, NULL, SDL_FLIP_NONE, 10/3);
    ButtonChange.renderFlip(POS_ARROW[RIGHT].x, POS_ARROW[RIGHT].y, NULL, 90, NULL, SDL_FLIP_NONE, 10/3);
}

// BUTTON LEFT OR RIGHT IS TAPPED
// CHANGE SELECTED BALL
void ChooseBall::handleEvent(SDL_Event* e){
    if(e->type != SDL_MOUSEBUTTONDOWN){
        return;
    }
    SDL_GetMouseState(&xMouse, &yMouse);
    
    if( isClicked(LEFT) ){
        selectedBall = (selectedBall ? selectedBall - 1 : TOTAL_BALL - 1);
    }
    else if( isClicked(RIGHT) ){
        selectedBall = (selectedBall + 1)%TOTAL_BALL;
    }
}

// CHECK WHETHER COORDINATE OF MOUSE IS IN BUTTON
bool ChooseBall::isClicked(int button){
    return (xMouse >= POS_ARROW[button].x &&
            xMouse <= POS_ARROW[button].x + ARROW_WIDTH &&
            yMouse >= POS_ARROW[button].y &&
            yMouse <= POS_ARROW[button].y + ARROW_HEIGHT );
}

std::string ChooseBall::getPath() {
    return 's' + pathBall[selectedBall];
}

void ChooseBall::close(){
    for(int i = 0; i < 6; i++)      Fire[i].freeFire();
    for(int i = 0; i < TOTAL_BALL; i++)     ball[i].freeFire();
    ButtonChange.freeFire();
}

void ChooseBall::init(){
    for(int i = 0; i < 6; i++)      Fire[i].loadFromFile(pathFire[i]);
    for(int i = 0; i < TOTAL_BALL; i++)     ball[i].loadFromFile(pathBall[i]);
    ButtonChange.loadFromFile("Arrow.png");
}

ChooseBall::ChooseBall(){
    for(int i = 0; i < 6; i++)      Fire[i].loadFromFile(pathFire[i]);
    for(int i = 0; i < TOTAL_BALL; i++)     ball[i].loadFromFile(pathBall[i]);
    ButtonChange.loadFromFile("Arrow.png");
    
    selectedBall = FOOT_BALL;
}

#endif /* ChooseBall_h */
