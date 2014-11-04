#ifndef UTIL_FILE_H
#define UTIL_FILE_H

#include <ctime>
#include <string>

#include <util/FileUtil.h>


/**
 * File class.
 * Used for reading clear text files.
 * Supports:
 *   - Checking timestamp for updates : isUpdated()
 *   - Checking if content modified hash : update()
 *
 * read() and update() to almost the same.
 * A read() can be replaced by update(),getLocalCopy()
 *
 * @author SwarmingLogic (Roald Fernandez)
 */
class File
{
 public:

  File();
  explicit File(const std::string& filename);

  /**
   * Let's caller specify filetype.
   * See FileUtil for details.
   */
  File(const std::string& filename,
       FileUtil::FileType filetype);


  virtual ~File();

  /**
   * Check if file exists, and is accessible.
   *
   * If you expect one to exist (presumably to read the content), use this
   * function to check for it.
   */
  bool exists() const;


  /**
   * Deletes the file it represents from filesystem
   * @return true if successfull, false otherwise.
   */
  bool remove() const;

  /**
   * Reads the whole content of file, returned as string.
   * Optional is  isModified() ->[true]-> getLocalCopy()
   */
  const std::string& read();


  /**
   * Replaces the content of the file.
   *
   * @param content  The content to replace with.
   */
  void write(const std::string& content);


  /**
   * Appends content to file.
   *
   * @param content The content to append.
   */
  void append(const std::string& content);


   /**
   * @return Local copy.
   */
  const std::string& getLocalCopy() const;

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
   * @note Updates local copy.
   * @see getLocalCopy
   *
   * @return True if hash is different, suggesting content has changed.
   */
  bool update();

  /**
   * @note filename_ is const and cannot change -> return by const reference.
   * @return filename.
   */
  const std::string& getFilename() const;
  void setFilename(const std::string& filename);

 private:
  std::time_t getLastModifiedTime() const;

  std::string filename_;
  std::time_t timeLastRead_;

  std::string localCopy_;
  std::size_t localCopyHash_;

  FileUtil::FileType filetype_;
  static std::string preferencePath_;
};

#endif  // UTIL_FILE_H
