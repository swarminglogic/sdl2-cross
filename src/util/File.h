#ifndef UTIL_FILE_H
#define UTIL_FILE_H

#include <ctime>
#include <string>


/**
 * File class.
 * Used for reading clear text files.
 * Supports:
 *   - Checking timestamp for updates : isUpdated()
 *   - Checking if content modified hash : isModified()
 *
 * Typical usage
 *   a) File(fn, false) -> read(), isUpdated(), read()
 *   b) File(fn)        -> read(), isModified(), getLocalCopy()
 *
 * Usage a) relies on OS-level timestamp, and might be inaccurate.
 * Usage b) relies on content hash, and is more costly.
 *
 * @author SwarmingLogic (Roald Fernandez)
 */
class File
{
public:
  File(const std::string& filename);

  File();

  virtual ~File();

  /**
   * Check if file exists, and is accessible.
   *
   * If you expect one to exist (presumably to read the content), use this
   * function to check for it.
   */
  bool exists() const;

  /**
   * Reads the whole content of file, returned as string.
   * Optional is  isModified() ->[true]-> getLocalCopy()
   */
  std::string read();

  /**
   * Reads the whole content of file, and stores it locally.
   **/
  void readToLocal();

  /**
   * @return Local copy.
   */
  const std::string& readCopy();

  /**
   * Checks modified-timestamp of file, and compares it to when it was last
   * read.
   *
   * @note This is highly dependant of OS-level timestamp resolution!
   *       Is in the possible range of [1s, 1hour].
   *
   * TODO swarminglogic, 2013-09-17: Write function that determines approximate
   *                                 resolution.
   *
   * @return True if it has been modified since last time it was read.
   */
  bool isUpdated() const;

  /**
   * Reads the file, and compares with hash from when it was last read.
   *
   * @note Updates local copy (if enabled).
   * @see getLocalCopy
   *
   * @return True if hash is different, suggesting content has changed.
   */
  bool isModified();

  /**
   * @note filename_ is const and cannot change -> return by const reference.
   * @return filename.
   */
  const std::string& getFilename() const;
  void setFilename(const std::string& filename);

private:
  std::time_t getLastReadTime() const;
  std::time_t getLastModifiedTime() const;

  std::string filename_;
  std::time_t timeLastRead_;

  std::string localCopy_;
  std::size_t contentReadHash_;
  std::size_t localCopyHash_;
};

#endif
