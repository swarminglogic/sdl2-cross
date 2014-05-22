#ifndef GRAPHICS_SHADERPROGRAM_H
#define GRAPHICS_SHADERPROGRAM_H

#include <map>

#include <graphics/ShaderUtil.h>
#include <util/Asset.h>
#include <util/Log.h>
#include <util/TimedExecution.h>


class File;

/**
 * ShaderProgram class.
 * For managing a shader program, and rebuilding.
 *
 * Only compile() actually modifies the stored compiled shader program.
 * clear(), setShader, removeShader, just configure the shader, prior to
 * compilation.
 *
 * @author SwarmingLogic (Roald Fernandez)
 */
class ShaderProgram
{
public:
  enum ShaderType{
    VERTEX,
    // CONTROL,
    // EVAL,
    // GEOMETRY,
    FRAGMENT
  };

  ShaderProgram();
  ShaderProgram(const std::map<ShaderType, AssetShader>& shaders);

  virtual ~ShaderProgram();

  GLuint get() const;

  void clear();
  void setShader(ShaderType type, const AssetShader& shaderfile);
  void setShader(const std::map<ShaderType, AssetShader>& shaders);

  /**
   * Deletes the registered
   * @param type
   */
  void removeShader(ShaderType type);

  /**
   * Checks if shader files have been modified.
   *
   * This function is protected by Timer interval, and can be called
   * frequently, at low cost.
   */
  bool isModified();

  /**
   * Destroys existing program, reads files, and compiles shaders.
   * Returns true if compilation successful, false otherwise.
   */
  bool compile();

private:
  void deleteProgram();
  bool hasShader(ShaderType type) const;

  static GLenum shaderType2GLEnum(ShaderType type);
  GLuint prepareShader(ShaderType type,
                       const std::string& source) const;

  Log log_;

  std::map<ShaderType, File> shaders_;
  bool isShaderInformationModified_;
  GLuint programId_;

  // Using TimedExecution as local guard.  Since it only accepts void(void), I'm
  // caching the result in member variable 'cached_isShadersModified_'.
  void guard_isShaderModified();
  bool cached_isShadersModified_;
  TimedExecution guard_isShaderModified_;

  // NonCopyable
  ShaderProgram(const ShaderProgram& c);
  ShaderProgram& operator=(const ShaderProgram& c);
};

typedef std::shared_ptr<ShaderProgram> ShaderProgramShPtr;
#endif
