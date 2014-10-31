#ifndef UTIL_TESTOBJUTIL_H
#define UTIL_TESTOBJUTIL_H

#include <sstream>

#include <extern/tiny_obj_loader.h>
#include <util/CObjUtil.h>
#include <util/ObjUtil.h>

#include <cxxtest/TestSuite.h>


/**
 * Test for the ObjUtil class.
 *
 * @author SwarmingLogic
 */
class TestObjUtil : public CxxTest::TestSuite
{
 public:
  void testObjRead()
  {
#ifdef __ANDROID__
    TS_SKIP("Not yet implemented for android.");
#endif
    std::string tmpObjFile("tmpobjfile.obj");
    {
      std::ofstream outfile(tmpObjFile);
      outfile << makeCubeObjFile();
    }

    std::vector<tinyobj::shape_t> cube;
    LoadObj(cube, tmpObjFile.c_str());
    TS_ASSERT(!cube.empty());
    TS_ASSERT_EQUALS(cube.size(), 1u);

    TS_ASSERT_EQUALS(cube[0].name, "CubiCube");
    TS_ASSERT_EQUALS(cube[0].mesh.positions.size(), 72);
    TS_ASSERT_EQUALS(cube[0].mesh.normals.size(),   72);
    TS_ASSERT_EQUALS(cube[0].mesh.texcoords.size(), 0);
    TS_ASSERT_EQUALS(cube[0].mesh.indices.size(),   3*2*6);

    const double delta = 0.000001;
    for (auto& x : cube[0].mesh.positions) {
      TS_ASSERT_LESS_THAN_EQUALS(x - delta, 1.0);
      TS_ASSERT_LESS_THAN_EQUALS(-1.0, x + delta);
    }

    TS_ASSERT(std::remove(tmpObjFile.c_str()) == 0);
  }

  void testObjReadFileSystemAgnostic() {
#ifdef __ANDROID__
    TS_SKIP("Not yet implemented for android.");
#endif
    std::string tmpObjFile("tmpobjfile.obj");
    {
      std::ofstream outfile(tmpObjFile);
      outfile << makeCubeObjFile();
    }
    std::vector<tinyobj::shape_t> cube;
    LoadObj(cube, tmpObjFile.c_str());

    AssetMesh tmpCObjAsset("tmpcobjfile.cobj");
    {
      std::ofstream ssCObjData(tmpCObjAsset.path());
      CObjUtil::writeCompressedObj(ssCObjData, cube);
    }

    const std::vector<tinyobj::shape_t> cubeFromCObj =
      ObjUtil::read(tmpCObjAsset);

    TS_ASSERT_EQUALS(cube.size(), cubeFromCObj.size());
    TS_ASSERT_EQUALS(cubeFromCObj.size(), 1u);

    TS_ASSERT(!cube.empty());
    TS_ASSERT(!cubeFromCObj.empty());
    if (!cube.empty() && !cubeFromCObj.empty()) {
      const float delta = 0.000001f;
      TS_ASSERT_EQUALS(cube[0].mesh.positions.size(),
                       cubeFromCObj[0].mesh.positions.size());
      for (size_t i = 0 ; i < cube[0].mesh.positions.size() ; ++i) {
        TS_ASSERT_DELTA(cube[0].mesh.positions[i],
                        cubeFromCObj[0].mesh.positions[i],
                        delta);
      }

      TS_ASSERT_EQUALS(cube[0].mesh.normals.size(),
                       cubeFromCObj[0].mesh.normals.size());
      for (size_t i = 0 ; i < cube[0].mesh.normals.size() ; ++i) {
        TS_ASSERT_DELTA(cube[0].mesh.normals[i],
                        cubeFromCObj[0].mesh.normals[i],
                        delta);
      }

      TS_ASSERT_EQUALS(cube[0].mesh.texcoords.size(),
                       cubeFromCObj[0].mesh.texcoords.size());
      for (size_t i = 0 ; i < cube[0].mesh.texcoords.size() ; ++i) {
        TS_ASSERT_DELTA(cube[0].mesh.texcoords[i],
                        cubeFromCObj[0].mesh.texcoords[i],
                        delta);
      }

      TS_ASSERT_EQUALS(cube[0].mesh.indices.size(),
                       cubeFromCObj[0].mesh.indices.size());
      for (size_t i = 0 ; i < cube[0].mesh.indices.size() ; ++i) {
        TS_ASSERT_EQUALS(cube[0].mesh.indices[i],
                         cubeFromCObj[0].mesh.indices[i]);
      }

      TS_ASSERT_SAME_DATA(cube[0].material.ambient,
                          cubeFromCObj[0].material.ambient, 3);
    }

    TS_ASSERT(std::remove(tmpObjFile.c_str()) == 0);
    TS_ASSERT(std::remove(tmpCObjAsset.path().c_str()) == 0);
  }



