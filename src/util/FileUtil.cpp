#include <cassert>
#include <iostream>
#include <util/FileUtil.h>

#include <cassert>
#include <cstdio>
#include <fstream>

#include <util/Exception.h>
#include <util/SDL.h>

#ifdef USE_BOOST_FS
#include <boost/filesystem.hpp>
namespace fs = boost::filesystem;
#elif defined(__LINUX__)
#include <sys/types.h>
#include <sys/stat.h>
#endif

std::string FileUtil::read(const std::string& filename)
{
  SdlRWopsPtr file(SDL_RWFromFile(filename.c_str(),"rb"));
  if (file) {
    std::string text;
    // Go to end and report get size
    SDL_RWseek(file.get(), 0, SEEK_END);
    text.resize(SDL_RWtell(file.get()));

    // Go to beginning
    SDL_RWseek(file.get(), 0, SEEK_SET);

    // Read content to string
    SDL_RWread(file.get(), &text[0], text.size(), sizeof(text[0]));

    return(text);
  }
  throw Exception(std::string("Failed to open file: ") + filename);
}


void FileUtil::write(const std::string& filename,
                     const std::string& content)
{
  std::ofstream outfile(filename);
  if (!outfile.good())
    throw Exception(std::string("Failed to write to file: ") + filename);
  outfile << content;
}


bool FileUtil::exists(const std::string& filename)
{
  SdlRWopsPtr file(SDL_RWFromFile(filename.c_str(),"rb"));
  return file.get() != nullptr;
}


void FileUtil::append(const std::string& filename,
                      const std::string& content)
{
  std::ofstream outfile(filename, std::ios_base::app);
  if (!outfile.good())
    throw Exception(std::string("Failed to append to file: ") + filename);
  outfile << content;
}


bool FileUtil::remove(const std::string& filename)
{
  return std::remove(filename.c_str()) == 0;
}


std::time_t FileUtil::getLastModifiedTime(const std::string& filename)
{
#ifdef USE_BOOST_FS
  return fs::last_write_time(filename.c_str());
#elif defined(__LINUX__)
  struct stat st;
  return stat(filename.c_str(), &st) == 0 ? st.st_mtime : 0;
#else
  (void)filename;
  return 1;
#endif
}
