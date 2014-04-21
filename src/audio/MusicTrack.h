#ifndef AUDIO_MUSICTRACK_H
#define AUDIO_MUSICTRACK_H

#include <memory>

#include <audio/SDL_mixer.h>
#include <util/Asset.h>


/**
 * MusicTrack class.
 *
 * @author SwarmingLogic (Roald Fernandez)
 */
class MusicTrack
{
public:
  MusicTrack(const AssetMusic& musicfile);
  virtual ~MusicTrack();

private:
  friend class MusicPlayer;

  SdlMusicPtr music_;

  // NonCopyable
  MusicTrack(const MusicTrack& c);
  MusicTrack& operator=(const MusicTrack& c);
};

typedef std::shared_ptr<MusicTrack> MusicTrackShPtr;

#endif
