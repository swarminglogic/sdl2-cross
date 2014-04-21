#include <audio/AudioPlayback.h>

#include <cassert>

#include <audio/SDL_mixer.h>
#include <util/Asset.h>


AudioPlayback::AudioPlayback()
  : log_("AudioPlayback")
{
}


AudioPlayback::~AudioPlayback()
{
}


MusicPlayer& AudioPlayback::music() {
  static MusicPlayer instance;
  return instance;
}


SoundPlayer& AudioPlayback::sound()
{
  static SoundPlayer instance;
  return instance;
}

