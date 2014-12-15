#ifndef UTIL_SDL_H
#define UTIL_SDL_H

#include <memory>

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
struct SDL_Deleter {
  void operator()(SDL_Surface*  ptr) { if (ptr) SDL_FreeSurface(ptr); }
  void operator()(SDL_Texture*  ptr) { if (ptr) SDL_DestroyTexture(ptr); }
  void operator()(SDL_Renderer* ptr) { if (ptr) SDL_DestroyRenderer(ptr); }
  void operator()(SDL_Window*   ptr) { if (ptr) SDL_DestroyWindow(ptr); }
  void operator()(SDL_RWops*    ptr) { if (ptr) SDL_RWclose(ptr); }

  void operator()(SDL_GLContext* ptr) { if (ptr) SDL_GL_DeleteContext(*ptr); }
};

// Unique Pointers
using SurfacePtr  = std::unique_ptr<SDL_Surface,  SDL_Deleter>;
using TexturePtr  = std::unique_ptr<SDL_Texture,  SDL_Deleter>;
using RendererPtr = std::unique_ptr<SDL_Renderer, SDL_Deleter>;
using WindowPtr   = std::unique_ptr<SDL_Window,   SDL_Deleter>;
using RWopsPtr    = std::unique_ptr<SDL_RWops,    SDL_Deleter>;

using GLContextPtr = std::unique_ptr<SDL_GLContext, SDL_Deleter>;

// Shared pointers
using SurfaceShPtr  = shared_ptr_with_deleter<SDL_Surface,  SDL_Deleter>;
using TextureShPtr  = shared_ptr_with_deleter<SDL_Texture,  SDL_Deleter>;
using RendererShPtr = shared_ptr_with_deleter<SDL_Renderer, SDL_Deleter>;
using WindowShPtr   = shared_ptr_with_deleter<SDL_Window,   SDL_Deleter>;
using RWopsShPtr    = shared_ptr_with_deleter<SDL_RWops,    SDL_Deleter>;
}




#endif  // UTIL_SDL_H
