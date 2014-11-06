#ifndef UTIL_FILEINFO_H
#define UTIL_FILEINFO_H

#include <string>


/**
 * FileInfo class.
 *
 * Simple class representing a tuple (filename,filetype)
 * @author SwarmingLogic
 */
class FileInfo
{
 public:
  enum FileType{
    TYPE_WRITABLE = 0,
    TYPE_ASSET,
    TYPE_UNSPECIFIED,
  };

  FileInfo();
  virtual ~FileInfo();
  explicit FileInfo(const std::string& filename);
  FileInfo(const std::string& filename, FileType filetype);

  const std::string& getFilename() const;
  void setFilename(const std::string& filename);

  FileType getFiletype() const;
  void setFiletype(FileType filetype);

 private:
  std::string filename_;
  FileType filetype_;
};

#endif  // UTIL_FILEINFO_H
