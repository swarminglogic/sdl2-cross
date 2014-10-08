#ifndef AUDIO_MUSICPLAYER_H
#define AUDIO_MUSICPLAYER_H

#include <audio/MusicTrack.h>


/**
 * MusicPlayer class.
 * Handles interface with SDL_mixer for music playback
 *
 * @author SwarmingLogic (Roald Fernandez)
 */
class MusicPlayer
{
 public:
  MusicPlayer();
  virtual ~MusicPlayer();

  void play(MusicTrackShPtr music);
  bool isPlaying(MusicTrackShPtr music) const;

 private:
  MusicTrackShPtr currentTrack_;


  // NonCopyable
  MusicPlayer(const MusicPlayer& c);
  MusicPlayer& operator=(const MusicPlayer& c);
};

#endif  // AUDIO_MUSICPLAYER_H
