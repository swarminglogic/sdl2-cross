#include <util/CObjUtil.h>

#include <util/Assert.h>


CObjUtil::CObjUtil()
{
}


CObjUtil::~CObjUtil()
{
}


void CObjUtil::writeCompressedObj(std::ostream& stream,
                                  const std::vector<tinyobj::shape_t>& shapes)
{
  assert(sizeof(float) == sizeof(uint32_t));
  const auto sz = sizeof(uint32_t);
  const uint32_t nMeshes = static_cast<uint32_t>(shapes.size());
  const uint32_t nMatProperties = 3;

  stream.write((const char*)&nMeshes, sz);        // nMeshes
  stream.write((const char*)&nMatProperties, sz); // nMatProperties

  for (size_t i = 0 ; i < nMeshes ; ++i) {
    const uint32_t nVertices  = (uint32_t)shapes[i].mesh.positions.size();
    const uint32_t nNormals   = (uint32_t)shapes[i].mesh.normals.size();
    const uint32_t nTexcoords = (uint32_t)shapes[i].mesh.texcoords.size();
    const uint32_t nIndices   = (uint32_t)shapes[i].mesh.indices.size();

    // Write nVertices, nNormals,, nTexcoords, nIndices
    // Write #nVertices positions
    // Write #nVertices normals
    // Write #nVertices texcoord
    // Write #nIndices  indices
    // Write #nMatProperties material properties
    stream.write((const char*)&nVertices,  sz);
    stream.write((const char*)&nNormals,   sz);
    stream.write((const char*)&nTexcoords, sz);
    stream.write((const char*)&nIndices,   sz);

    stream.write((const char*)&shapes[i].mesh.positions[0], nVertices  * sz);
    stream.write((const char*)&shapes[i].mesh.normals[0],   nNormals   * sz);
    stream.write((const char*)&shapes[i].mesh.texcoords[0], nTexcoords * sz);
    stream.write((const char*)&shapes[i].mesh.indices[0],   nIndices   * sz);
    stream.write((const char*)&shapes[i].material.ambient[0], 3 * sz);
  }
}


std::vector<tinyobj::shape_t> CObjUtil::readCompressedObj(std::istream& stream)
{
  assert(sizeof(float) == sizeof(uint32_t));
  const auto sz = sizeof(uint32_t);

  std::vector<tinyobj::shape_t> shapes;

  uint32_t nMeshes = 0;
  uint32_t nMatProperties = 0;
  stream.read((char*)&nMeshes, sz);
  stream.read((char*)&nMatProperties, sz);
  shapes.resize(nMeshes);
  for (size_t i = 0 ; i < nMeshes ; ++i) {
    uint32_t nVertices = 0, nNormals = 0, nTexcoords = 0, nIndices = 0;
    stream.read((char*)&nVertices,  sz);
    stream.read((char*)&nNormals,   sz);
    stream.read((char*)&nTexcoords, sz);
    stream.read((char*)&nIndices,   sz);

    shapes[i].mesh.positions.resize(nVertices);
    shapes[i].mesh.normals.resize(nNormals);
    shapes[i].mesh.texcoords.resize(nTexcoords);
    shapes[i].mesh.indices.resize(nIndices);

    stream.read((char*)&shapes[i].mesh.positions[0], nVertices  * sz);
    stream.read((char*)&shapes[i].mesh.normals[0],   nNormals   * sz);
    stream.read((char*)&shapes[i].mesh.texcoords[0], nTexcoords * sz);
    stream.read((char*)&shapes[i].mesh.indices[0],   nIndices   * sz);
    stream.read((char*)&shapes[i].material.ambient[0], 3 * sz);
  }
  return shapes;
}
