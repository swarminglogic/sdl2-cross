#ifndef UTIL_FILEUTIL_H
#define UTIL_FILEUTIL_H

#include <ctime>
#include <string>

#include <util/FileInfo.h>


/**
 * Static utility class for managing files
 *
 * Since SDL does OS-specific stuff behind the scene for determining
 * paths. This class adds a layer of safety on top of that.
 * For instance, on linux, using SDL_RWFromFile() with just a filename will
 * write on the current working directory if it can. However, on Android,
 * this is not the case, and it will write in the SDL_GetPrefPath(),
 * which is the desired behaviour.
 *
 * A filename can be prefixed with a prefix path using prefixPath(),
 * or directly in the various functions, read(), append(), etc.
 * Caller should know whether this path already contains the prefix path.
 *
 * Use of FileUtil::read, and FileUtil::append without a prefixPath of
 * FILETYPE_USER_WRITEABLE is highly suspect, unless the path is already
 * absolute.
 *
 * @author Roald Fernandez
 */
class FileUtil
{
 public:
  static std::string prefixPath(const FileInfo& fileInfo);
  static std::string prefixPath(const std::string& filename,
                                FileInfo::FileType ft);

  /**
   * Reads the whole content of a file, returned as a string.
   *
   * Check if file exists before trying to read it.
   *
   * @throw Exception, if I/O error occurs.
   * @return Content of file.
   */
  static std::string read(const FileInfo& fileInfo);
  static std::string read(const std::string& filename,
                          FileInfo::FileType ft = FileInfo::TYPE_UNSPECIFIED);

  /**
   * Appends content to a file (creates one if non-existent).
   *
   * @throw Exception, if failed to open and also failed to create file.
   *
   * @param filename
   * @param content
   */
  static void append(const FileInfo& fileInfo,
                     const std::string& content);
  static void append(const std::string& filename,
                     const std::string& content,
                     FileInfo::FileType ft = FileInfo::TYPE_UNSPECIFIED);

  /**
   * Checks if a file exists or not.
   * Does not create a file if none exist.
   *
   * @param filename  Path to file to check.
   * @return  True if file exists (and is accessible, of course).
   */
  static bool exists(const FileInfo& fileInfo);
  static bool exists(const std::string& filename,
                     FileInfo::FileType ft = FileInfo::TYPE_UNSPECIFIED);

  /**
   * Writes content to a file (creates one if non-existent).
   *
   * @throw Exception, if failed to create file.
   * @note Warning: Overrides file if it exists!
   * @param filename
   * @param content
   */
  static void write(const FileInfo& fileInfo,
                    const std::string& content);
  static void write(const std::string& filename,
                    const std::string& content,
                    FileInfo::FileType ft = FileInfo::TYPE_UNSPECIFIED);


  /**
   * More expensive function to replace the content of the file.
   * It writes to a temporary file (suffixed by ".tmp").
   *
   * Reads back the data, and if as expected, renames the file to replace it
   *
   * Returns true if it was succesfull, false otherwise.  On failure, it also
   * clears up temporary file, and leaves the original as was (if it didn't
   * exist before this call, it still doesn't).
   *
   * @param content
   */
  static bool safeWrite(const FileInfo& fileInfo,
                        const std::string& content);
  static bool safeWrite(const std::string& filename,
                        const std::string& content,
                        FileInfo::FileType ft = FileInfo::TYPE_WRITABLE);

  /**
   * Renames a file, returns true if successfull, false otherwise.
   */
  static bool rename(const FileInfo& fileInfo, const std::string& newFilename);
  static bool rename(const FileInfo& fileInfo, const FileInfo& newFileInfo);
  static bool rename(const std::string& filename,
                     const std::string& newFilename,
                     FileInfo::FileType ft = FileInfo::TYPE_UNSPECIFIED);

  /**
   * Deletes a file, returns true if successfull, false otherwise.
   */
  static bool remove(const FileInfo& fileInfo);
  static bool remove(const std::string& filename,
                     FileInfo::FileType ft = FileInfo::TYPE_UNSPECIFIED);

  static std::time_t getLastModifiedTime(const FileInfo& fileInfo);
  static std::time_t getLastModifiedTime(
      const std::string& filename,
      FileInfo::FileType ft = FileInfo::TYPE_UNSPECIFIED);

  /**
   * Determines the path used to store user files.
   * If no such path is determined, an exception thrown.
   *
   * The return value is cached upon first call.
   */
  static const std::string& getUserWritablePath();

 private:
  static void write_or_append(const std::string& filename,
                              const std::string& content,
                              FileInfo::FileType ft,
                              const char* mode);

  static bool isUserWriteablePathCached_;
  static std::string userWriteablePath_;

  FileUtil();  // Static class
  // Not implemented!
  FileUtil(const FileUtil& c);
  FileUtil& operator=(const FileUtil& c);
};

#endif  // UTIL_FILEUTIL_H
