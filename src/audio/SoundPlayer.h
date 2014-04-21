#ifndef AUDIO_SOUNDPLAYER_H
#define AUDIO_SOUNDPLAYER_H

#include <audio/SoundChunk.h>
#include <util/Log.h>


/**
 * SoundPlayer class.
 * Handles interface with SDL_mixer for sound chunk playback
 *
 * @author SwarmingLogic (Roald Fernandez)
 */
class SoundPlayer
{
public:
  SoundPlayer();
  virtual ~SoundPlayer();

  /**
   * Plays a sound chunk, the position defines on the stereo space,
   * where it it plays. 0 is balanced, -1:left, 1:right.
   *
   * Distance determines how loud it is played back:
   *  0:closest (loud)
   *  1:farthest (quiet)
   *
   * Position is clamped [-1, 1]
   * Distance is clamped [0, 1]
   */
  void play(SoundChunkShPtr sound,
            float position = 0.0f,
            float distance = 0.0f);

  // TODO swarminglogic, 2014-02-08:
  //
  // Keep track of each sound currently being played, and in which channel.  it
  // uses callback to mantain this list properly, together with a map to the
  // SoundChunkShPtr, and some other playback ID, channel?.
  //
  // This should allow the functionality to update the position of a
  // channel (say a long play clip that changes position while playing,
  // then it should play at a fixed panning.
  //
  // update(SoundChunkShPtr sound, id?, position, distance)

private:
  Log log_;

  // NonCopyable
  SoundPlayer(const SoundPlayer& c);
  SoundPlayer& operator=(const SoundPlayer& c);
};

#endif
