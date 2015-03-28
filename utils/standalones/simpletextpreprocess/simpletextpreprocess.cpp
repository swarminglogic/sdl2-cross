#include <iostream>

#include <cstdio>
#include <fstream>
#include <string>
#include <util/StringUtil.h>

namespace {
  static std::string read(std::ifstream& file)
  {
    std::string text;
    file.seekg(0, std::ios::end);
    text.resize(file.tellg());
    file.seekg(0, std::ios::beg);
    file.read(&text[0], text.size());
    file.close();
    return(text);
  }
}



int main(int argc, char *argv[])
{
  if (argc != 3) {
    std::cerr << "Invalid paramters." << std::endl
              << "Useage: "
              << argv[0] << " KEYWORD file" << std::endl;
    return EXIT_FAILURE;
  }

  std::string keyword(argv[1]);
  std::string filename(argv[2]);

  std::ifstream file(filename, std::ios::in | std::ios::binary);
  if (!file) {
    std::cerr << "Error: File " << filename << " was not found." << std::endl;
  }

  std::string contents;
  contents = ::read(file);
  std::cout << StringUtil::processIfEndif(contents, keyword) << std::endl;

  return EXIT_SUCCESS;
}
