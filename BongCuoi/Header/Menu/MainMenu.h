//  Created by Phan Tho on 09/04/2024.

#ifndef MainMenu_h
#define MainMenu_h

#include "ChooseBall.h"

class MainMenu{
public:
    MainMenu();
    
    void handle();
    
protected:
    void init();
    
    void close();
    
    void renderOptMode();
    
    void renderButton();
    
    void handeEvent(SDL_Event* e);      // isTapped
    
    void handleInputText(SDL_Event* e);
    
    void ChooseMode(SDL_Event* e);
    
    void play();
    
    void writeFile();
    
    bool isInButton(const int& button);
    
private:
    const int FRAME_SLT_HEIGHT = 190;
    const int FRAME_SLT_WIDTH  = 420;
    
    enum CHANNEL{
        FIRST_CHANNEL,      // Tapped
        SECCOND_CHANNEL     // Back ground music
    };
    
    enum Option{
        BASIC_MODE,
        SHOOTING_MODE,
        PLAY,
        HIGH_SCORE,
        USER_NAME,
        TOTAL,
        NONE
    };
    
    bool ButtonState[2];         // 0 when mouse out button, 1 when mouse in button
    int xMouse, yMouse;
    
    const std::string TURTORIAL[TOTAL] = { "Ball with gun chase and shoot enemy", 
                                           "Move the ball and avoid trap" };
    
    int isZoomOut = NONE;
    int selectedMode = BASIC_MODE;
    bool isFixingName = false;
    
    enum idData{
        lineName,
        lineBasicMode,
        lineShootingMode
    };
    std::vector<std::string> data;
    
    const SDL_Point POS[TOTAL] = { {14, 564}, { 14, 380 },
                                    {14, 310}, {14, 260}, {350, 150} };
    const SDL_Point DIMENSIONS[TOTAL] = { {400, 170}, {400, 170},
                                          { 140, 40 }, {140, 40}, {50, 20} };        // w, h
    
    LTexture mBackGr;
    LTexture blackScreen;
    LTexture frameSelect;
    
    LTexture mPlay;
    LTexture mHighScore;
//    LTexture mUserName;
    
    LTexture textTurtorial[2];
    LTexture textUserName;
    LTexture textScore;
    
    Mix_Chunk* mBackGrMusic;
    Mix_Chunk* mMusicWhenTap;
    
    ChooseBall chooseBall;
};

void MainMenu::handle(){
    Mix_PlayChannel(SECCOND_CHANNEL, mBackGrMusic, -1);
    
    bool quit = false;
    SDL_Event e;

    while( !quit ){
        while( SDL_PollEvent( &e ) != 0 ){
            if( e.type == SDL_QUIT){
                quit = true;
                close();
            }
            handeEvent(&e);
        }
        SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
        SDL_RenderClear( gRenderer );
        
        mBackGr.render(0, 0, NULL);
        
        chooseBall.render();
        renderOptMode();
        renderButton();
        
        SDL_RenderPresent( gRenderer );
    }
}


// CHOOSE MODE
// CHANGE USER NAME
// PLAY GAME
void MainMenu::handeEvent(SDL_Event *e){
    chooseBall.handleEvent(e);
    // TYPE NEW NAME
    if(isFixingName){
        handleInputText(e);
    }
    
    if(e->type != SDL_MOUSEBUTTONDOWN && e->type != SDL_MOUSEMOTION){
        return;
    }
    SDL_GetMouseState(&xMouse, &yMouse);
    
    // Not zoom out any thing when mouse isnot in any button
    isZoomOut = NONE;
    
    ChooseMode(e);
    
    if( isInButton(PLAY) ){
        isZoomOut = PLAY;
        
        if(e->type == SDL_MOUSEBUTTONDOWN){
            play();
        }
    }
    else if( isInButton(HIGH_SCORE) ){
        isZoomOut = HIGH_SCORE;
        
        // RESET HIGHEST SCORE WHEN TAPPED THIS BUTTON
        if(e->type == SDL_MOUSEBUTTONDOWN){
            data[lineBasicMode] = "0";
            data[lineShootingMode] = "0";
            writeFile();
        }
    }
    else if( isInButton(USER_NAME) ){
        if(isFixingName == 0)      isZoomOut = USER_NAME;

        if(e->type == SDL_MOUSEBUTTONDOWN){
            isFixingName = 1;
            data[lineName] = " ";
//            textUserName.loadFromRenderedText(data[lineName], {255, 255, 255});
        }
    }
}


