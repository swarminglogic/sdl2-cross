#ifndef GRAPHICS_GLCHECK_H
#define GRAPHICS_GLCHECK_H

#include <graphics/SDL_opengl.h>
#include <util/Log.h>


class GlCheck
{
public:
  static const char* glStatusString(GLenum error);
  static bool checkGlFramebuffer();
  static void debugGlFramebuffer();
  static const char* glEnumString(GLenum e);
private:
  GlCheck(){}
  virtual ~GlCheck(){}
};


// Define this variable if a CHECKGL error should trigger an assert
#define GL_ASSERT_ERROR

#ifndef CHECKGL
  #ifndef NDEBUG
    #ifndef GL_ASSERT_ERROR
    #define CHECKGL()                                     \
        do {                                              \
            GLenum glError = glGetError();                \
            if(glError != GL_NO_ERROR) {                  \
              Log log("CheckGL");                 \
              log.e() << __FILE__ << ":" << __LINE__      \
                      << ": OpenGL Error: "               \
                      << GlCheck::glStatusString(glError) \
                      << Log::end;                        \
            }                                             \
        } while(__LINE__ == -1)
    #else // ifdef GL_ASSERT_ERROR
    #define CHECKGL()                                     \
        do {                                              \
            GLenum glError = glGetError();                \
            if(glError != GL_NO_ERROR) {                  \
              Log log("CheckGL");                 \
              log.e() << __FILE__ << ":" << __LINE__      \
                      << ": OpenGL Error: "               \
                      << GlCheck::glStatusString(glError) \
                      << Log::end;                        \
              assert(false);                              \
            }                                             \
        } while(__LINE__ == -1)
    #endif
  #else // ifdef NDEBUG
    #define CHECKGL()
  #endif
#endif


#endif
