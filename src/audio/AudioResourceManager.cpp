#include <audio/AudioResourceManager.h>

#include <string>

#include <audio/MusicTrack.h>
#include <audio/SoundChunk.h>
#include <util/Asset.h>
#include <util/FileUtil.h>


AudioResourceManager::AudioResourceManager()
  : log_("AudioResouceManager")
{
}


AudioResourceManager::~AudioResourceManager()
{
}



MusicTrackShPtr AudioResourceManager::load(const AssetMusic& music)
{
  MusicTrackShPtr resource = loadedMusic_[music];
  if (!resource) {
    if (!FileUtil::exists(music)) {
      resource.reset();
    } else {
      resource.reset(new MusicTrack(music));
      loadedMusic_[music] = resource;
    }
  }
  return resource;
}


SoundChunkShPtr AudioResourceManager::load(const AssetSound& sound)
{
  log_.d() << "Trying to load sound: " << sound.path() << Log::end;
  SoundChunkShPtr resource = loadedSounds_[sound];
  if (!resource) {
    if (!FileUtil::exists(sound)) {
      resource.reset();
    } else {
      resource.reset(new SoundChunk(sound));
      loadedSounds_[sound] = resource;
    }
  }
  return resource;
}


SoundChunkShPtr AudioResourceManager::loadSound(const std::string& soundfile)
{
  return load(AssetSound(soundfile));
}


MusicTrackShPtr AudioResourceManager::loadMusic(const std::string&  musicfile)
{
  return load(AssetMusic(musicfile));
}
