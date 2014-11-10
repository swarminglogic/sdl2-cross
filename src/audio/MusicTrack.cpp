#include <audio/MusicTrack.h>

#include <audio/AudioPlayback.h>
#include <util/FileUtil.h>


MusicTrack::MusicTrack(const AssetMusic& musicfile)
    : music_(Mix_LoadMUS(FileUtil::prefixPath(musicfile).c_str()))
{
}


MusicTrack::~MusicTrack()
{
}
