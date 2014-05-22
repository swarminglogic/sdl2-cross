#include <graphics/GlState.h>

#include <cassert>


const std::array<GLenum, GlState::N_GLCAP> GlState::capEnumToGL_
{GL_BLEND, GL_CULL_FACE, GL_DEPTH_TEST};
// , GL_DEPTH_CLAMP

const std::array<GLenum, GlState::N_BUFFER> GlState::bufferEnumToGL_
{GL_ARRAY_BUFFER, GL_ELEMENT_ARRAY_BUFFER};

GlState::GlState()
  : log_("GlState"),
    capStates_(),
    bufferBindings_{0},
    viewport_(Rect(0,0, 0,0)),
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


bool GlState::enable(Capability cap)
{return instance().f_enable(cap);}
bool GlState::disable(Capability cap)
{return instance().f_disable(cap);}
bool GlState::viewport(const Rect& vp)
{return instance().f_viewport(vp);}
bool GlState::activeTexture(GLenum activeTex)
{return instance().f_activeTexture(activeTex);}
bool GlState::useProgram(GLuint program)
{return instance().f_useProgram(program);}
bool GlState::blendFunc(GLenum sfactor, GLenum dfactor)
{return instance().f_blendFunc(sfactor, dfactor);}
bool GlState::bindBuffer(BufferTarget target, GLuint buffer)
{return instance().f_bindBuffer(target, buffer);}
bool GlState::bindTexture(GLenum target, GLuint buffer)
{return instance().f_bindTexture(target, buffer);}
bool GlState::bindFramebuffer(GLenum target, GLuint buffer)
{return instance().f_bindFramebuffer(target, buffer);}
bool GlState::bindRenderbuffer(GLuint buffer)
{return instance().f_bindRenderbuffer(buffer);}



bool GlState::isEnabled(Capability cap)
{return instance().f_isEnabled(cap);}
Rect GlState::getViewport()
{return instance().f_getViewport();}
GLenum GlState::getActiveTexture()
{return instance().f_getActiveTexture();}
GLuint GlState::getProgram()
{return instance().f_getProgram();}


void GlState::f_syncronize()
{
  GLint vp[4] {0};
  glGetIntegerv(GL_VIEWPORT, vp);
  viewport_ = Rect(vp[0], vp[1], vp[2], vp[3]);
}


bool GlState::f_enable(Capability cap)
{
  assert(cap != N_GLCAP);
  if (!capStates_[cap]) {
    glEnable(capEnumToGL_[cap]);
    capStates_[cap] = true;
    return true;
  }
  return false;
}


bool GlState::f_disable(Capability cap)
{
  assert(cap != N_GLCAP);
  if (capStates_[cap]) {
    glDisable(capEnumToGL_[cap]);
    capStates_[cap] = false;
    return true;
  }
  return false;
}


bool GlState::f_viewport(const Rect& vp)
{
  const bool change = vp != viewport_;
  if (change) {
    glViewport(vp.x(), vp.y(), vp.w(), vp.h());
    viewport_ = vp;
  }
  return change;
}


bool GlState::f_activeTexture(GLenum activeTex)
{
  const bool change = activeTex != activeTexture_;
  if (change) {
    glActiveTexture(activeTex);
    activeTexture_ = activeTex;
  }
  return change;
}


bool GlState::f_useProgram(GLuint program)
{
  const bool change = program != program_;
  if (change) {
    glUseProgram(program);
    program_ = program;
  }
  return change;
}


bool GlState::f_blendFunc(GLenum sfactor, GLenum dfactor)
{
  const bool change = sfactor != blendSFactor_ || dfactor != blendDFactor_;
  if (change) {
    glBlendFunc(sfactor, dfactor);
    blendSFactor_ = sfactor;
    blendDFactor_ = dfactor;
  }
  return change;
}


bool GlState::f_bindBuffer(BufferTarget target, GLuint buffer)
{
  const bool change = bufferBindings_[target] != buffer;
  if (change) {
    glBindBuffer(bufferEnumToGL_[target], buffer);
    bufferBindings_[target ] = buffer;
  }
  return change;
}


bool GlState::f_bindTexture(GLenum target, GLuint buffer)
{
  const bool change = (textureBinding_.first != target ||
                       textureBinding_.second != buffer);
  if (change) {
    glBindTexture(target, buffer);
    textureBinding_.first  = target;
    textureBinding_.second = buffer;
  }
  return change;
}


bool GlState::f_bindFramebuffer(GLenum target, GLuint buffer)
{
  assert(target == GL_DRAW_FRAMEBUFFER ||
         target == GL_READ_FRAMEBUFFER ||
         target == GL_FRAMEBUFFER);

  const bool change = (framebufferBinding_.first != target ||
                       framebufferBinding_.second != buffer);
  if (change) {
    glBindFramebuffer(target, buffer);
    framebufferBinding_.first = target;
    framebufferBinding_.second = buffer;
  }

  return change;
}


bool GlState::f_bindRenderbuffer(GLuint buffer)
{
  const bool change = buffer != renderbufferBinding_;
  if (change) {
    glBindRenderbuffer(GL_RENDERBUFFER, buffer);
    renderbufferBinding_ = buffer;
  }
  return change;
}


bool GlState::f_isEnabled(Capability cap) const
{return capStates_[cap];}
Rect GlState::f_getViewport() const
{return viewport_;}
GLenum GlState::f_getActiveTexture() const
{return activeTexture_;}
GLuint GlState::f_getProgram() const
{return program_;}


// Pass-through functions
void GlState::bufferData(BufferTarget target,
                         GLsizeiptr size,
                         const GLvoid * data,
                         GLenum usage)
{glBufferData(bufferEnumToGL_[target], size, data, usage);}

void GlState::bufferSubData(BufferTarget target, GLintptr offset,
                            GLsizeiptr size, const GLvoid * data)
{glBufferSubData(bufferEnumToGL_[target], offset, size, data);}


GLenum GlState::toGLenum(Capability cap)
{
  return capEnumToGL_[cap];
}

GLenum GlState::toGLenum(BufferTarget target)
{
  return bufferEnumToGL_[target];
}
