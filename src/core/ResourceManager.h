#ifndef CORE_RESOURCEMANAGER_H
#define CORE_RESOURCEMANAGER_H

#include <memory>


class AudioResourceManager;
class ImageResourceManager;
class MeshResourceManager;
class ShaderResourceManager;

/**
 * ResourceManager class.
 *
 * Convenience class for grouping the various ResourceManagers
 *
 * @author SwarmingLogic
 */
class ResourceManager
{
public:
  ResourceManager();
  ~ResourceManager();

  // Direct access to managers
  AudioResourceManager&  audio();
  ImageResourceManager&  image();
  MeshResourceManager&   mesh();
  ShaderResourceManager& shader();

  // For sharing with various core systems.
  std::shared_ptr<AudioResourceManager>  getAudio();
  std::shared_ptr<ImageResourceManager>  getImage();
  std::shared_ptr<MeshResourceManager>   getMesh();
  std::shared_ptr<ShaderResourceManager> getShader();

private:
  std::shared_ptr<AudioResourceManager>  audioResources_;
  std::shared_ptr<ImageResourceManager>  imageResources_;
  std::shared_ptr<MeshResourceManager>   meshResources_;
  std::shared_ptr<ShaderResourceManager> shaderResources_;


  // NonCopyable
  ResourceManager(const ResourceManager& c);
  ResourceManager& operator=(const ResourceManager& c);
};

#endif
