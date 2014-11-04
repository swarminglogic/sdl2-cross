#ifndef UTIL_FILEMONITOR_H
#define UTIL_FILEMONITOR_H

#include <ctime>
#include <string>

#include <util/FileUtil.h>


/**
 * FileMonitor class.
 * Used for checking if file timestamp has changed.
 *
 * @author SwarmingLogic (Roald Fernandez)
 */
class FileMonitor
{
 public:
  FileMonitor(const std::string& filename,
              FileUtil::FileType ft = FileUtil::FILETYPE_UNSPECIFIED);

  virtual ~FileMonitor();

  /**
   * Check if file exists, and is accessible.
   */
  bool exists() const;

  /**
   * Checks modified-timestamp of file, and compares it to when it was last
   * reset.
   *
   * @return True if it has been modified since last time it was reset.
   */
  bool isUpdated() const;
  void resetTimeStamp();

  const std::string& getFilename() const;

 private:
  std::time_t getLastReadTime() const;
  std::time_t getLastModifiedTime() const;

  std::string filename_;
  std::time_t timestamp_;

  FileUtil::FileType fileType_;
};

#endif  // UTIL_FILEMONITOR_H