// READ FROM KEYBOARD
// UPDATE USER NAME
// WRITE TO FILE DATA
// INPUT: KEYBOARD EVENT
void MainMenu::handleInputText(SDL_Event* e){
    if(e->type == SDL_TEXTINPUT){
        data[lineName] += e->text.text;
//        textUserName.loadFromRenderedText(data[lineName], {255, 255, 255});
    }
    if(e->key.keysym.sym == SDLK_BACKSPACE){
        if(data[lineName].size() > 1){
            data[lineName].pop_back();
//            textUserName.loadFromRenderedText(data[lineName], {255, 255, 255});
        }
    }
    else if(e->type == SDL_MOUSEBUTTONDOWN || e->key.keysym.sym == SDLK_RETURN){
        isFixingName = 0;
        writeFile();
    }
}

void MainMenu::ChooseMode(SDL_Event* e){
    for(int button = 0; button < 2; button++){
        if( isInButton(button)) {
            if( e->type == SDL_MOUSEBUTTONDOWN ){
                selectedMode = button;
                // play music when tap
                Mix_PlayChannel(0, mMusicWhenTap, 0);
            }
            ButtonState[button] = 1;
            ButtonState[!button] = 0;
            return;
        }
        else{
            ButtonState[button] = 0;       // is hided
        }
    }
}



// WHEN CLICK BUTTON PLAY. GO TO PLAY SELECTED MODE
// DESTROY MEDIA OF MENU WHEN START GAME AND INIT AFTER THAT
void MainMenu::play(){
    Mix_PlayChannel(FIRST_CHANNEL, mMusicWhenTap, 0);
    Mix_HaltChannel(SECCOND_CHANNEL);
    
    // CLOSE MEDIA OF MENU
    close();
    if(selectedMode == SHOOTING_MODE){
        ShootingMode shootingMode;
        shootingMode.Play();
    }
    else{
        BasicMode basicMode;
        basicMode.Play();
    }
    
    // INIT MEDIA
    init();
    // resume back ground music
    Mix_PlayChannel(SECCOND_CHANNEL, mBackGrMusic, -1);
}


// Basic mode and Shooting mode
void MainMenu::renderOptMode(){
    if(ButtonState[BASIC_MODE] || ButtonState[SHOOTING_MODE]){
        for(int button = 0; button < 2; button++){
            if(ButtonState[button]){
                frameSelect.render(POS[button].x - 10, POS[button].y - 10, NULL);
                textTurtorial[button].renderCustomSize(30, 350, 0.6);
            }
            else{
                blackScreen.render(POS[button].x, POS[button].y, NULL);
            }
        }
    }
    else{
        frameSelect.render(POS[selectedMode].x - 10, POS[selectedMode].y - 10, NULL);
        textTurtorial[selectedMode].renderCustomSize(30, 350, 0.6);
        
        blackScreen.render(POS[!selectedMode].x, POS[!selectedMode].y, NULL);
    }
}


// RENDER ALL OPTION BUTTON AND INFO
// High score
// Button Play
// User name
// UPDATE FEATURE ZOOM OUT IN FUTURE
void MainMenu::renderButton(){
    if(isZoomOut == PLAY){
        mPlay.renderCustomSize(POS[PLAY].x - DIMENSIONS[PLAY].x/10, POS[PLAY].y - DIMENSIONS[PLAY].y/10, 1.2);
    }
    else{
        mPlay.render(POS[PLAY].x, POS[PLAY].y, NULL);
    }
    
    if(isZoomOut == HIGH_SCORE){
        mHighScore.renderCustomSize(POS[HIGH_SCORE].x - DIMENSIONS[HIGH_SCORE].x/10, POS[HIGH_SCORE].y - DIMENSIONS[HIGH_SCORE].y/10, 1.2);
        textScore.loadFromRenderedText("Tap to RESET", {255, 0, 0});
        textScore.renderCustomSize(POS[HIGH_SCORE].x + 6, POS[HIGH_SCORE].y + 6, 0.8);
    }
    else{
        mHighScore.render(POS[HIGH_SCORE].x, POS[HIGH_SCORE].y, NULL);
        textScore.loadFromRenderedText(data[(ButtonState[SHOOTING_MODE] == 1 ? lineShootingMode : lineBasicMode)], {255, 0, 0});
        textScore.renderCustomSize(POS[HIGH_SCORE].x + 40, POS[HIGH_SCORE].y + 6, 0.8);
    }
    
    if(isZoomOut == USER_NAME){
        textUserName.loadFromRenderedText("RENAME", {255, 255, 255});
        textUserName.renderCustomSize(POS[USER_NAME].x - 5, POS[USER_NAME].y, 0.8);
    }
    else{
//        mUserName.render(POS[USER_NAME].x, POS[USER_NAME].y, NULL);
        textUserName.loadFromRenderedText(data[lineName], {255, 255, 255});
        textUserName.renderCustomSize(SCREEN_WIDTH - textUserName.getWidth(), POS[USER_NAME].y, 0.8);
    }
}


