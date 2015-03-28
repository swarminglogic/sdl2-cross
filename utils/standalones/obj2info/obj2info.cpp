#include <cstdio>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include <util/CObjUtil.h>
#include <util/StringUtil.h>


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

  const size_t outputLimit = 100;
  ss << "Vertices: " << std::endl;
  if (shape.mesh.positions.size() < outputLimit) {
    for (auto& x : shape.mesh.positions)
      ss << "  " << x;
  }
  else ss << "  [ Skipped ... " << shape.mesh.positions.size() << "  ]";
  ss << std::endl;

  ss << "Normals: " << std::endl;
  if (shape.mesh.normals.size() < outputLimit) {
    for (auto& x : shape.mesh.normals)
      ss << "  " << x;
  }
  else ss << "  [ Skipped ... " << shape.mesh.normals.size() << "  ]";
  ss << std::endl;

  ss << "Texcoords: " << std::endl;
  if (shape.mesh.texcoords.size() < outputLimit) {
    for (auto& x : shape.mesh.texcoords)
      ss << "  " << x;
  }
  else ss << "  [ Skipped ... " << shape.mesh.texcoords.size() << "  ]";
  ss << std::endl;

  ss << "Indices: " << std::endl;
  if (shape.mesh.indices.size() < outputLimit) {
  for (auto& x : shape.mesh.indices)
    ss << "  " << x;
  }
  else ss << "  [ Skipped ... " << shape.mesh.indices.size() << "  ]";
  ss << std::endl;

  return ss.str();
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

int main(int argc, char *argv[])
{
  if (argc != 2) {
    std::cerr << "Invalid parameters." << std::endl
              << "Useage: "
              << argv[0] << " input.[obj|cobj]" << std::endl;
    std::cerr << "" << std::endl;
    return EXIT_FAILURE;
  }

  std::string input(argv[1]);
  std::ifstream file(input, std::ios::in | std::ios::binary);
  if (!file) {
    std::cerr << "Error: File " << input << " was not found." << std::endl;
    return EXIT_FAILURE;
  }

  std::vector<tinyobj::shape_t> tempShapes;

  bool isObjExtension = StringUtil::suffix(input, 4) == std::string(".obj");
  bool isCompressedObjExtension =
    StringUtil::suffix(input, 5) == std::string(".cobj");

  if (isObjExtension) {
    // load w/tiny_obj
    std::string emsg = tinyobj::LoadObj(tempShapes, input.c_str());
    if (!emsg.empty()) {
      std::cerr << "Error when loading Wavefront OBJ file (" << input << "):\n"
                << emsg << std::endl;
      return EXIT_FAILURE;
    }
  } else if (isCompressedObjExtension) {
    // load with util
    tempShapes = CObjUtil::readCompressedObj(file);
  } else {
    std::cerr << "Input filename must have .obj or .cobj extension."
              << std::endl;
    return EXIT_FAILURE;
  }

  if (tempShapes.empty()) {
    if (isCompressedObjExtension)
      std::cerr << "Compressed ";
    std::cerr << "Wavefront OBJ file (" << input
              << ") contained no meshes. Aborting." << std::endl;
  } else {
    dumpTinyObj(tempShapes);
  }

  return EXIT_SUCCESS;
}
