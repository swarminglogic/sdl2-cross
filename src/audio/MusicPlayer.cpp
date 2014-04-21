#include <audio/MusicPlayer.h>

#include <cassert>


MusicPlayer::MusicPlayer()
{
}


MusicPlayer::~MusicPlayer()
{
}


void MusicPlayer::play(MusicTrackShPtr music)
{
  currentTrack_ = music;
  Mix_PlayMusic(currentTrack_->music_.get(), 1);
}


bool MusicPlayer::isPlaying(MusicTrackShPtr music) const
{
  return Mix_PlayingMusic() && (currentTrack_ == music);
}

// TODO swarminglogic, 2014-02-08:
// // make a music finished function
// void musicFinished()
// {
//     printf("Music stopped.\n");
// }
// ...
// // use musicFinished for when music stops
// Mix_HookMusicFinished(musicFinished);
