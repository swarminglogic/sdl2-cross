#ifndef GRAPHICS_SDL_OPENGL_H
#define GRAPHICS_SDL_OPENGL_H

/**
 * SDL_opengl wrapper
 * @author SwarmingLogic (Roald Fernandez)
 */

// Required definition call for OpenGL 3.0+ functions
#define GL_GLEXT_PROTOTYPES 1

#if defined(__ANDROID__)
// ANDROID SPECIFICS
#define USE_OPENGLES
#include <GLES3/gl3.h>
#include <GLES3/gl3ext.h>

#else
// DESKTOP SPECIFICS
#  ifdef __WIN32__
#    define GLEW_STATIC
#  endif
#include <GL/glew.h>
#include <SDL2/SDL_opengl_glext.h>
#endif

#endif  // GRAPHICS_SDL_OPENGL_H
