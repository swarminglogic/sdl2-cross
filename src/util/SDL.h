#ifndef UTIL_SDL_H
#define UTIL_SDL_H

#if defined(__ANDROID__)
#include <SDL.h>
#else
#include <SDL2/SDL.h>
#endif

#include <util/RAIIhelper.h>


/**
 * SDL header wrapper
 * @author SwarmingLogic (Roald Fernandez)
 */
namespace sdl {
  CREATE_RAII_UP(SDL_Surface,  SDL_FreeSurface)     SurfacePtr;
  CREATE_RAII_UP(SDL_Texture,  SDL_DestroyTexture)  TexturePtr;
  CREATE_RAII_UP(SDL_Renderer, SDL_DestroyRenderer) RendererPtr;
  CREATE_RAII_UP(SDL_Window,   SDL_DestroyWindow)   WindowPtr;
  CREATE_RAII_UP(SDL_RWops,    SDL_RWclose)         RWopsPtr;
  CREATE_RAII2_UP(SDL_GLContext,   SDL_GL_DeleteContext)   GLContextPtr;
}




#endif  // UTIL_SDL_H
