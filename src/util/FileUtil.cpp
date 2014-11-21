#include <util/FileUtil.h>

#include <cstdio>

#include <util/Assert.h>
#include <util/Asset.h>
#include <util/Exception.h>
#include <util/FileInfo.h>
#include <util/SDL.h>


#ifdef USE_BOOST_FS
#include <boost/filesystem.hpp>
namespace fs = boost::filesystem;
#elif defined(__LINUX__)
#include <sys/types.h>
#include <sys/stat.h>
#endif


bool FileUtil::isUserWriteablePathCached_ = false;
std::string FileUtil::userWriteablePath_ = std::string();
#ifdef __ANDROID__
const std::string FileUtil::assetPrefix_("");
#else
const std::string FileUtil::assetPrefix_("./assets/");
#endif


std::string FileUtil::prefixPath(const FileInfo& fileInfo)
{
  return prefixPath(fileInfo.getFilename(), fileInfo.getFiletype());
}

std::string FileUtil::prefixPath(const Asset& asset)
{
  return prefixPath(asset.path(), FileInfo::TYPE_ASSET);
}


std::string FileUtil::prefixPath(const std::string& filename,
                                 FileInfo::FileType fileType)
{
  if (fileType == FileInfo::TYPE_WRITABLE) {
    const std::string& prefix = FileUtil::getUserWritablePath();
    if (prefix[0] == filename[0])
      return filename;
    else
      return prefix + filename;
  } else if (fileType == FileInfo::TYPE_ASSET){
    return assetPrefix_ + filename;
  } else {
    return filename;
  }
}

namespace sdl {
//  Modified version of SDL_RWFromFile, that solves the problem of properly
//  distinguishing between assets and user modifiable directory.
SDL_RWops* RWFromFile(const char *file,
                      const char *mode,
                      FileInfo::FileType fileType) {
  RWopsPtr rwops(nullptr);
#ifdef __ANDROID__
  if (fileType == FileInfo::TYPE_WRITABLE) {
    if (*file == '/') {
      FILE *fp = fopen(file, mode);
      if (fp)
        return SDL_RWFromFP(fp, SDL_TRUE);
    } else {
      std::string path = FileUtil::getUserWritablePath() + file;
      FILE *fp = nullptr;
      fp = fopen(path.c_str(), mode);
      if (fp != nullptr)
        return SDL_RWFromFP(fp, SDL_TRUE);
    }
    return nullptr;
  }
#endif
  (void) fileType;
  return SDL_RWFromFile(file, mode);
}
}


std::string FileUtil::read(const FileInfo& fileInfo)
{
  return read(fileInfo.getFilename(), fileInfo.getFiletype());
}

std::string FileUtil::read(const Asset& asset)
{
  return read(asset.path(), FileInfo::TYPE_ASSET);
}

std::string FileUtil::read(const std::string& filename,
                           FileInfo::FileType fileType)
{
  const std::string pFilename = prefixPath(filename, fileType);
  sdl::RWopsPtr file(sdl::RWFromFile(pFilename.c_str(), "rb", fileType));
  if (file) {
    std::string data;

    // Go to end and report get size
    SDL_RWseek(file.get(), 0, SEEK_END);
    data.resize(SDL_RWtell(file.get()));

    // Go to beginning
    SDL_RWseek(file.get(), 0, SEEK_SET);

    // Read content to string
    SDL_RWread(file.get(), &data[0], data.size(), sizeof(data[0]));

    return(data);
  }
  throw Exception("Failed to open file: " + pFilename);
}


void FileUtil::write(const FileInfo& fileInfo,
                     const std::string& content)
{
  write(fileInfo.getFilename(), content, fileInfo.getFiletype());
}


void FileUtil::write(const std::string& filename,
                     const std::string& content,
                     FileInfo::FileType fileType)
{
  write_or_append(filename, content, fileType, "w");
}


bool FileUtil::safeWrite(const FileInfo& fileInfo,
                         const std::string& content)
{
  return safeWrite(fileInfo.getFilename(), content, fileInfo.getFiletype());
}

bool FileUtil::safeWrite(const std::string& filename,
                         const std::string& content,
                         FileInfo::FileType ft)
{
  // Create temporary file and write the data to this file.
  const std::string tmpFilename = filename + ".tmp";
  bool isDataOk = false;
  try {
    assert(!FileUtil::exists(tmpFilename, ft));
    FileUtil::write(tmpFilename, content, ft);
    isDataOk = FileUtil::read(tmpFilename, ft) == content;
    if (isDataOk)
      FileUtil::rename(tmpFilename, filename, ft);
  } catch (const Exception& e) {
    isDataOk = false;
  }

  // Double check cleanup
  if (!isDataOk && FileUtil::exists(tmpFilename, ft))
    FileUtil::remove(tmpFilename, ft);
  return isDataOk;
}