 private:
  std::string makeCubeObjFile() {
    return R"(# Blender v2.70 (sub 5) OBJ File: ''
# www.blender.org
o CubiCube
v 1.000000 -1.000000 -1.000000
v 1.000000 -1.000000 1.000000
v -1.000000 -1.000000 1.000000
v -1.000000 -1.000000 -1.000000
v 1.000000 1.000000 -0.999999
v 0.999999 1.000000 1.000001
v -1.000000 1.000000 1.000000
v -1.000000 1.000000 -1.000000
vn 0.000000 -1.000000 0.000000
vn 0.000000 1.000000 0.000000
vn 1.000000 -0.000000 0.000000
vn -0.000000 -0.000000 1.000000
vn -1.000000 -0.000000 -0.000000
vn 0.000000 0.000000 -1.000000
s off
f 2//1 3//1 4//1
f 8//2 7//2 6//2
f 1//3 5//3 6//3
f 2//4 6//4 7//4
f 7//5 8//5 4//5
f 1//6 4//6 8//6
f 1//1 2//1 4//1
f 5//2 8//2 6//2
f 2//3 1//3 6//3
f 3//4 2//4 7//4
f 3//5 7//5 4//5
f 5//6 1//6 8//6
)";
  }

  // Useful for debugging shape_t object contents
  // Not actually used in tests
  void dumpTinyObj(const std::vector<tinyobj::shape_t>& shapes) {
    std::cout << std::endl;
    for (auto& shape : shapes) {
      std::cout << "########################################" << std::endl;
      std::cout << shapeToString(shape) << std::endl;
    }
    (void)shapes;
  }

  // Useful for debugging shape_t object contents
  // Not actually used in tests
  std::string shapeToString(const tinyobj::shape_t& shape) {
    std::stringstream ss;
    ss << "Shape: " << shape.name << std::endl;

    ss << "Material:        " << std::endl;

    const float* amb = shape.material.ambient;
    ss << "  ambient:       "
       << amb[0] << ", " << amb[1] << ", " << amb[2] << std::endl;

    const float* diff = shape.material.diffuse;
    ss << "  diffuse:       " <<
      diff[0] << ", " << diff[1] << ", " << diff[2] << std::endl;

    const float* spec = shape.material.specular;
    ss << "  specular:      "
       << spec[0] << ", " << spec[1] << ", " << spec[2] << std::endl;

    const float* trans = shape.material.transmittance;
    ss << "  transmittance: "
       << trans[0] << ", " << trans[1] << ", " << trans[2] << std::endl;

    const float* emi = shape.material.emission;
    ss << "  emission:      "
       << emi[0] << ", " << emi[1] << ", " << emi[2] << std::endl;

    ss << "  shininess:     " << shape.material.shininess << std::endl;
    ss << "  ior:           " << shape.material.ior       << std::endl;

    ss << "Ambient tex:  " << shape.material.ambient_texname  << std::endl;
    ss << "Diffuse tex:  " << shape.material.diffuse_texname  << std::endl;
    ss << "Specular tex: " << shape.material.specular_texname << std::endl;
    ss << "Normal tex:   " << shape.material.normal_texname   << std::endl;

    ss << "Unknown parameters: " << shape.material.unknown_parameter.size()
       << std::endl;
    if (!shape.material.unknown_parameter.empty()) {
      for (auto& p : shape.material.unknown_parameter) {
        ss << "  - " << p.first << ": " << p.second << std::endl;
      }
    }

    ss << "Mesh: " << std::endl;
    ss << "Vertex count:   " << shape.mesh.positions.size() << std::endl;
    ss << "Normal count:   " << shape.mesh.normals.size()   << std::endl;
    ss << "Texcoord count: " << shape.mesh.texcoords.size() << std::endl;
    ss << "Indices count:  " << shape.mesh.indices.size()   << std::endl;

    ss << "Vertices: " << std::endl;
    for (auto& x : shape.mesh.positions)
      ss << "  " << x;
    ss << std::endl;

    ss << "Normals: " << std::endl;
    for (auto& x : shape.mesh.normals)
      ss << "  " << x;
    ss << std::endl;

    ss << "Texcoords: " << std::endl;
    for (auto& x : shape.mesh.texcoords)
      ss << "  " << x;
    ss << std::endl;

    ss << "Indices: " << std::endl;
    for (auto& x : shape.mesh.indices)
      ss << "  " << x;
    ss << std::endl;

    return ss.str();
  }
};

#endif  // UTIL_TESTOBJUTIL_H
