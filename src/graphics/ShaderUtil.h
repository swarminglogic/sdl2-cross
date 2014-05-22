#ifndef GRAPHICS_SHADERUTIL_H
#define GRAPHICS_SHADERUTIL_H

#include <string>
#include <utility>
#include <vector>

#include <graphics/SDL_opengl.h>
#include <util/Log.h>


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
   * Creates a shader program using the provided.
   * Accepts pairs of shader types and corresponding source file paths.
   *
   * Lists must be the same size.
   * Errors and debug information is sent to cerr.
   *
   * @note Does not check for duplicate shader types.
   *
   * @param shaders  Vector of < SHADER-TYPE, SOURCE-FILE-PATH > pairs.
   * @return  Program id created.
   */
  static GLuint createShaderProgram(const std::vector<ShaderInfo>& shaders);

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
  /**
   * Creates a OpenGL shader of specified type, with provided source file.
   *
   * @param type         OpenGL shader type enum, e.g GL_VERTEX_SHADER
   * @param sourceFile   Full path to shader source file
   * @return             Id of created shader.
   */
  static GLuint prepareShader(GLenum type, const std::string& sourceFile);

  static Log log_;

  // Pure static class, private constructor
  ShaderUtil();
};

#endif
