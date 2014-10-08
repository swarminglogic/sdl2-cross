#include <cstdlib>
#include <iostream>
#include <memory>
#include <string>

#include <audio/AudioResourceManager.h>
#include <audio/SDL_mixer.h>
#include <core/MainManager.h>
#include <graphics/SDL_image.h>
#include <graphics/SDL_opengl.h>
#include <util/Log.h>
#include <util/SDL.h>


void quitSdl() {
  // SDL_image
  IMG_Quit();

  // SDL_mixer
  const int nOpenAudio = Mix_QuerySpec(nullptr, nullptr, nullptr);
  for (int i = 0 ; i < nOpenAudio ; ++i)
    Mix_CloseAudio();
  while (Mix_Init(0))
    Mix_Quit();

  // SDL
  SDL_Quit();
}


[[ noreturn ]]
void quit(int exitCode) {
  Log log("main");
  if (exitCode == EXIT_SUCCESS)
    log.i("Terminating application normally");
  else
    log.i("Terminating application due to error");
  log.i("Terminating SDL");
  quitSdl();

  exit(exitCode);
}


int main(int argc, char *argv[]) {
  (void)argc;
  (void)argv;

  // Initialize logger system
  LogManager& lm = LogManager::instance();
  lm.setStreamTarget(std::cout);
#ifdef __ANDROID__
  lm.setLogfilePath("runtime.log");
#endif
  lm.logColumnHeaders();
  Log log("main");

  try {
    MainManager& mainManager = MainManager::instance();
    mainManager.initialize();
    mainManager.run();
  }
  catch (Exception e) {
    log.e(std::string("Terminating early due to exception: ") + e.what());
    quit(EXIT_FAILURE);
  }
  catch (...) {
    log.e(std::string("Terminating early due to unhandled exception"));
    quit(EXIT_FAILURE);
  }

  MainManager::instance().finalize();
  quit(EXIT_SUCCESS);
  return EXIT_SUCCESS;
}
