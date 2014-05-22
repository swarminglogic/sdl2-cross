#include <graphics/GlUtil.h>


GLuint GlUtil::allocateVertexBuffer(GLsizeiptr size,
                                    GlState::BufferTarget target,
                                    GLenum usage)
{
  GLuint buffername = 0;
  glGenBuffers(1, &buffername);
  GlState::bindBuffer(target, buffername);
  GlState::bufferData(target, size, nullptr, usage);
  return buffername;
}
