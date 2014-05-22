#include <graphics/MeshResourceManager.h>

#include <graphics/Mesh.h>
#include <util/FileUtil.h>


MeshResourceManager::MeshResourceManager()
  : log_("MeshResourceManager")
{
}


MeshResourceManager::~MeshResourceManager()
{
}


MeshShPtr MeshResourceManager::load(const AssetMesh& mesh)
{
  log_.d() << "Trying to load mesh: " << mesh.path() << Log::end;
  MeshShPtr resource = loadedMeshes_[mesh];
  if (!resource) {
    if (!FileUtil::exists(mesh.path())) {
      resource.reset();
    } else {
      resource.reset(new Mesh(mesh));
      loadedMeshes_[mesh] = resource;
    }
  }
  return resource;
}

MeshShPtr MeshResourceManager::loadMesh(const std::string&  meshfile)
{
  return load(AssetMesh(meshfile));
}
