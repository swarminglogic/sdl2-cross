#include <audio/MusicTrack.h>

#include <audio/AudioPlayback.h>


MusicTrack::MusicTrack(const AssetMusic& musicfile)
  : music_(Mix_LoadMUS(musicfile.path().c_str()))
{
}


MusicTrack::~MusicTrack()
{
}
