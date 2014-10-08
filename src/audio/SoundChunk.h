#ifndef AUDIO_SOUNDCHUNK_H
#define AUDIO_SOUNDCHUNK_H

#include <memory>

#include <audio/SDL_mixer.h>
#include <util/Asset.h>


/**
 * SoundChunk class.
 * Represents the container of SDL_mixer's Mix_Chunk.
 * @author SwarmingLogic (Roald Fernandez)
 */
class SoundChunk
{
 public:
  explicit SoundChunk(const AssetSound& soundfile);
  explicit SoundChunk(sdl::SoundPtr sound);
  virtual ~SoundChunk();

  /**
   * Set/get volume level.
   * Value is clamped in range [0, 1]
   */
  float getVolumeFactor() const;
  void setVolumeFactor(float volumeFactor);

 private:
  friend class SoundPlayer;
  sdl::SoundPtr sound_;
  float volumeFactor_;


  // NonCopyable
  SoundChunk(const SoundChunk& c);
  SoundChunk& operator=(const SoundChunk& c);
};

typedef std::shared_ptr<SoundChunk> SoundChunkShPtr;

#endif  // AUDIO_SOUNDCHUNK_H
