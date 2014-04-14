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

CREATE_RAII_UP(SDL_Surface,  SDL_FreeSurface)     SdlSurfacePtr;
CREATE_RAII_UP(SDL_Texture,  SDL_DestroyTexture)  SdlTexturePtr;
CREATE_RAII_UP(SDL_Renderer, SDL_DestroyRenderer) SdlRendererPtr;
CREATE_RAII_UP(SDL_Window,   SDL_DestroyWindow)   SdlWindowPtr;

CREATE_RAII2_UP(SDL_GLContext,   SDL_GL_DeleteContext)   SdlGLContextPtr;

#endif
