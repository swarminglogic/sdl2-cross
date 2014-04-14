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
#include <GLES3/gl3.h>
#include <GLES3/gl3ext.h>

#else
// DESKTOP SPECIFICS

#define NO_SDL_GLEXT
#include <SDL2/SDL_opengl.h>
#ifdef NO_SDL_GLEXT
#include <GL/glext.h>
#endif

#endif

#endif
