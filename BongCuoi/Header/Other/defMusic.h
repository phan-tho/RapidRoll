//  Created by Phan Tho on 19/03/2024.

#ifndef defMusic_h
#define defMusic_h

#include "def.h"

Mix_Music* gBackGrMusic = NULL;         // Music play long sound and can be stop easily

Mix_Chunk* gMusicWhenMove = NULL;       // Chunk can promote via channel
Mix_Chunk* gTailFireMove = NULL;

#endif /* defMusic_h */
