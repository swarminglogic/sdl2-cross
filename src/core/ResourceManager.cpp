#include <core/ResourceManager.h>

#include <audio/AudioResourceManager.h>
#include <graphics/ImageResourceManager.h>
#include <graphics/MeshResourceManager.h>
#include <graphics/ShaderResourceManager.h>
#include <util/assert.h>


ResourceManager::ResourceManager()
  : audioResources_(new AudioResourceManager),
    imageResources_(new ImageResourceManager),
    meshResources_(new MeshResourceManager),
    shaderResources_(new ShaderResourceManager)
{
}


ResourceManager::~ResourceManager()
{
}


AudioResourceManager&  ResourceManager::audio()
{
  assert(audioResources_);
  return *audioResources_;
}

ImageResourceManager&  ResourceManager::image()
{
  assert(imageResources_);
  return *imageResources_;
}

MeshResourceManager&   ResourceManager::mesh()
{
  assert(meshResources_);
  return *meshResources_;
}

ShaderResourceManager& ResourceManager::shader()
{
  assert(shaderResources_);
  return *shaderResources_;
}


std::shared_ptr<AudioResourceManager>  ResourceManager::getAudio()
{
  return audioResources_;
}

std::shared_ptr<ImageResourceManager>  ResourceManager::getImage()
{
  return imageResources_;
}

std::shared_ptr<MeshResourceManager>   ResourceManager::getMesh()
{
  return meshResources_;
}

std::shared_ptr<ShaderResourceManager> ResourceManager::getShader()
{
  return shaderResources_;
}
