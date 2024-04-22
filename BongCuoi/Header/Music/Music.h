//  Created by Phan Tho on 02/04/2024.
#ifndef Music_h
#define Music_h

class Music{
public:
    Music();
    
    void backGrMusic();
    void pauseBackGrMusic();
    void resumeBackGrMusic();
    
    void whenTappedButton();
    
    void whenMove();
    void whenNotMove();
    
    void whenShoot();
    
    void whenDie();
    void whenKill();
    
    void whenEatHeartFuel();
    
    void close();
    
private:
    enum channel{
        FIRST_CHANNEL,          // move, eat
        SECCOND_CHANNEL,        // die, shoot, tap, load bullets
        THIRD_CHANNEL,
        FOURTH_CHANNEL
    };
    
    Mix_Music* mBackGrMusic;
    
    Mix_Chunk* mMusicWhenMove;
    Mix_Chunk* mMusicWhenEat;
    
    Mix_Chunk* mMusicWhenDie;
    Mix_Chunk* mMusicWhenShoot;
    Mix_Chunk* mMusicWhenTap;
    Mix_Chunk* mMusicWhenKill;
    
//    Mix_Chunk* mMsLoadBullet;
};

void Music::backGrMusic(){
    Mix_PlayMusic(mBackGrMusic, -1);
}

void Music::resumeBackGrMusic(){
    if(Mix_PausedMusic()){
        Mix_ResumeMusic();
    }
}

void Music::pauseBackGrMusic(){
    Mix_PauseMusic();
}

void Music::whenMove(){
    if( !Mix_Playing(FIRST_CHANNEL) ){
        Mix_PlayChannel(FIRST_CHANNEL, mMusicWhenMove, -1);
    }
}

void Music::whenNotMove(){
    if( Mix_Playing(FIRST_CHANNEL) ){
        Mix_HaltChannel(FIRST_CHANNEL);
    }
}

void Music::whenEatHeartFuel(){
    Mix_PlayChannel(SECCOND_CHANNEL, mMusicWhenEat, 0);
}

void Music::whenTappedButton(){
    Mix_PlayChannel(SECCOND_CHANNEL, mMusicWhenTap, 0);
}

void Music::whenDie(){
    Mix_PlayChannel(SECCOND_CHANNEL, mMusicWhenDie, 0);
}

void Music::whenKill(){
    Mix_PlayChannel(SECCOND_CHANNEL, mMusicWhenKill, 0);
}

void Music::whenShoot(){
    Mix_PlayChannel(SECCOND_CHANNEL, mMusicWhenShoot, 0);
}


void Music::close(){
    Mix_FreeMusic(mBackGrMusic);
    
    Mix_FreeChunk(mMusicWhenMove);
    Mix_FreeChunk(mMusicWhenEat);
    Mix_FreeChunk(mMusicWhenDie);
    Mix_FreeChunk(mMusicWhenShoot);
    Mix_FreeChunk(mMusicWhenTap);
    Mix_FreeChunk(mMusicWhenKill);
    
    mBackGrMusic    = NULL;
    mMusicWhenMove  = NULL;
    mMusicWhenEat   = NULL;
    mMusicWhenDie   = NULL;
    mMusicWhenShoot = NULL;
    mMusicWhenTap   = NULL;
    mMusicWhenKill  = NULL;
}

Music::Music(){
    mBackGrMusic   = Mix_LoadMUS("BackGrMusic.wav");
    
    mMusicWhenMove = Mix_LoadWAV("fireMove.wav");
    mMusicWhenEat  = Mix_LoadWAV("EatHeart.wav");
    
    mMusicWhenDie   = Mix_LoadWAV("Died.wav");
    mMusicWhenKill  = Mix_LoadWAV("CashRegister.wav");
    mMusicWhenShoot = Mix_LoadWAV("SingleBullet.wav");
    mMusicWhenTap   = Mix_LoadWAV("Tap.wav");
    
    Mix_VolumeMusic(MIX_MAX_VOLUME/32);
    Mix_VolumeChunk(mMusicWhenMove, MIX_MAX_VOLUME/64);
    Mix_VolumeChunk(mMusicWhenShoot, MIX_MAX_VOLUME/12);
}

#endif /* Music_h */