void FileUtil::write_or_append(const std::string& filename,
                               const std::string& content,
                               FileInfo::FileType fileType,
                               const char* mode)
{
  // TODO swarminglogic, 2014-11-03: add assertion on bad fileType

  const std::string pFilename = prefixPath(filename, fileType);
  sdl::RWopsPtr file(sdl::RWFromFile(pFilename.c_str(), mode, fileType));
  if (file.get() == nullptr) {
    throw Exception(std::string("Failed to write to file: ") + pFilename);
  } else {
    const bool isWriteOk = (SDL_RWwrite(file.get(),
                                        content.c_str(),
                                        1,
                                        content.size())
                            == content.size());
    if (!isWriteOk) {
      throw Exception("Failed to fully write: " + pFilename);
    }
  }
}


bool FileUtil::exists(const FileInfo& fileInfo)
{
  return exists(fileInfo.getFilename(), fileInfo.getFiletype());
}


bool FileUtil::exists(const Asset& asset)
{
  return exists(asset.path(), FileInfo::TYPE_ASSET);
}

bool FileUtil::exists(const std::string& filename,
                      FileInfo::FileType fileType)
{
  const std::string pFilename = prefixPath(filename, fileType);
  sdl::RWopsPtr file(sdl::RWFromFile(pFilename.c_str(),
                                     "rb",
                                     fileType));
  return file.get() != nullptr;
}


void FileUtil::append(const FileInfo& fileInfo,
                      const std::string& content)
{
  return append(fileInfo.getFilename(), content, fileInfo.getFiletype());
}



void FileUtil::append(const std::string& filename,
                      const std::string& content,
                      FileInfo::FileType fileType)
{
  // TODO swarminglogic, 2014-11-03: add assertion on bad fileType
  write_or_append(filename, content, fileType, "a");
}


bool FileUtil::remove(const FileInfo& fileInfo)
{
  return remove(fileInfo.getFilename(), fileInfo.getFiletype());
}


bool FileUtil::remove(const std::string& filename,
                      FileInfo::FileType fileType)
{
  // TODO swarminglogic, 2014-11-03: add assertion on bad fileType
  const std::string pFilename = prefixPath(filename, fileType);
  return std::remove(pFilename.c_str()) == 0;
}

bool FileUtil::rename(const FileInfo& fileInfo, const std::string& newFilename)
{
  return rename(fileInfo.getFilename(), newFilename, fileInfo.getFiletype());
}

bool FileUtil::rename(const FileInfo& fileInfo, const FileInfo& newFileInfo)
{
  assert(fileInfo.getFiletype() == newFileInfo.getFiletype());
  return rename(fileInfo, newFileInfo.getFilename());
}

bool FileUtil::rename(const std::string& filename,
                      const std::string& newFilename,
                      FileInfo::FileType ft)
{
  std::string pFilename = prefixPath(filename, ft);
  std::string pNewFilename = prefixPath(newFilename, ft);
  if (pFilename == pNewFilename) {
    return false;
  } else {
    // If file to rename doesn't exist
    if (!FileUtil::exists(filename, ft))
      return false;

    // For windows, we need to remove the file,
    // as std::rename doesn't override
#ifdef __WIN32__
    if (FileUtil::exists(newFilename, ft))
      assert(FileUtil::remove(newFilename, ft));
#endif

    return std::rename(pFilename.c_str(), pNewFilename.c_str()) == 0;
  }
}


std::time_t FileUtil::getLastModifiedTime(const FileInfo& fileInfo)
{
  return getLastModifiedTime(fileInfo.getFilename(), fileInfo.getFiletype());
}


std::time_t FileUtil::getLastModifiedTime(const std::string& filename,
                                          FileInfo::FileType fileType)
{
  const std::string pFilename = prefixPath(filename, fileType);
#ifdef USE_BOOST_FS
  return fs::last_write_time(pFilename.c_str());
#elif defined(__LINUX__)
  struct stat st;
  return stat(pFilename.c_str(), &st) == 0 ? st.st_mtime : 0;
#else
  (void)filename;
  (void)pFilename;
  return 1;
#endif
}


const std::string& FileUtil::getUserWritablePath()
{
  if (!isUserWriteablePathCached_) {
    char *prefPath = SDL_GetPrefPath("swarminglogic", "sdl-cross");
    if (prefPath != nullptr) {
      userWriteablePath_ = std::string(prefPath);
      SDL_free(prefPath);
      isUserWriteablePathCached_ = true;
    } else {
      // PS: there is no memory leak here, even though it could look like it.
      throw Exception("Filed to acquire application writeable folder");
    }
  }

  return userWriteablePath_;
}
