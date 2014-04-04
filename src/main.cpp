#include <cstdlib>
#include <iostream>

#include <sstream>


#if defined(__IPHONEOS__) || defined(__ANDROID__)
#include <GLES3/gl3.h>
#include <GLES3/gl3ext.h>
#include <cpu-features.h>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#else
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_opengl.h>
#endif


#include <glm/gtc/matrix_transform.hpp>


void logSDLVersion(const std::string& what,
                   const SDL_version& compiled,
                   const SDL_version& linked,
                   std::string revision = std::string(""))
{
  std::stringstream ss;
  ss << what << " Version (Compiled): "
     << (int)compiled.major << "."
     << (int)compiled.minor << "."
     << (int)compiled.patch;
  if (!revision.empty())
    ss << " (" << revision << ")";

  SDL_Log(ss.str().c_str());

  ss.clear();
  ss.str(std::string(""));
  ss << what << " Version (Runtime):  "
     << (int)linked.major << "."
     << (int)linked.minor << "."
     << (int)linked.patch;
  SDL_Log(ss.str().c_str());
}

#ifndef __ANDROID__
[[ noreturn ]]
#endif
void quit(int returnCode) {
  // SDL_image
  IMG_Quit();

  // SDL_mixer
  const int nOpenAudio = Mix_QuerySpec(nullptr, nullptr, nullptr);
  for (int i = 0 ; i < nOpenAudio ; ++i)
    Mix_CloseAudio();
  while(Mix_Init(0))
    Mix_Quit();

  // SDL
  SDL_Quit();
  exit(returnCode);
}


int main(int argc, char *argv[]) {
  (void)argc; (void)argv;

  SDL_Log("Initializing SDL");
  if (SDL_Init(SDL_INIT_EVERYTHING) == -1) {
    SDL_Log("Failed to initialize SDL: %s", SDL_GetError());
  }

  // Write version information to log
  SDL_version compiled;
  SDL_version linked;
  SDL_VERSION(&compiled);
  SDL_GetVersion(&linked);
  logSDLVersion("SDL", compiled, linked, SDL_GetRevision());

  SDL_Log("Initializing SDL_image");
  int imgFlags = IMG_INIT_PNG | IMG_INIT_JPG;
  int imgFlagsInit = IMG_Init(imgFlags);
  if ((imgFlagsInit & imgFlags) != imgFlags)
    SDL_Log("Failed to initialize SDL_img: %s", IMG_GetError());
  SDL_IMAGE_VERSION(&compiled);
  logSDLVersion("SDL_image", compiled, *IMG_Linked_Version());

  SDL_Log("Initializing SDL_mixer");
  int mixFlags = MIX_INIT_OGG;
  int mixFlagsInit = Mix_Init(mixFlags);
  if ((mixFlagsInit & mixFlags) != mixFlags)
    SDL_Log("Failed to initialize SDL_mixer: %s", Mix_GetError());
  //   // throw log_.exception("Failed to initialize SDL_mixer", Mix_GetError);
  if( Mix_OpenAudio( 22050, MIX_DEFAULT_FORMAT, 2, 1024 ) == -1 )
    SDL_Log("Failed to aquire sound device: %s", Mix_GetError());

  SDL_MIXER_VERSION(&compiled);
  logSDLVersion("SDL_mixer", compiled, *Mix_Linked_Version());

  // Write music decoder information to log
  const int nMusicDecoders = Mix_GetNumMusicDecoders();
  std::stringstream ss;
  if (nMusicDecoders > 0)
    ss << Mix_GetMusicDecoder(0);
  for (int i = 1 ; i < nMusicDecoders ; ++i) {
    ss << ", " << Mix_GetMusicDecoder(i);
  }
  SDL_Log("Music decoders (%i): %s", nMusicDecoders, ss.str().c_str());

  // Write audio decoder information to log
  const int nChunkDecoders = Mix_GetNumChunkDecoders();
  ss.str(std::string(""));
  if (nChunkDecoders > 0)
    ss << Mix_GetChunkDecoder(0);
  for (int i = 1 ; i < nChunkDecoders ; ++i) {
    ss << ", " << Mix_GetChunkDecoder(i);
  }
  SDL_Log("Audio decoders (%i): %s", nChunkDecoders, ss.str().c_str());

#if defined(__ANDROID__)
  if (android_getCpuFamily() == ANDROID_CPU_FAMILY_ARM) {
    SDL_Log("Device CPU is of ARM family");
  }
  SDL_Log("Device has %i cores", android_getCpuCount());
#endif

  SDL_Delay(2000);

  quit(EXIT_SUCCESS);
  return 0;
}
