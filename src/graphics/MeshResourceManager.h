#ifndef GRAPHICS_MESHRESOURCEMANAGER_H
#define GRAPHICS_MESHRESOURCEMANAGER_H

#include <map>
#include <memory>
#include <set>
#include <string>

#include <util/Asset.h>
#include <util/Log.h>


class Mesh;


/**
 * MeshResourceManager class.
 *
 * @author SwarmingLogic
 */
class MeshResourceManager
{
 public:
  typedef std::shared_ptr<Mesh> MeshShPtr;

  MeshResourceManager();
  virtual ~MeshResourceManager();

  MeshShPtr load(const AssetMesh& mesh);
  MeshShPtr loadMesh(const std::string& meshfile);

 private:
  Log log_;
  std::map<AssetMesh,  MeshShPtr> loadedMeshes_;

  // NonCopyable
  MeshResourceManager(const MeshResourceManager& c);
  MeshResourceManager& operator=(const MeshResourceManager& c);
};

#endif  // GRAPHICS_MESHRESOURCEMANAGER_H
