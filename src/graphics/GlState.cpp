#include <graphics/GlState.h>

#include <graphics/GlCheck.h>
#include <util/assert.h>


const std::array<GLenum, GlState::N_GLCAP> GlState::capEnumToGL_
{GL_BLEND, GL_CULL_FACE, GL_DEPTH_TEST};
// , GL_DEPTH_CLAMP

const std::array<GLenum, GlState::N_BUFFER> GlState::bufferEnumToGL_
{GL_ARRAY_BUFFER, GL_ELEMENT_ARRAY_BUFFER};

GlState::GlState()
  : log_("GlState"),
    capStates_(),
    bufferBindings_{0},
    viewport_(Rect(0, 0, 0, 0)),
    activeTexture_(GL_TEXTURE0),
    program_(0),
    blendSFactor_(0),
    blendDFactor_(0)
{}


GlState::~GlState()
{}


GlState& GlState::instance()
{
  static GlState instance;
  return instance;
}


void GlState::syncronize()
{
  instance().f_syncronize();
}


void GlState::enable(Capability cap)
{ instance().f_enable(cap);}
void GlState::disable(Capability cap)
{ instance().f_disable(cap);}
void GlState::viewport(const Rect& vp)
{ instance().f_viewport(vp);}
void GlState::activeTexture(GLenum activeTex)
{ instance().f_activeTexture(activeTex);}
void GlState::useProgram(GLuint program)
{ instance().f_useProgram(program);}
void GlState::blendFunc(GLenum sfactor, GLenum dfactor)
{ instance().f_blendFunc(sfactor, dfactor);}
void GlState::bindBuffer(BufferTarget target, GLuint buffer)
{ instance().f_bindBuffer(target, buffer);}
void GlState::bindTexture(GLenum target, GLuint buffer)
{ instance().f_bindTexture(target, buffer);}
void GlState::bindFramebuffer(GLenum target, GLuint buffer)
{ instance().f_bindFramebuffer(target, buffer);}
void GlState::bindRenderbuffer(GLuint buffer)
{ instance().f_bindRenderbuffer(buffer);}
void GlState::clearColor(float r, float g, float b, float a)
{ instance().f_clearColor(r, g, b, a); }



bool GlState::isEnabled(Capability cap)
{return instance().f_isEnabled(cap);}
Rect GlState::getViewport()
{return instance().f_getViewport();}
GLint GlState::getActiveTexture()
{return instance().f_getActiveTexture();}
GLuint GlState::getProgram()
{return instance().f_getProgram();}

void GlState::f_syncronize()
{
  CHECKGL();
  GLint vp[4] {0};
  glGetIntegerv(GL_VIEWPORT, vp);
  viewport_ = Rect(vp[0], vp[1], vp[2], vp[3]);


  // TODO swarminglogic, 2014-06-12: use glGet
  for (size_t i = 0 ; i < 4u ; ++i)
    clearColor_[i] = 0.0f;

  CHECKGL();
}


void GlState::f_enable(Capability cap)
{
  CHECKGL();
  assert(cap != N_GLCAP);
#ifndef GL_STATE_GUARD_DISABLED
  if (!capStates_[cap]) {
#endif
    glEnable(capEnumToGL_[cap]);
#ifndef GL_STATE_GUARD_DISABLED
    capStates_[cap] = true;
  }
#endif
  CHECKGL();
}


void GlState::f_disable(Capability cap)
{
  CHECKGL();
  assert(cap != N_GLCAP);
#ifndef GL_STATE_GUARD_DISABLED
  if (capStates_[cap]) {
#endif
    glDisable(capEnumToGL_[cap]);
#ifndef GL_STATE_GUARD_DISABLED
    capStates_[cap] = false;
  }
#endif
  CHECKGL();
}


void GlState::f_viewport(const Rect& vp)
{
  CHECKGL();
#ifndef GL_STATE_GUARD_DISABLED
  if (vp != viewport_) {
#endif
    glViewport(vp.x(), vp.y(), vp.w(), vp.h());
#ifndef GL_STATE_GUARD_DISABLED
    viewport_ = vp;
  }
#endif
  CHECKGL();
}


void GlState::f_activeTexture(GLenum activeTex)
{
  CHECKGL();
#ifndef GL_STATE_GUARD_DISABLED
  if (activeTex != activeTexture_) {
#endif
    glActiveTexture(activeTex);
#ifndef GL_STATE_GUARD_DISABLED
    activeTexture_ = activeTex;
  }
#endif
  CHECKGL();
}


void GlState::f_useProgram(GLuint program)
{
  CHECKGL();
#ifndef GL_STATE_GUARD_DISABLED
  if (program != program_) {
#endif
    glUseProgram(program);
#ifndef GL_STATE_GUARD_DISABLED
    program_ = program;
  }
#endif
  CHECKGL();
}


