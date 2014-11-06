#include <util/FileMonitor.h>

#include <functional>

#include <util/Clock.h>
#include <util/FileUtil.h>
#include <util/assert.h>


FileMonitor::FileMonitor(const std::string& filename,
                         FileUtil::FileType fileType)
  : filename_(filename),
    timestamp_(0),
    fileType_(fileType)
{
}


FileMonitor::~FileMonitor()
{
}


bool FileMonitor::exists() const
{
  return FileUtil::exists(filename_, fileType_);
}



bool FileMonitor::isUpdated() const
{
  return timestamp_ < getLastModifiedTime();
}


void FileMonitor::resetTimeStamp()
{
  timestamp_ = FileUtil::getLastModifiedTime(filename_, fileType_);
}


std::time_t FileMonitor::getLastModifiedTime() const
{
  return FileUtil::getLastModifiedTime(filename_, fileType_);
}


const std::string& FileMonitor::getFilename() const
{
  return filename_;
}
