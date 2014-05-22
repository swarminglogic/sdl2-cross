#ifndef GRAPHICS_SHADERRESOURCEMANAGER_H
#define GRAPHICS_SHADERRESOURCEMANAGER_H

#include <iosfwd>
#include <map>
#include <memory>

#include <graphics/ShaderProgram.h>
#include <unordered_map>
#include <util/Asset.h>
#include <util/Log.h>


/**
 * ShaderResourceManager class.
 *
 * Manages audio resource files, making sure that only one
 * file is loaded to memory at one point.
 *
 * Future:
 *
 * // TODO swarminglogic, 2014-01-27:
 * // 1: Extend to asynchronously load files, in a non-blocking way.
 * // 2: Maintain a resource alive even though single user abandoned it.
 * // 3: Check if resource has been modified. Request file.
 * //    This does not alter current memory pointed to, unless all users have
 * //    abandoned old content. What is the GUID then? File path not enough.
 * //    Is this also only useful when developing?
 * //
 *
 * @author SwarmingLogic (Roald Fernandez)
 */
class ShaderResourceManager
{
public:
  typedef std::map<ShaderProgram::ShaderType, AssetShader> ShaderKey;
  typedef std::map<ShaderProgram::ShaderType, std::string> ShaderfileKey;

  ShaderResourceManager();
  virtual ~ShaderResourceManager();

  ShaderProgramShPtr load(const ShaderKey& shaders);
  ShaderProgramShPtr loadShader(const ShaderfileKey& shaders);

private:
  Log log_;
  std::map<ShaderKey,  ShaderProgramShPtr> loadedShaders_;

  // NonCopyable
  ShaderResourceManager(const ShaderResourceManager& c);
  ShaderResourceManager& operator=(const ShaderResourceManager& c);
};

#endif
