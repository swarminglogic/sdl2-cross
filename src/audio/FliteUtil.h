#ifndef AUDIO_FLITEUTIL_H
#define AUDIO_FLITEUTIL_H

#include <cassert>

#include <audio/SDL_mixer.h>

// Forward declarations
struct cst_wave_struct;
typedef cst_wave_struct cst_wave;

class FliteUtil
{
 public:
  static sdl::SoundPtr textToSpeech(const std::string& text);

 private:
  static bool writeWaveData(const cst_wave *w, SDL_RWops* out);

  FliteUtil();
  virtual ~FliteUtil();
};



#endif  // AUDIO_FLITEUTIL_H
