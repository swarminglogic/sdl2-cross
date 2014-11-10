#include <graphics/Mesh.h>

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
    return static_cast<GLsizei>(shapes_[shapeIndex].indices.size());
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

GLuint Mesh::getTangentBuffer(size_t shapeIndex) const
{
  assert(shapeIndex < shapes_.size());
  return bufferIds_[shapeIndex].tangent;
}

GLuint Mesh::getBitangentBuffer(size_t shapeIndex) const
{
  assert(shapeIndex < shapes_.size());
  return bufferIds_[shapeIndex].bitangent;
}


bool Mesh::hasUVdata(size_t shapeIndex) const
{
  assert(shapeIndex < shapes_.size());
  return !shapes_[shapeIndex].texcoords.empty();
}


bool Mesh::hasNormalData(size_t shapeIndex) const
{
  assert(shapeIndex < shapes_.size());
  return !shapes_[shapeIndex].normals.empty();
}

bool Mesh::hasTangentSpace(size_t shapeIndex) const
{
  assert(shapeIndex < shapes_.size());
  return (hasNormalData(shapeIndex) &&
          !shapes_[shapeIndex].tangents.empty() &&
          !shapes_[shapeIndex].bitangents.empty());
}


bool Mesh::load()
{
  log_.i() << "Loading OBJ file: " << filename_.path() << Log::end;
  if (!FileUtil::exists(filename_)) {
    log_.e() << "Could not find OBJ file " << filename_.path() << Log::end;
    return false;
  }

  timer_.start();
  std::vector<tinyobj::shape_t> shapes = ObjUtil::read(filename_);

  // TODO swarminglogic, 2014-04-28: PORT
  // fileMonitor_.resetTimeStamp();
  unsigned int time = timer_.reset();
  log_.i() << "Finished loading. Spent "
           << static_cast<int>(time) << " ms." << Log::end;

  log_.d() << "Number of mesh objects: " << shapes.size() << Log::end;

  if (shapes.empty()) {
    log_.w() << "Failed to load mesh: " << filename_.path() << Log::end;
    return false;
  }

  // Move stuff
  shapes_.resize(shapes.size());
  for (size_t i = 0 ; i < shapes.size() ; ++i) {
    shapes_[i].positions = std::move(shapes[i].mesh.positions);
    shapes_[i].normals   = std::move(shapes[i].mesh.normals);
    shapes_[i].texcoords = std::move(shapes[i].mesh.texcoords);
    shapes_[i].indices   = std::move(shapes[i].mesh.indices);

    assert(shapes_[i].positions.size() == shapes_[i].normals.size());
    if (shapes_[i].texcoords.size() == 2 * shapes_[i].positions.size() / 3) {
      log_.d() << "Mesh [" << i << "]: Computing tangent basis" << Log::end;
      computeTangentBasis(shapes_[i]);
    }
  }

  clear();
  bufferIds_.resize(shapes_.size(), {0u, 0u, 0u, 0u, 0u, 0u});
  for (size_t i = 0 ; i < shapes_.size() ; ++i) {
    log_.d() << "Mesh [" << i << "]: "
             << "I:" << shapes_[i].indices.size() << "  "
             << "V:" << shapes_[i].positions.size() << "  "
             << "T:" << shapes_[i].texcoords.size() << "  "
             << "N:" << shapes_[i].normals.size()
             << Log::end;
    // Indices
    bufferIds_[i].index =
      GlUtil::prepareVertexBuffer(shapes_[i].indices,
                                  GlState::ELEMENT_ARRAY_BUFFER);

    // Vertices
    bufferIds_[i].vertex =
      GlUtil::prepareVertexBuffer(shapes_[i].positions);

    // UVs
    if (hasUVdata(i))
      bufferIds_[i].uv =
        GlUtil::prepareVertexBuffer(shapes_[i].texcoords);

    // Normals
    if (hasNormalData(i))
      bufferIds_[i].normal =
        GlUtil::prepareVertexBuffer(shapes_[i].normals);

    // Tangent space data
    if (hasTangentSpace(i)) {
      bufferIds_[i].tangent =
        GlUtil::prepareVertexBuffer(shapes_[i].tangents);
      bufferIds_[i].bitangent =
        GlUtil::prepareVertexBuffer(shapes_[i].bitangents);
    }
  }

  return true;
}

