#include <cstdlib>
#include <iostream>
#include <memory>

#ifdef __ANDROID__
#include <SDL.h>
#else
#include <SDL2/SDL.h>
#endif

[[ noreturn ]]
void quit(int returnCode) {
  SDL_Log("Terminating SDL");
  SDL_Quit();
  exit(returnCode);
}

int main(int argc, char *argv[]) {
  (void)argc; (void)argv;

  SDL_Log("Initializing SDL");
  if (SDL_Init(SDL_INIT_EVERYTHING) == -1) {
    SDL_Log("Failed to initialize SDL: %s", SDL_GetError());
  }


  quit(EXIT_SUCCESS);
  return 0;
}
