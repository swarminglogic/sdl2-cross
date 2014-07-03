#ifndef GRAPHICS_MESH_H
#define GRAPHICS_MESH_H

#include <array>
#include <memory>
#include <string>
#include <vector>

#include <extern/tiny_obj_loader.h>
#include <graphics/SDL_opengl.h>
#include <util/Asset.h>
#include <util/Log.h>
#include <util/TimedExecution.h>

// TODO swarminglogic, 2014-04-28: PORT
// #include <util/FileMonitor.h>

/**
 * Mesh class
 *
 * @author SwarmingLogic (Roald Fernandez)
 */
class Mesh
{
public:
  Mesh(const AssetMesh& meshfile);
  virtual ~Mesh();

  bool isEmpty() const;
  size_t getShapeCount() const;
  GLsizei getIndicesCount(size_t shapeIndex) const;
  GLuint getIndexBuffer(size_t shapeIndex) const;
  GLuint getVertexBuffer(size_t shapeIndex) const;
  GLuint getUVBuffer(size_t shapeIndex) const;
  GLuint getNormalBuffer(size_t shapeIndex) const;
  GLuint getTangentBuffer(size_t shapeIndex) const;
  GLuint getBitangentBuffer(size_t shapeIndex) const;

  bool load();
  bool isUpdated();

  bool hasUVdata(size_t shapeIndex) const;
  bool hasNormalData(size_t shapeIndex) const;
  bool hasTangentSpace(size_t shapeIndex) const;

private:
  struct BufferGroup
  {
    GLuint index;
    GLuint vertex;
    GLuint uv;
    GLuint normal;
    GLuint tangent;
    GLuint bitangent;
  };

  struct MeshData
  {
    // nVertices = nTriangles * 3
    std::vector<unsigned int> indices; // nTriangles * 3
    std::vector<float> positions;      // nVertices * 3
    std::vector<float> texcoords;      // nVertices * 2
    std::vector<float> normals;        // nVertices * 3
    std::vector<float> tangents;       // nVertices * 3
    std::vector<float> bitangents;     // nVertices * 3
  };

  void clear();
  static void computeTangentBasis(MeshData& shape);

  Log log_;
  AssetMesh filename_;

  std::vector<MeshData> shapes_;
  std::vector<BufferGroup> bufferIds_;

  // TODO swarminglogic, 2014-04-28: PORT
  // FileMonitor fileMonitor_;

  // TODO swarminglogic, 2014-04-28: PORT
  // // Using TimedExecution as local guard.
  // // Since it only accepts void(void), I'm
  // // caching the result in member variable 'cached_isShadersModified_'.
  // void guard_isObjModified();
  // bool cached_isObjModified_;
  // TimedExecution guard_isObjModified_;

  Timer timer_;

  // NonCopyable
  Mesh(const Mesh& c);
  Mesh& operator=(const Mesh& c);
};

typedef std::shared_ptr<Mesh> MeshShPtr;

#endif
