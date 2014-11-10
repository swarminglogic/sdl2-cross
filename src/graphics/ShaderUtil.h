#ifndef GRAPHICS_SHADERUTIL_H
#define GRAPHICS_SHADERUTIL_H

#include <string>
#include <utility>
#include <vector>

#include <graphics/SDL_opengl.h>


typedef std::pair<GLenum, std::string> ShaderInfo;

/**
 * ShaderUtil class
 *
 * @author SwarmingLogic (Roald Fernandez)
 */
class ShaderUtil
{
 public:
  /**
   * Utility for checking OpenGL Shader information.
   *
   * @param shaderId    The shader to query
   * @param statusType  The status to query glGetShaderiv
   * @return String containing the debug information.
   */
  static std::string checkShaderInfo(GLuint shaderId,
                                     GLenum statusType = GL_COMPILE_STATUS);

  /**
   * Utility for checking OpenGL Program information.
   *
   * @param programId   The program to query.
   * @param statusType  The status to query, see glGetProgramiv
   * @return  String containing the debug information.
   */
  static std::string checkProgramInfo(GLuint programId,
                                      GLenum statusType = GL_LINK_STATUS);

 private:
  // Pure static class, private constructor
  ShaderUtil();
};

#endif  // GRAPHICS_SHADERUTIL_H
