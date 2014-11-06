#include <util/FileInfo.h>



FileInfo::FileInfo()
    : FileInfo("", FileType::TYPE_UNSPECIFIED)
{
}


FileInfo::~FileInfo()
{
}


FileInfo::FileInfo(const std::string& filename)
    : FileInfo(filename, FileType::TYPE_UNSPECIFIED)
{
}


FileInfo::FileInfo(const std::string& filename, FileType filetype)
    : filename_(filename),
      filetype_(filetype)
{
}



const std::string& FileInfo::getFilename() const
{
  return filename_;
}


void FileInfo::setFilename(const std::string& filename)
{
  filename_ = filename;
}



FileInfo::FileType FileInfo::getFiletype() const
{
  return filetype_;
}


void FileInfo::setFiletype(FileType filetype)
{
  filetype_ = filetype;
}
