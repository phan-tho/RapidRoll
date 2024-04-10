#include "Header/Header.h"

int main( int argc, char* argv[] ){
    init();
    loadMedia();
    
    MainMenu menu;
    menu.handle();
    
    close();
    return 0;
}

void loadMedia(){
//    gFont = TTF_OpenFont("Sears_Tower.ttf", 30);
    gFont = TTF_OpenFont("turok.ttf", 30);
}

void close(){
    gTextTexture.freeFire();
    
    TTF_CloseFont(gFont);
    gFont = NULL;

    //Destroy window
    SDL_DestroyRenderer( gRenderer );
    SDL_DestroyWindow( gWindow );
    gWindow = NULL;
    gRenderer = NULL;

    //Quit SDL subsystems
    IMG_Quit();
    TTF_Quit();
    Mix_Quit();
    SDL_Quit();
}
