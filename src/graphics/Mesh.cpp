#include <graphics/Mesh.h>

#include <fstream>

#include <glm/glm.hpp>
#include <graphics/GlState.h>
#include <graphics/GlUtil.h>
#include <util/ObjUtil.h>
#include <util/FileUtil.h>
#include <util/StringUtil.h>


Mesh::Mesh(const AssetMesh& meshfile)
  : log_("Mesh"),
    filename_(meshfile),
    shapes_(),
    bufferIds_(),
    // fileMonitor_(meshfile.path()),
    // cached_isObjModified_(false),
    // guard_isObjModified_(std::bind(&Mesh::guard_isObjModified,
    //                                this),
    //                      1000),
    timer_()
{
}


Mesh::~Mesh()
{
  clear();
}


bool Mesh::isEmpty() const
{
  return shapes_.empty();
}


size_t Mesh::getShapeCount() const
{
  return shapes_.size();
}


GLsizei Mesh::getIndicesCount(size_t shapeIndex) const
{
  if (shapeIndex >= shapes_.size())
    return 0u;
  else
    return static_cast<GLsizei>(shapes_[shapeIndex].mesh.indices.size());
}


GLuint Mesh::getIndexBuffer(size_t shapeIndex) const
{
  assert(shapeIndex < shapes_.size());
  return bufferIds_[shapeIndex].index;
}


GLuint Mesh::getVertexBuffer(size_t shapeIndex) const
{
  assert(shapeIndex < shapes_.size());
  return bufferIds_[shapeIndex].vertex;
}


GLuint Mesh::getUVBuffer(size_t shapeIndex) const
{
  assert(shapeIndex < shapes_.size());
  return bufferIds_[shapeIndex].uv;
}


GLuint Mesh::getNormalBuffer(size_t shapeIndex) const
{
  assert(shapeIndex < shapes_.size());
  return bufferIds_[shapeIndex].normal;
}


const tinyobj::material_t& Mesh::getMaterial(size_t shapeIndex) const
{
  assert(shapeIndex < shapes_.size());
  return shapes_[shapeIndex].material;
}


bool Mesh::hasUVdata(size_t shapeIndex) const
{
  assert(shapeIndex < shapes_.size());
  return !shapes_[shapeIndex].mesh.texcoords.empty();
}


bool Mesh::hasNormalData(size_t shapeIndex) const
{
  assert(shapeIndex < shapes_.size());
  return !shapes_[shapeIndex].mesh.normals.empty();
}


bool Mesh::load()
{
  log_.i() << "Loading OBJ file: " << filename_.path() << Log::end;
  if (!FileUtil::exists(filename_.path())) {
    log_.e() << "Could not find OBJ file " << filename_.path() << Log::end;
    return false;
  }

  timer_.start();
  std::vector<tinyobj::shape_t> tempShapes = ObjUtil::read(filename_);

  // TODO swarminglogic, 2014-04-28: PORT
  // fileMonitor_.resetTimeStamp();
  unsigned int time = timer_.reset();
  log_.i() << "Finished loading. Spent " << (int)time << " ms." << Log::end;

  log_.d() << "Number of mesh objects: " << tempShapes.size() << Log::end;
  if (!tempShapes.empty()) {
    shapes_ = tempShapes;
    clear();
    bufferIds_.resize(shapes_.size(), {0, 0, 0, 0});
    for (size_t i = 0 ; i < shapes_.size() ; ++i) {
      log_.d() << "Mesh [" << i << "]: "
               << "I:" << shapes_[i].mesh.indices.size() << "  "
               << "V:" << shapes_[i].mesh.positions.size() << "  "
               << "T:" << shapes_[i].mesh.texcoords.size() << "  "
               << "N:" << shapes_[i].mesh.normals.size()
               << Log::end;
      // Indices
      bufferIds_[i].index =
        GlUtil::prepareVertexBuffer(shapes_[i].mesh.indices,
                                    GlState::ELEMENT_ARRAY_BUFFER);

      // Vertices
      bufferIds_[i].vertex =
        GlUtil::prepareVertexBuffer(shapes_[i].mesh.positions);

      // UVs
      if (hasUVdata(i))
        bufferIds_[i].uv =
          GlUtil::prepareVertexBuffer(shapes_[i].mesh.texcoords);

      // Normals
      if (hasNormalData(i))
        bufferIds_[i].normal =
          GlUtil::prepareVertexBuffer(shapes_[i].mesh.normals);
    }

  } else {
    log_.w() << "Failed to load mesh: " << filename_.path() << Log::end;
  }

  return !tempShapes.empty();
}


bool Mesh::isUpdated()
{
  // TODO swarminglogic, 2014-04-28: PORT
  // cached_isObjModified_ = false;
  // const bool wasExecuted = guard_isObjModified_.exec();
  // return wasExecuted && cached_isObjModified_;
  return true;
}

void Mesh::clear()
{
  for (size_t i = 0 ; i < bufferIds_.size() ; ++i) {
    if (bufferIds_[i].index  != 0)
      glDeleteBuffers(1, &bufferIds_[i].index);
    if (bufferIds_[i].vertex != 0)
      glDeleteBuffers(1, &bufferIds_[i].vertex);
    if (bufferIds_[i].uv     != 0)
      glDeleteBuffers(1, &bufferIds_[i].uv);
    if (bufferIds_[i].normal != 0)
      glDeleteBuffers(1, &bufferIds_[i].normal);
  }

  bufferIds_.clear();
}

// TODO swarminglogic, 2014-04-28: PORT
// void Mesh::guard_isObjModified()
// {
//   cached_isObjModified_ = fileMonitor_.isUpdated();
// }
