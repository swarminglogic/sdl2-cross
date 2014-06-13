#ifndef GRAPHICS_GLSTATE_H
#define GRAPHICS_GLSTATE_H

#include <array>

#include <graphics/SDL_opengl.h>
#include <math/Rect.h>
#include <util/Log.h>


// Uncomment this define to use GL-passthrough
#define GL_STATE_GUARD_DISABLED

/**
 * GlState class.
 *
 * Singleton class for managing OpenGL state, and handling
 * OpenGL state-related changing calls.
 *
 * This keeps track of enabled/disabled parameters, and bindings,
 * and thus avoids redundant opengl state changes.
 *
 * @author SwarmingLogic (Roald Fernandez)
 */
class GlState
{
public:
  // Uncomment as needed.
  enum Capability{
    BLEND = 0,
    // CLIP_DISTANCE0,
    // CLIP_DISTANCE1,
    // CLIP_DISTANCE2,
    // CLIP_DISTANCE3,
    // CLIP_DISTANCE4,
    // CLIP_DISTANCE5,
    // CLIP_DISTANCE6,
    // CLIP_DISTANCE7,
    // COLOR_LOGIC_OP,
    CULL_FACE,
    // DEBUG_OUTPUT,
    // DEBUG_OUTPUT_SYNCHRONOUS,
    // DEPTH_CLAMP,
    DEPTH_TEST,
    // DITHER,
    // FRAMEBUFFER_SRGB,
    // LINE_SMOOTH,
    // MULTISAMPLE,
    // POLYGON_OFFSET_FILL,
    // POLYGON_OFFSET_LINE,
    // POLYGON_OFFSET_POINT,
    // POLYGON_SMOOTH,
    // PRIMITIVE_RESTART,
    // PRIMITIVE_RESTART_FIXED_INDEX,
    // RASTERIZER_DISCARD,
    // SAMPLE_ALPHA_TO_COVERAGE,
    // SAMPLE_ALPHA_TO_ONE,
    // SAMPLE_COVERAGE,
    // SAMPLE_SHADING,
    // SAMPLE_MASK,
    // SCISSOR_TEST,
    // STENCIL_TEST,
    // TEXTURE_CUBE_MAP_SEAMLESS,
    // PROGRAM_POINT_SIZE,
    N_GLCAP
  };

  enum BufferTarget{
    ARRAY_BUFFER = 0,
    // ATOMIC_COUNTER_BUFFER,
    // COPY_READ_BUFFER,
    // COPY_WRITE_BUFFER,
    // DRAW_INDIRECT_BUFFER,
    // DISPATCH_INDIRECT_BUFFER,
    ELEMENT_ARRAY_BUFFER,
    // PIXEL_PACK_BUFFER,
    // PIXEL_UNPACK_BUFFER,
    // QUERY_BUFFER,
    // SHADER_STORAGE_BUFFER,
    // TEXTURE_BUFFER,
    // TRANSFORM_FEEDBACK_BUFFER,
    // UNIFORM_BUFFER
    N_BUFFER
  };


  /**
   * Functions to set state.
   *
   * All functions return true if they actually change the state, false if
   * NO-OP.
   */
  static void enable(Capability cap);
  static void disable(Capability cap);

  static void viewport(const Rect& viewport);
  static void activeTexture(GLenum activeTexture);
  static void useProgram(GLuint program);
  static void blendFunc(GLenum sfactor, GLenum dfactor);
  static void bindBuffer(BufferTarget target, GLuint buffer);
  static void bindTexture(GLenum target, GLuint buffer);
  static void bindFramebuffer(GLenum target, GLuint buffer);
  static void bindRenderbuffer(GLuint buffer);
  static void clearColor(float r, float g, float b, float a);

  static bool isEnabled(Capability cap);
  static Rect getViewport();
  static GLint getActiveTexture();
  static GLuint getProgram();


  /**
   * Queries and mirrors OpenGL state.
   * Call this once, after OpenGL context has been established.
   */
  static void syncronize();

  // Pass through functions
  static void bufferData(BufferTarget target, GLsizeiptr size,
                         const GLvoid * data, GLenum usage);
  static void bufferSubData(BufferTarget target, GLintptr offset,
                            GLsizeiptr size, const GLvoid * data);

  // Converters
  static GLenum toGLenum(Capability cap);
  static GLenum toGLenum(BufferTarget target);

private:
  static GlState& instance();
  void f_syncronize();

  void f_enable(Capability cap);
  void f_disable(Capability cap);
  void f_viewport(const Rect& viewport);
  void f_activeTexture(GLenum activeTexture);
  void f_useProgram(GLuint program);
  void f_blendFunc(GLenum sfactor, GLenum dfactor);
  void f_bindBuffer(BufferTarget target, GLuint buffer);
  void f_bindTexture(GLenum target, GLuint buffer);
  void f_bindFramebuffer(GLenum target, GLuint buffer);
  void f_bindRenderbuffer(GLuint buffer);
  void f_clearColor(float r, float g, float b, float a);

  /**
   * Functions to get state information.
   */
  bool f_isEnabled(Capability cap) const;
  Rect f_getViewport() const;
  GLint f_getActiveTexture() const;
  GLuint f_getProgram() const;


  Log log_;

  std::array<bool, N_GLCAP> capStates_;
  std::array<GLuint, N_BUFFER> bufferBindings_;

  static const std::array<GLenum, N_GLCAP> capEnumToGL_;
  static const std::array<GLenum, N_BUFFER> bufferEnumToGL_;

  Rect viewport_;
  GLenum activeTexture_;
  GLuint program_;
  GLenum blendSFactor_;
  GLenum blendDFactor_;

  float clearColor_[4];

  std::pair<GLenum, GLuint> framebufferBinding_;
  std::pair<GLenum, GLuint> textureBinding_;
  GLuint renderbufferBinding_;

  GlState();
  ~GlState();

  // NonCopyable
  GlState(const GlState& c);
  GlState& operator=(const GlState& c);
};

#endif
