#include <cstdlib>
#include <iostream>

#include <sstream>


#if defined(__IPHONEOS__) || defined(__ANDROID__)
#include <GLES3/gl3.h>
#include <GLES3/gl3ext.h>
#include <cpu-features.h>
#else
#include "SDL_opengl.h"
#endif

#include <SDL.h>
#include <SDL_image.h>
#include <glm/gtc/matrix_transform.hpp>


void logSDLVersion(const std::string& what,
                   const SDL_version& compiled,
                   const SDL_version& linked,
                   std::string revision)
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

/* Call this instead of exit(), so we can clean up SDL: atexit() is evil. */
static void
quit(int rc)
{
    // if (context) {
    //     /* SDL_GL_MakeCurrent(0, NULL); *//* doesn't do anything */
    //     SDL_GL_DeleteContext(context);
    // }
    // SDLTest_CommonQuit(state);
  SDL_VideoQuit();
  SDL_AudioQuit();
  SDL_Quit();
  exit(rc);
}

int main(int argc, char *argv[]) {
  (void)argc; (void)argv;

  SDL_Log("Initializing SDL");
  if (SDL_Init(SDL_INIT_EVERYTHING) == -1) {
    SDL_Log("Failed to initialize SDL: %s", SDL_GetError());
    // throw log_.exception("Failed to initialize SDL", SDL_GetError);
  }

  // Write version information to log
  SDL_version compiled;
  SDL_version linked;
  SDL_VERSION(&compiled);
  SDL_GetVersion(&linked);
  logSDLVersion("SDL", compiled, linked, SDL_GetRevision());

  // std::string test("Hello World 5!");
  // SDL_Log(test.c_str());
  // atexit(SDL_Quit);

  // SDL_Log("Hello");

#if defined(__ANDROID__)
  if (android_getCpuFamily() == ANDROID_CPU_FAMILY_ARM) {
    SDL_Log("Device CPU is of ARM family");
  }
  SDL_Log("Device has %i cores", android_getCpuCount());
#endif

#if !defined(__ANDROID__)
  quit(0);
#endif

  return 0;
}
