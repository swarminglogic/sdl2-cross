#include <util/File.h>

#include <functional>
#include <limits>

#include <util/Clock.h>
#include <util/FileInfo.h>
#include <util/FileUtil.h>
#include <util/Assert.h>


File::File() : File("")
{
}


File::File(const std::string& filename)
    : File(filename, FileInfo::TYPE_UNSPECIFIED)
{
}


File::File(const std::string& filename,
           FileInfo::FileType filetype)
    : File(FileInfo(filename, filetype))
{
}


File::File(const FileInfo& fileInfo)
    : fileInfo_(fileInfo),
      timeLastRead_(0),
      localCopy_(""),
      localCopyHash_(0)
{
}


File::~File()
{
}


bool File::exists() const
{
  return FileUtil::exists(fileInfo_);
}


bool File::remove() const
{
  return FileUtil::remove(fileInfo_);
}


bool File::rename(const std::string& newFilename)
{
  bool isRenameOk = FileUtil::rename(fileInfo_, newFilename);
  if (isRenameOk)
    fileInfo_.setFilename(newFilename);
  return isRenameOk;
}


bool File::rename(const FileInfo& newFileInfo)
{
  assert(fileInfo_.getFiletype() == newFileInfo.getFiletype());
  return rename(newFileInfo.getFilename());
}


const std::string& File::read()
{
  timeLastRead_ = Clock::now();
  const std::string content = FileUtil::read(fileInfo_);
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
  FileUtil::write(fileInfo_, content);
}


bool File::safeWrite(const std::string& content)
{
  return FileUtil::safeWrite(fileInfo_, content);
}


void File::append(const std::string& content)
{
  // TODO swarminglogic, 2014-11-04: Assert correct filetype_
  FileUtil::append(fileInfo_, content);
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
  return FileUtil::getLastModifiedTime(fileInfo_.getFilename());
}


const std::string& File::getFilename() const
{
  return fileInfo_.getFilename();
}


void File::setFilename(const std::string& filename)
{
  if (fileInfo_.getFilename() == filename)
    return;

  fileInfo_.setFilename(filename);
  timeLastRead_  = 0;
  localCopy_     = "";
  localCopyHash_ = 0;
}


const FileInfo& File::getFileInfo() const
{
  return fileInfo_;
}
