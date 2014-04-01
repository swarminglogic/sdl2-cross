#include <cstdlib>
#include <iostream>


#if defined(__IPHONEOS__) || defined(__ANDROID__)
#include <GLES3/gl3.h>
#include <GLES3/gl3ext.h>
#else
#include "SDL_opengl.h"
#endif

#include <SDL.h>
#include <glm/gtc/matrix_transform.hpp>


int main(int argc, char *argv[]) {
  (void)argc; (void)argv;
// #if !defined(__ANDROID__)
//   quit(0);
// #endif

  return 0;
}
