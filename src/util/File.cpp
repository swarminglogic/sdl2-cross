#include <util/File.h>

#include <functional>
#include <limits>

#include <util/Clock.h>
#include <util/FileUtil.h>
#include <util/assert.h>


File::File() : File("")
{
}


File::File(const std::string& filename)
    : File(filename, FileUtil::FILETYPE_UNSPECIFIED)
{
}


File::File(const std::string& filename,
           FileUtil::FileType filetype)
    : filename_(filename),
      timeLastRead_(0),
      localCopy_(""),
      localCopyHash_(0),
      filetype_(filetype)
{
}


File::~File()
{
}


bool File::exists() const
{
  return FileUtil::exists(filename_, filetype_);
}


bool File::remove() const
{
  return FileUtil::remove(filename_, filetype_);
}


const std::string& File::read()
{
  timeLastRead_ = Clock::now();
  const std::string content = FileUtil::read(filename_, filetype_);
  std::size_t contentHash = std::hash<std::string>()(content);
  if (contentHash != localCopyHash_) {
    localCopy_ = content;
    localCopyHash_ = contentHash;
  }
  return localCopy_;
}


void File::write(const std::string& content)
{
  // TODO swarminglogic, 2014-11-04: Assert correct filetype_
  FileUtil::write(filename_, content, filetype_);
}


void File::append(const std::string& content)
{
  // TODO swarminglogic, 2014-11-04: Assert correct filetype_
  FileUtil::append(filename_, content, filetype_);
}


const std::string& File::getLocalCopy() const
{
  return localCopy_;
}


bool File::update()
{
  const std::size_t localCopyHashTmp =  localCopyHash_;
  read();
  return localCopyHash_ != localCopyHashTmp;
}


bool File::isUpdated() const
{
  return timeLastRead_ < getLastModifiedTime();
}


std::time_t File::getLastModifiedTime() const
{
  return FileUtil::getLastModifiedTime(filename_);
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
  localCopyHash_   = 0;
}
