#include <graphics/ShaderUtil.h>

#include <array>
#include <memory>
#include <sstream>

#include <util/FileInfo.h>
#include <util/FileUtil.h>


std::string ShaderUtil::checkShaderInfo(GLuint shaderId,
                                        GLenum statusType)
{
  GLint result = GL_TRUE;
  glGetShaderiv(shaderId, statusType, &result);
  if (GL_FALSE != result) {
    return std::string("");
  } else {
    std::stringstream ss;
    ss << "Shader error ocurred\n";
    GLint loglen;
    glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &loglen);
    if (loglen > 0) {
      std::unique_ptr<char[]> log(new char[loglen]);
      GLsizei written;
      glGetShaderInfoLog(shaderId, loglen, &written, log.get());
      ss << log.get() << std::endl;
    }
    return ss.str();
  }
}


std::string ShaderUtil::checkProgramInfo(GLuint programId,
                                         GLenum statusType)
{
  GLint result = GL_TRUE;
  glGetProgramiv(programId, statusType, &result);
  if (GL_FALSE != result) {
    return std::string("");
  } else {
    std::stringstream ss;
    ss << "Program error ocurred\n";
    GLint loglen;
    glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &loglen);
    if (loglen > 0) {
      std::unique_ptr<char[]> log(new char[loglen]);
      GLsizei written;
      glGetProgramInfoLog(programId, loglen, &written, log.get());
      ss << log.get() << std::endl;
    }
    return ss.str();
  }
}