void GlState::f_blendFunc(GLenum sfactor, GLenum dfactor)
{
  CHECKGL();
#ifndef GL_STATE_GUARD_DISABLED
  if (sfactor != blendSFactor_ ||
      dfactor != blendDFactor_) {
#endif
    glBlendFunc(sfactor, dfactor);
#ifndef GL_STATE_GUARD_DISABLED
    blendSFactor_ = sfactor;
    blendDFactor_ = dfactor;
  }
#endif
  CHECKGL();
}


void GlState::f_bindBuffer(BufferTarget target, GLuint buffer)
{
  CHECKGL();
#ifndef GL_STATE_GUARD_DISABLED
  if (bufferBindings_[target] != buffer) {
#endif
    glBindBuffer(bufferEnumToGL_[target], buffer);
#ifndef GL_STATE_GUARD_DISABLED
    bufferBindings_[target ] = buffer;
  }
#endif
  CHECKGL();
}


void GlState::f_bindTexture(GLenum target, GLuint buffer)
{
  CHECKGL();
#ifndef GL_STATE_GUARD_DISABLED
  if (textureBinding_.first  != target ||
      textureBinding_.second != buffer) {
#endif
    glBindTexture(target, buffer);
#ifndef GL_STATE_GUARD_DISABLED
    textureBinding_.first  = target;
    textureBinding_.second = buffer;
  }
#endif
  CHECKGL();
}


void GlState::f_bindFramebuffer(GLenum target, GLuint buffer)
{
  assert(target == GL_DRAW_FRAMEBUFFER ||
         target == GL_READ_FRAMEBUFFER ||
         target == GL_FRAMEBUFFER);
  CHECKGL();
#ifndef GL_STATE_GUARD_DISABLED
  if (framebufferBinding_.first  != target ||
      framebufferBinding_.second != buffer) {
#endif
    glBindFramebuffer(target, buffer);
#ifndef GL_STATE_GUARD_DISABLED
    framebufferBinding_.first = target;
    framebufferBinding_.second = buffer;
  }
#endif
  CHECKGL();
}



void GlState::f_bindRenderbuffer(GLuint buffer)
{
  CHECKGL();
#ifndef GL_STATE_GUARD_DISABLED
  if (buffer != renderbufferBinding_) {
#endif
    glBindRenderbuffer(GL_RENDERBUFFER, buffer);
#ifndef GL_STATE_GUARD_DISABLED
    renderbufferBinding_ = buffer;
  }
#endif
  CHECKGL();
}

void GlState::f_clearColor(float r, float g, float b, float a)
{
  CHECKGL();
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wfloat-equal"
#ifndef GL_STATE_GUARD_DISABLED
  if (clearColor_[0] != r ||
      clearColor_[1] != g ||
      clearColor_[2] != b ||
      clearColor_[3] != a) {
#endif
    glClearColor(r, g, b, a);
#ifndef GL_STATE_GUARD_DISABLED
    clearColor_[0] = r;
    clearColor_[1] = g;
    clearColor_[2] = b;
    clearColor_[3] = a;
  }
#endif
#pragma GCC diagnostic pop
  CHECKGL();
}



bool GlState::f_isEnabled(Capability cap) const
{
#ifndef GL_STATE_GUARD_DISABLED
  return capStates_[cap];
#else
  return glIsEnabled(capEnumToGL_[cap]);
#endif
}


Rect GlState::f_getViewport() const
{
#ifndef GL_STATE_GUARD_DISABLED
  return viewport_;
#else
  GLint vp[4] {0, 0, 0, 0};
  glGetIntegerv(GL_VIEWPORT, vp);
  return Rect(vp[0], vp[1], vp[2], vp[3]);
#endif
}


GLint GlState::f_getActiveTexture() const
{
#ifndef GL_STATE_GUARD_DISABLED
  return activeTexture_;
#else
  GLint val = 0;
  glGetIntegerv(GL_ACTIVE_TEXTURE, &val);
  return val;
#endif
}


GLuint GlState::f_getProgram() const
{
#ifndef GL_STATE_GUARD_DISABLED
  return program_;
#else
  GLint val = 0;
  glGetIntegerv(GL_CURRENT_PROGRAM, &val);
  return val;
#endif
}


// Pass-through functions
void GlState::bufferData(BufferTarget target,
                         GLsizeiptr size,
                         const GLvoid * data,
                         GLenum usage)
{
  glBufferData(bufferEnumToGL_[target], size, data, usage);
}

void GlState::bufferSubData(BufferTarget target, GLintptr offset,
                            GLsizeiptr size, const GLvoid * data)
{
  glBufferSubData(bufferEnumToGL_[target], offset, size, data);
}



GLenum GlState::toGLenum(Capability cap)
{
  return capEnumToGL_[cap];
}

GLenum GlState::toGLenum(BufferTarget target)
{
  return bufferEnumToGL_[target];
}
