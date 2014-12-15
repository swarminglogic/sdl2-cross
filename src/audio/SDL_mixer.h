#ifndef AUDIO_SDL_MIXER_H
#define AUDIO_SDL_MIXER_H

#include <memory>

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
namespace sdl {
struct Mix_Deleter {
  void operator()(Mix_Music* ptr) { if (ptr) Mix_FreeMusic(ptr); }
  void operator()(Mix_Chunk* ptr) { if (ptr) Mix_FreeChunk(ptr); }
};

using MusicPtr = std::unique_ptr<Mix_Music, Mix_Deleter>;
using SoundPtr = std::unique_ptr<Mix_Chunk, Mix_Deleter>;

using MusicShPtr = shared_ptr_with_deleter<Mix_Music, Mix_Deleter>;
using SoundShPtr = shared_ptr_with_deleter<Mix_Chunk, Mix_Deleter>;
}

#endif  // AUDIO_SDL_MIXER_H
