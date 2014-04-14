#ifndef AUDIO_SDL_MIXER_H
#define AUDIO_SDL_MIXER_H


#if defined(__ANDROID__)
#include <SDL_mixer.h>
#else
#include <SDL2/SDL_mixer.h>
#endif

#include <util/RAIIhelper.h>


/**
 * SDL mixer header wrapper
 * @author SwarmingLogic (Roald Fernandez)
 */
CREATE_RAII_UP(Mix_Music, Mix_FreeMusic)  SdlMusicPtr;
CREATE_RAII_UP(Mix_Chunk, Mix_FreeChunk)  SdlSoundPtr;

#endif
