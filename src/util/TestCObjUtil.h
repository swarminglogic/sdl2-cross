#ifndef UTIL_TESTCOBJUTIL_H
#define UTIL_TESTCOBJUTIL_H

#include <util/CObjUtil.h>
#include <util/ObjUtil.h>

#include <cxxtest/TestSuite.h>


/**
 * Test for the CObjUtil class.
 *
 * @author SwarmingLogic
 */
class TestCObjUtil : public CxxTest::TestSuite
{
public:
  void testCobjReadWrite()
  {
    std::string tmpObjFile("tmpobjfile.obj");
    {
      std::ofstream outfile(tmpObjFile);
      outfile << makeCubeObjFile();
    }
    std::vector<tinyobj::shape_t> cube;
    LoadObj(cube, tmpObjFile.c_str());

    // Manipulating ambient material property
    const float amb0 =  0.123f;
    const float amb1 =  1.234f;
    const float amb2 =  2.345f;
    cube[0].material.ambient[0] = amb0;
    cube[0].material.ambient[1] = amb1;
    cube[0].material.ambient[2] = amb2;

    std::stringstream ssCObjData;
    CObjUtil::writeCompressedObj(ssCObjData, cube);

    const std::vector<tinyobj::shape_t> cubeFromCObj =
      CObjUtil::readCompressedObj(ssCObjData);

    TS_ASSERT_EQUALS(cube.size(), cubeFromCObj.size());
    TS_ASSERT_EQUALS(cubeFromCObj.size(), 1u);

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
    TS_ASSERT_DELTA(cubeFromCObj[0].material.ambient[0], amb0, delta);
    TS_ASSERT_DELTA(cubeFromCObj[0].material.ambient[1], amb1, delta);
    TS_ASSERT_DELTA(cubeFromCObj[0].material.ambient[2], amb2, delta);
    TS_ASSERT(std::remove(tmpObjFile.c_str()) == 0);
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
};

#endif
