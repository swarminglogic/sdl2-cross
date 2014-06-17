#include <graphics/GlCheck.h>


#define STATUS_CASE(enum) case enum: return #enum
const char* GlCheck::glStatusString(GLenum error)
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
#undef STATUS_CASE

bool GlCheck::checkGlFramebuffer()
{
  Log log("CheckGL");
  if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
    const GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status == GL_FRAMEBUFFER_UNDEFINED)
      log.e() << "Framebuffer Status: Undefined. No window?" << Log::end;
    if (status == GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT)
      log.e() << "Framebuffer Status: Incomplete Attachment. "
              << "Check attachment status." << Log::end;
    else if (status == GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT)
      log.e() << "Framebuffer Status: Incomplete Missing Attachment. "
              << "Attach at least one buffer to FBO" << Log::end;
#ifndef __ANDROID__
    else if (status == GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER)
      log.e() << "Framebuffer Status: Incomplete Draw Buffer. "
              << "Check attachments enabled via glDrawBuffers are OK"
              << Log::end;
    else if (status == GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER)
      log.e() << "Framebuffer Status: Incomplete Read Buffer. "
              << "Check attachments enabled via glReadBuffer are OK"
              << Log::end;
    else if (status == GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS)
      log.e() << "Framebuffer Status: Incomplete Layer Targets. "
              << "Make sure number of layers is the same for each attachment"
              << Log::end;
#endif
    else if (status == GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE)
      log.e() << "Framebuffer Status: Incomplete Multisample. "
              << "Make sure number of samples is the same for each attachment"
              << Log::end;
    else if (status == GL_FRAMEBUFFER_UNSUPPORTED)
      log.e() << "Framebuffer Status: Unsupported. Formats might be bad."
              << Log::end;
    else
      log.e() << "Framebuffer Status: ??" << Log::end;

    log.exception("Framebuffer is not ok.");
    return false;
  }
  return true;
}
