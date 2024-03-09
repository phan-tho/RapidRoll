#ifndef LTexture_h
#define LTexture_h

class LTexture
{
    public:
        //Initializes variables
        LTexture();

        //Loads image at specified path
        void loadFromFile( std::string path );

        //Deallocates texture
        void freeFire();
    
        //Set blending
        void setBlendMode( SDL_BlendMode blending = SDL_BLENDMODE_BLEND);

        //Set alpha modulation
        void setAlpha( Uint8 alpha );
    
        #if defined(SDL_TTF_MAJOR_VERSION)
        //Creates image from font string
        bool loadFromRenderedText( std::string textureText, SDL_Color textColor );
        #endif
    
        //Set color modulation
        void setColor( Uint8 red, Uint8 green, Uint8 blue );

        //Renders texture at given point
        void render( int x, int y, SDL_Rect* clip);             // clip ==> a part of pictrure {x, y, w, h}
    
        //Renders texture at given point
        void renderFlip( int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip);
//        void renderFlip( int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE );

        //Gets image dimensions
        int getWidth();
        int getHeight();

    private:
        //The actual hardware texture
        SDL_Texture* mTexture;

        //Image dimensions
        int mWidth;
        int mHeight;
};

void LTexture::loadFromFile(std::string path){
    freeFire();
    SDL_Surface* newSurface = IMG_Load(path.c_str());
    
    if(newSurface == NULL){
        std::cout << "Deo load duoc anh " << path << "\n";
    }
    
    SDL_SetColorKey(newSurface, SDL_TRUE, SDL_MapRGB(newSurface -> format, 0xFF, 0xFF, 0xFF));             // set color key ==> alpha = max
    mTexture = SDL_CreateTextureFromSurface(gRenderer, newSurface);
    if(mTexture != NULL){
        mWidth = newSurface->w;
        mHeight = newSurface->h;
    }
    SDL_FreeSurface(newSurface);
}

void LTexture::render(int x, int y, SDL_Rect* clip){                    // copy to gRender
    SDL_Rect xywh = {x, y, mWidth, mHeight};
    if( clip != NULL ){
        xywh.w = clip->w;
        xywh.h = clip->h;
    }

    SDL_RenderCopy(gRenderer, mTexture, clip, &xywh);
}

void LTexture::renderFlip( int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip )
{
    //Set rendering space and render to screen
    SDL_Rect renderQuad = { x, y, mWidth, mHeight };

    //Set clip rendering dimensions
    if( clip != NULL ){
        renderQuad.w = clip->w;
        renderQuad.h = clip->h;
    }

    //Render to screen
    SDL_RenderCopyEx( gRenderer, mTexture, clip, &renderQuad, angle, center, flip );
}

void LTexture::setColor( Uint8 red, Uint8 green, Uint8 blue )
{
    //Modulate texture
    SDL_SetTextureColorMod( mTexture, red, green, blue );
}

void LTexture::setBlendMode( SDL_BlendMode blending )
{
    //Set blending function
    SDL_SetTextureBlendMode( mTexture, blending );
}

void LTexture::setAlpha( Uint8 alpha )
{
    //Modulate texture alpha
    SDL_SetTextureAlphaMod( mTexture, alpha );
}

LTexture::LTexture(){           // init
    mTexture = NULL;
    mWidth = 0;
    mHeight = 0;
}

void LTexture::freeFire(){
    if(mTexture == NULL)        return;
    SDL_DestroyTexture(mTexture);
    mTexture = NULL;
    mWidth = 0;
    mHeight = 0;
}

#if defined(SDL_TTF_MAJOR_VERSION)
bool LTexture::loadFromRenderedText( std::string textureText, SDL_Color textColor )
{
    //Get rid of preexisting texture
    freeFire();

    //Render text surface
    SDL_Surface* textSurface = TTF_RenderText_Solid( gFont, textureText.c_str(), textColor );
    if( textSurface == NULL )
    {
        printf( "Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError() );
    }
    else
    {
        //Create texture from surface pixels
        mTexture = SDL_CreateTextureFromSurface( gRenderer, textSurface );
        if( mTexture == NULL )
        {
            printf( "Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError() );
        }
        else
        {
            //Get image dimensions
            mWidth = textSurface->w;
            mHeight = textSurface->h;
        }

        //Get rid of old surface
        SDL_FreeSurface( textSurface );
    }
    
    //Return success
    return mTexture != NULL;
}
#endif

int LTexture::getWidth(){
    return mWidth;
}

int LTexture::getHeight(){
    return mHeight;
}

#endif /* LTexture_h */