void Mesh::computeTangentBasis(MeshData& shape)
{
  const size_t nPositions = shape.normals.size();
  shape.tangents.resize(nPositions);
  shape.bitangents.resize(nPositions);

  const size_t vertexCount = shape.positions.size() / 3;
  const size_t triangleCount = shape.indices.size() / 3;

  const glm::uvec3* vTriangles =
    reinterpret_cast<glm::uvec3*>(&shape.indices[0]);
  const glm::vec3* vVertices =
    reinterpret_cast<glm::vec3*>(&shape.positions[0]);
  const glm::vec3* vNormals =
    reinterpret_cast<glm::vec3*>(&shape.normals[0]);
  const glm::vec2* vTexcoords =
    reinterpret_cast<glm::vec2*>(&shape.texcoords[0]);

  glm::vec3* vTangents = reinterpret_cast<glm::vec3*>(&shape.tangents[0]);
  glm::vec3* vBitangents = reinterpret_cast<glm::vec3*>(&shape.bitangents[0]);

  assert(vertexCount < 3 * shape.normals.size());
  assert(vertexCount < 3 * shape.tangents.size());
  assert(vertexCount < 3 * shape.bitangents.size());

  std::vector<glm::vec3> tan1(vertexCount, glm::vec3(0.0f, 0.0f, 0.0f));
  // std::vector<glm::vec3> tan2(vertexCount, glm::vec3(0.0f,0.0f,0.0f));

  for (size_t a = 0 ; a < triangleCount ; ++a) {
    const uint& i1 = vTriangles[a].x;
    const uint& i2 = vTriangles[a].y;
    const uint& i3 = vTriangles[a].z;

    assert(i1 < 3 * shape.positions.size());
    assert(i2 < 3 * shape.positions.size());
    assert(i3 < 3 * shape.positions.size());
    assert(i1 < 2 * shape.texcoords.size());
    assert(i2 < 2 * shape.texcoords.size());
    assert(i3 < 2 * shape.texcoords.size());

    const glm::vec3& v1 = vVertices[i1];
    const glm::vec3& v2 = vVertices[i2];
    const glm::vec3& v3 = vVertices[i3];

    const glm::vec2& w1 = vTexcoords[i1];
    const glm::vec2& w2 = vTexcoords[i2];
    const glm::vec2& w3 = vTexcoords[i3];

    const float x1 = v2.x - v1.x;
    const float x2 = v3.x - v1.x;
    const float y1 = v2.y - v1.y;
    const float y2 = v3.y - v1.y;
    const float z1 = v2.z - v1.z;
    const float z2 = v3.z - v1.z;

    const float s1 = w2.x - w1.x;
    const float s2 = w3.x - w1.x;
    const float t1 = w2.y - w1.y;
    const float t2 = w3.y - w1.y;

    const float r = 1.0f / (s1 * t2 - s2 * t1);
    glm::vec3 sdir((t2 * x1 - t1 * x2) * r,
                   (t2 * y1 - t1 * y2) * r,
                   (t2 * z1 - t1 * z2) * r);
    // glm::vec3 tdir((s1 * x2 - s2 * x1) * r,
    //                (s1 * y2 - s2 * y1) * r,
    //                (s1 * z2 - s2 * z1) * r);

    tan1[i1] += sdir;
    tan1[i2] += sdir;
    tan1[i3] += sdir;

    // tan2[i1] += tdir;
    // tan2[i2] += tdir;
    // tan2[i3] += tdir;
  }

  for (size_t a = 0; a < vertexCount; a++) {
    const glm::vec3& n = vNormals[a];
    const glm::vec3& t = tan1[a];
    vTangents[a] = glm::normalize(t - n * glm::dot(n, t));
    vBitangents[a] = glm::cross(n, vTangents[a]);

    // TODO swarminglogic, 2014-06-25: If handedness is necessary,
    // extend MeshData.tangents and MeshData.bitangets size to be
    // 4 * (MeshData.size() / 3), replace glm::vec3 with glm::vec4
    // And use the following:
    // vTangents[a].w = (glm::dot(glm::cross(n, t), tan2[a]) < 0.0f)
    //   ? -1.0f : 1.0f;
  }
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
    if (bufferIds_[i].tangent != 0)
      glDeleteBuffers(1, &bufferIds_[i].tangent);
    if (bufferIds_[i].bitangent != 0)
      glDeleteBuffers(1, &bufferIds_[i].bitangent);
  }

  bufferIds_.clear();
}

// TODO swarminglogic, 2014-04-28: PORT
// void Mesh::guard_isObjModified()
// {
//   cached_isObjModified_ = fileMonitor_.isUpdated();
// }
