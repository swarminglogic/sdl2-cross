#ifndef AUDIO_AUDIORESOURCEMANAGER_H
#define AUDIO_AUDIORESOURCEMANAGER_H

#include <iosfwd>
#include <map>
#include <memory>

#include <unordered_map>
#include <util/Asset.h>
#include <util/Log.h>


class MusicTrack;
class SoundChunk;

/**
 * AudioResourceManager class.
 *
 * Manages audio resource files, making sure that only one
 * file is loaded to memory at one point.
 *
 * Future:
 *
 * // TODO swarminglogic, 2014-01-27:
 * // 1: Extend to asynchronously load files, in a non-blocking way.
 * // 2: Maintain a resource alive even though single user abandoned it.
 * // 3: Check if resource has been modified. Request file.
 * //    This does not alter current memory pointed to, unless all users have
 * //    abandoned old content. What is the GUID then? File path not enough.
 * //    Is this also only useful when developing?
 * //
 *
 * @author SwarmingLogic (Roald Fernandez)
 */
class AudioResourceManager
{
 public:
  typedef std::shared_ptr<MusicTrack> MusicTrackShPtr;
  typedef std::shared_ptr<SoundChunk> SoundChunkShPtr;

  AudioResourceManager();
  virtual ~AudioResourceManager();

  MusicTrackShPtr load(const AssetMusic& music);
  MusicTrackShPtr loadMusic(const std::string&  musicfile);

  SoundChunkShPtr load(const AssetSound& sound);
  SoundChunkShPtr loadSound(const std::string&  soundfile);

 private:
  Log log_;

  std::map<AssetMusic, MusicTrackShPtr>    loadedMusic_;
  std::map<AssetSound, SoundChunkShPtr>    loadedSounds_;

  // NonCopyable
  AudioResourceManager(const AudioResourceManager& c);
  AudioResourceManager& operator=(const AudioResourceManager& c);
};

#endif  // AUDIO_AUDIORESOURCEMANAGER_H
