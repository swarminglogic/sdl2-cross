#include <cstdio>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include <util/CObjUtil.h>
#include <util/StringUtil.h>


int main(int argc, char *argv[])
{
  if (argc < 2) {
    std::cerr << "Invalid parameters." << std::endl
              << "Useage: "
              << argv[0] << " input.obj [output.cobj]" << std::endl;
    std::cerr << "" << std::endl;
    return EXIT_FAILURE;
  }

  std::string input(argv[1]);
  std::string output;
  if (argc == 3) {
    output = argv[2];
    if (StringUtil::suffix(input, 4) != std::string(".obj")) {
      std::cerr <<  "Input filename must have .obj extension"  << std::endl;
      return EXIT_FAILURE;
    }
  } else {
    output = StringUtil::prefix(input, -4) + std::string(".cobj");
  }

  std::ifstream file(input, std::ios::in | std::ios::binary);
  if (!file) {
    std::cerr << "Error: File " << input << " was not found." << std::endl;
    return EXIT_FAILURE;
  }
  file.close();

  std::vector<tinyobj::shape_t> tempShapes;
  std::string emsg = tinyobj::LoadObj(tempShapes, input.c_str());

  if (emsg.empty()) {
    if (!tempShapes.empty()) {
      std::ofstream fileout(output, std::ios::out | std::ios::binary);
      if (fileout)
        CObjUtil::writeCompressedObj(fileout, tempShapes);
      else {
        std::cerr << "Error: Output file target " << output << " was invalid."
                  << std::endl;
        return EXIT_FAILURE;
      }
    } else {
      std::cerr << "Wavefront OBJ file (" << input
                << ") contained no meshes." << std::endl;
      std::cerr << "Output file (" << output << ") was not created. Aborting."
                << std::endl;
    }
  } else {
    std::cerr << "Error when loading Wavefront OBJ file (" << input << "):\n"
                << emsg << std::endl;
  }

  return EXIT_SUCCESS;
}
