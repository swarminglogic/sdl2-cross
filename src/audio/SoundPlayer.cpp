#include <audio/SoundPlayer.h>

#include <math/MathUtil.h>


SoundPlayer::SoundPlayer()
  : log_("SoundPlayer")
{
}


SoundPlayer::~SoundPlayer()
{
}


void SoundPlayer::play(SoundChunkShPtr sound,
                       float position,
                       float distance)
{
  distance = MathUtil::clamp(distance, 0.0f, 1.0f);
  position = MathUtil::clamp(position, -1.0f, 1.0f);

  if (sound) {
    int channel = Mix_PlayChannel( -1, sound->sound_.get(), 0);
    if ( channel >= 0 ) {
      const Uint8 right = MathUtil::mapToU8special(position);
      const Uint8 left  = static_cast<Uint8>(254u - right);
      const Uint8 dist  = MathUtil::mapToU8(distance);
      Mix_SetDistance(channel, dist);
      Mix_SetPanning(channel, left, right);
    }
    else {
      log_.w() << "Failed to acquire audio channel" << Log::end;
    }
  }
  else {
    log_.w() << "Tried to play bad sound chunk. " << Log::end;
  }
}
