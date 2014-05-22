#include <util/FileMonitor.h>

#include <cassert>
#include <functional>

#include <util/Clock.h>
#include <util/FileUtil.h>


FileMonitor::FileMonitor(const std::string& filename)
  : filename_(filename),
    timestamp_(0)
{
}


FileMonitor::~FileMonitor()
{
}


bool FileMonitor::exists() const
{
  return FileUtil::exists(filename_);
}



bool FileMonitor::isUpdated() const
{
  return timestamp_ < getLastModifiedTime();
}


void FileMonitor::resetTimeStamp()
{
  timestamp_ = FileUtil::getLastModifiedTime(filename_);
}


std::time_t FileMonitor::getLastModifiedTime() const
{
  return FileUtil::getLastModifiedTime(filename_);
}


const std::string& FileMonitor::getFilename() const
{
  return filename_;
}

