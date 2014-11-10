#include <graphics/ShaderProgram.h>

#include <functional>

#include <util/Assert.h>
#include <util/File.h>
#include <util/StringUtil.h>


ShaderProgram::ShaderProgram()
  : ShaderProgram(std::map<ShaderType, AssetShader>{})
{
}


ShaderProgram::ShaderProgram(const std::map<ShaderType, AssetShader>& shaders)
  : log_("ShaderProgram"),
    shaders_(),
    isShaderInformationModified_(true),
    programId_(0),
    cached_isShadersModified_(false),
    guard_isShaderModified_(std::bind(&ShaderProgram::guard_isShaderModified,
                                      this),
                            1000)
{
  setShader(shaders);
}


ShaderProgram::~ShaderProgram()
{
  deleteProgram();
}



GLuint ShaderProgram::getGlId() const
{
  return programId_;
}


void ShaderProgram::clear()
{
  shaders_.clear();
  isShaderInformationModified_ = true;
}


void ShaderProgram::setShader(ShaderType type, const AssetShader& shaderfile)
{
  shaders_[type] = File(shaderfile.path(), FileInfo::TYPE_ASSET);
  isShaderInformationModified_ = true;
  shaders_[type].update();
}

void ShaderProgram::setShader(const std::map<ShaderType, AssetShader>& shaders)
{
  for (auto& shader : shaders)
    setShader(shader.first, shader.second);
}



void ShaderProgram::removeShader(ShaderType type)
{
  if (hasShader(type))
    shaders_.erase(type);
}


bool ShaderProgram::isModified()
{
  if (isShaderInformationModified_) return true;

  cached_isShadersModified_ = false;
  const bool wasExecuted = guard_isShaderModified_.exec();
  return wasExecuted && cached_isShadersModified_;
}


void ShaderProgram::guard_isShaderModified()
{
  bool isMod = false;
  for (auto& shader : shaders_)
    isMod |= shader.second.isUpdated();
  cached_isShadersModified_ = isMod;
}


bool ShaderProgram::compile()
{
  for (auto& shader : shaders_) {
    if (!shader.second.exists())
      return false;
  }
  log_.i("Creating shader program from shader files.");

  GLuint newProgramId = glCreateProgram();

  // Compile shaders.
  std::vector<GLuint> shaderIds;
  for (auto& shader : shaders_) {
    if (shader.second.isUpdated()) {
      log_.i() << "Loading shader file: "
               << shader.second.getFilename() << Log::end;
      shader.second.update();
    }

    GLuint shaderId = prepareShader(shader.first,
                                    shader.second.getLocalCopy());

    if (shaderId != 0) {
      shaderIds.push_back(shaderId);
      glAttachShader(newProgramId, shaderId);
    // ERROR HANDLING
    } else {
      log_.e() << "Failed to compile shader: "
               << shader.second.getFilename() << Log::end;
      // Delete all shaders compiled up until this point.
      for (auto id : shaderIds)
        glDeleteShader(id);
      return false;
    }
  }

  glLinkProgram(newProgramId);

  // ERROR HANDLING
  std::string linkInfo = ShaderUtil::checkProgramInfo(newProgramId);
  if (!linkInfo.empty()) {
    log_.e() << "Program Link Info: \n";
    for (auto& shader : shaders_) {
      log_ << "  - " << shader.second.getFilename() << "\n";
    }
    log_ << linkInfo << Log::end;
    for (auto id : shaderIds)
      glDeleteShader(id);
    glDeleteProgram(newProgramId);
    return false;
  }

  // CLEANUP
  deleteProgram();
  programId_ = newProgramId;
  for (auto id : shaderIds) {
    glDetachShader(programId_, id);
    glDeleteShader(id);
  }

  isShaderInformationModified_ = false;
  return true;
}


bool ShaderProgram::hasShader(ShaderType type) const
{
  return shaders_.find(type) != shaders_.end();
}


void ShaderProgram::deleteProgram()
{
  if (programId_ > 0)
    glDeleteProgram(programId_);
  programId_ = 0;
}


GLuint ShaderProgram::prepareShader(ShaderType type,
                                    const std::string& source) const
{
  assert(!source.empty() && "Trying to prepare empty shader data");
#ifdef USE_OPENGLES
  std::string preprocessed = "#version 300 es\n";
  preprocessed.append(StringUtil::processIfEndif(source, "ES"));
#else
  std::string preprocessed = "#version 430 core\n";
  preprocessed.append(StringUtil::processIfEndif(source, "GL"));
#endif
  GLenum glShaderType = shaderType2GLEnum(type);
  const GLchar* shader_source = preprocessed.c_str();
  GLuint shaderId = glCreateShader(glShaderType);
  glShaderSource(shaderId, 1, &shader_source, NULL);
  glCompileShader(shaderId);

  // Check for errors.
  std::string compileInfo =
    ShaderUtil::checkShaderInfo(shaderId);
  if (!compileInfo.empty()) {
    log_.e() << "Shader Compile Info: \n" << compileInfo << Log::end;
  }
  return shaderId;
}


GLenum ShaderProgram::shaderType2GLEnum(ShaderType type)
{
  switch (type) {
  case VERTEX:
    return GL_VERTEX_SHADER;
  // case CONTROL:
  //   return GL_TESS_CONTROL_SHADER;
  // case EVAL:
  //   return GL_TESS_EVALUATION_SHADER;
  // case GEOMETRY:
  //   return GL_GEOMETRY_SHADER;
  case FRAGMENT:
    return GL_FRAGMENT_SHADER;
  default:
    return 0;
  }
}
