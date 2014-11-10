#include <audio/SoundChunk.h>

#include <math/MathUtil.h>
#include <util/FileUtil.h>


SoundChunk::SoundChunk(const AssetSound& soundfile)
    : sound_(Mix_LoadWAV(FileUtil::prefixPath(soundfile).c_str()))
{
}


SoundChunk::SoundChunk(sdl::SoundPtr sound)
{
  sound_ = std::move(sound);
}



SoundChunk::~SoundChunk()
{
}


float SoundChunk::getVolumeFactor() const
{
  return volumeFactor_;
}

void SoundChunk::setVolumeFactor(float volumeFactor)
{
  volumeFactor_ = MathUtil::clamp(volumeFactor, 0.0f, 1.0f);
  Mix_VolumeChunk(sound_.get(),
                  static_cast<int>(volumeFactor * MIX_MAX_VOLUME));
}