void MainMenu::writeFile(){
    std::ofstream out("YouCantSeeMe.txt");
    std::cout << (out.is_open() ? "write success\n" : "deo mo duoc file out\n");
    out << data[lineName] << "\n" << data[lineBasicMode] << "\n" << data[lineShootingMode] << "\n";
    out.close();
}


// CHECK IS IN BUTTON
// INPUT: pos and dimensions
bool MainMenu::isInButton(const int& button){
    return (xMouse >= POS[button].x &&
            xMouse <= POS[button].x + DIMENSIONS[button].x &&
            yMouse >= POS[button].y &&
            yMouse <= POS[button].y + DIMENSIONS[button].y );
}


// CLOSE MEDIA OF MENU
void MainMenu::close(){
    chooseBall.close();
    
    mBackGr.freeFire();
    blackScreen.freeFire();
    frameSelect.freeFire();
    
    mPlay.freeFire();
    mHighScore.freeFire();
//    mUserName.freeFire();
    
    textTurtorial[0].freeFire();
    textTurtorial[1].freeFire();
    
    textScore.freeFire();
    textUserName.freeFire();
    
//    music.close();
    Mix_FreeChunk(mBackGrMusic);
    Mix_FreeChunk(mMusicWhenTap);
    
    mMusicWhenTap = NULL;
    mBackGrMusic = NULL;
}


// INIT MEDIA AFTER GAME IS PERFORMED
void MainMenu::init(){
    chooseBall.init();
    
    mBackGr.loadFromFile("MainMenu.png");
    blackScreen.loadFromFile("BlackScreen.png");
    blackScreen.setAlpha(100);
    
    frameSelect.loadFromFile("Selected.png");
    
    mPlay.loadFromFile("Play.png");
    mHighScore.loadFromFile("HighScore.png");
//    mUserName.loadFromFile("UserName.png");
    
    textTurtorial[SHOOTING_MODE].loadFromRenderedText(TURTORIAL[SHOOTING_MODE], {0, 0, 0});
    textTurtorial[BASIC_MODE].loadFromRenderedText(TURTORIAL[BASIC_MODE], {0, 0, 0});
    textUserName.loadFromRenderedText(data[lineName], {255, 255, 255});
    
    mBackGrMusic  = Mix_LoadWAV("MainMenu.wav");
    mMusicWhenTap = Mix_LoadWAV("Tap.wav");
}

MainMenu::MainMenu(){
    std::ifstream in("YouCantSeeMe.txt");
    std::cout << (in.is_open() ? "Opened\n" : "Deo the mo\n");
    
//    in << "Guest\n0\n0";                // init file. This file can't see. This is feature
    
    std::string str;
    while(getline(in, str)){
        data.push_back(str);
    }
    in.close();
    
    mBackGr.loadFromFile("MainMenu.png");
    blackScreen.loadFromFile("BlackScreen.png");
    blackScreen.setAlpha(100);
    
    frameSelect.loadFromFile("Selected.png");
    
    mPlay.loadFromFile("Play.png");
    mHighScore.loadFromFile("HighScore.png");
//    mUserName.loadFromFile("UserName.png");
    
    textTurtorial[SHOOTING_MODE].loadFromRenderedText(TURTORIAL[SHOOTING_MODE], {0, 0, 0});
    textTurtorial[BASIC_MODE].loadFromRenderedText(TURTORIAL[BASIC_MODE], {0, 0, 0});
    textUserName.loadFromRenderedText(data[lineName], {255, 255, 255});
    
    mBackGrMusic  = Mix_LoadWAV("MainMenu.wav");
    mMusicWhenTap = Mix_LoadWAV("Tap.wav");
    
    ButtonState[SHOOTING_MODE] = 0;
    ButtonState[BASIC_MODE] = 0;
}

#endif /* MainMenu_h */