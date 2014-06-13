#ifndef GRAPHICS_GLCHECK_H
#define GRAPHICS_GLCHECK_H


#define STATUS_CASE(enum) case enum: return #enum
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-function"
namespace gldebug {
  static const char* glStatusString(GLenum error)
  {
    switch(error) {
      STATUS_CASE(GL_NO_ERROR);
      STATUS_CASE(GL_INVALID_ENUM);
      STATUS_CASE(GL_INVALID_VALUE);
      STATUS_CASE(GL_INVALID_OPERATION);
      STATUS_CASE(GL_INVALID_FRAMEBUFFER_OPERATION);
      STATUS_CASE(GL_OUT_OF_MEMORY);
      STATUS_CASE(GL_FRAMEBUFFER_COMPLETE);
      STATUS_CASE(GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT);
      STATUS_CASE(GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT);
      STATUS_CASE(GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE);
      STATUS_CASE(GL_FRAMEBUFFER_UNSUPPORTED);
    }
    return NULL;
  }
}
#pragma GCC diagnostic pop
#undef STATUS_CASE

// Define this variable if a CHECKGL error should trigger an assert
#define GL_ASSERT_ERROR

#ifndef CHECKGL
  #ifndef NDEBUG
    #ifndef GL_ASSERT_ERROR
    #define CHECKGL()                                     \
        do {                                              \
            GLenum glError = glGetError();                \
            if(glError != GL_NO_ERROR) {                  \
              Log log("GlState CheckGL");                 \
              log.e() << __FILE__ << ":" << __LINE__      \
                      << ": OpenGL Error: "               \
                      << gldebug::glStatusString(glError) \
                      << Log::end;                        \
            }                                             \
        } while(__LINE__ == -1)
    #else // ifdef GL_ASSERT_ERROR
    #define CHECKGL()                                     \
        do {                                              \
            GLenum glError = glGetError();                \
            if(glError != GL_NO_ERROR) {                  \
              Log log("GlState CheckGL");                 \
              log.e() << __FILE__ << ":" << __LINE__      \
                      << ": OpenGL Error: "               \
                      << gldebug::glStatusString(glError) \
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
