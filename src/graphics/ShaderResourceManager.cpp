#include <graphics/ShaderResourceManager.h>

#include <util/FileUtil.h>


ShaderResourceManager::ShaderResourceManager()
  : log_("ShaderResourceManager")
{
}


ShaderResourceManager::~ShaderResourceManager()
{
}


ShaderProgramShPtr ShaderResourceManager::load(const ShaderKey& shaders)
{
  log_.d() << "Trying to load shader from:";
  for (auto& shader : shaders) {
    log_ << " " << shader.second.filename();
  }
  log_ << Log::end;

  ShaderProgramShPtr resource = loadedShaders_[shaders];
  if (!resource) {
    // Simple check to see that all files exist:
    for (auto& shader : shaders) {
      if (!FileUtil::exists(shader.second.path())) {
        resource.reset();
        log_.e() << "Failed to find shader file: "
                 << shader.second.path() << Log::end;
        return resource;
      }
    }
    resource.reset(new ShaderProgram(shaders));
    loadedShaders_[shaders] = resource;
  }
  return resource;
}



ShaderProgramShPtr ShaderResourceManager::loadShader(
      const ShaderfileKey& shaders)
{
  ShaderKey key;
  for (auto& shader : shaders)
    key[shader.first] = AssetShader(shader.second);
  return load(key);
}


ShaderResourceManager& ShaderResourceManager::instance()
{
  static ShaderResourceManager srm;
  return srm;
}
