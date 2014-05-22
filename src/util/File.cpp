#include <util/File.h>

#include <cassert>
#include <functional>

#include <util/Clock.h>
#include <util/FileUtil.h>


File::File(const std::string& filename)
  : filename_(filename),
    timeLastRead_(),
    localCopy_(""),
    contentReadHash_(0),
    localCopyHash_(0)
{
}

File::File() : File("")
{
}



File::~File()
{
}


bool File::exists() const
{
  return FileUtil::exists(filename_);
}


std::string File::read()
{
  timeLastRead_ = Clock::now();
  const std::string content = FileUtil::read(filename_);
  assert(timeLastRead_ >= getLastModifiedTime());

  std::size_t contentHash = std::hash<std::string>()(content);
  const bool isHashModified = contentHash != contentReadHash_;

  if (isHashModified)
    localCopy_ = content;
  contentReadHash_ = contentHash;
  return content;
}


void File::readToLocal()
{
  isModified();
}


const std::string& File::readCopy()
{
  contentReadHash_ = localCopyHash_;
  return localCopy_;
}


bool File::isModified()
{
  timeLastRead_ = Clock::now();
  const std::string content = FileUtil::read(filename_);
  assert(timeLastRead_ >= getLastModifiedTime());

  std::size_t contentHash = std::hash<std::string>()(content);
  const bool isHashModified = contentHash != contentReadHash_;

  if (isHashModified) {
    const bool actualStoredContentDiffers = (contentHash != localCopyHash_);
    if (actualStoredContentDiffers)
      localCopy_ = content;
  }

  // Don't update local hash, even if you mantain a copy of the file!
  localCopyHash_ = contentHash;
  return isHashModified;
}


bool File::isUpdated() const
{
  return timeLastRead_ < getLastModifiedTime();
}


std::time_t File::getLastModifiedTime() const
{
  return FileUtil::getLastModifiedTime(filename_);
}


std::time_t File::getLastReadTime() const
{
  return timeLastRead_;
}


const std::string& File::File::getFilename() const
{
  return filename_;
}


void File::setFilename(const std::string& filename)
{
  if (filename_ == filename) return;

  filename_        = filename;
  timeLastRead_    = 0;
  localCopy_       = "";
  contentReadHash_ = 0;
  localCopyHash_   = 0;
}

