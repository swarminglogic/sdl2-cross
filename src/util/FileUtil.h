#ifndef UTIL_FILEUTIL_H
#define UTIL_FILEUTIL_H

#include <ctime>
#include <string>


/**
 * Static utility class for managing files
 *
 * @author Roald Fernandez
 */
class FileUtil
{
 public:
  /**
   * Reads the whole content of a file, returned as a string.
   *
   * Check if file exists before trying to read it.
   *
   * @throw Exception, if I/O error occurs.
   * @return Content of file.
   */
  static std::string read(const std::string& filename);

  /**
   * Appends content to a file (creates one if non-existent).
   *
   * @throw Exception, if failed to open and also failed to create file.
   *
   * @param filename
   * @param content
   */
  static void append(const std::string& filename,
                     const std::string& content);

  /**
   * Checks if a file exists or not.
   * Does not create a file if none exist.
   *
   * @param filename  Path to file to check.
   * @return  True if file exists (and is accessible, of course).
   */
  static bool exists(const std::string& filename);

  /**
   * Writes content to a file (creates one if non-existent).
   *
   * @throw Exception, if failed to create file.
   * @note Warning: Overrides file if it exists!
   * @param filename
   * @param content
   */
  static void write(const std::string& filename,
                    const std::string& content);

  /**
   * Deletes a file, returns true if successfull, false otherwise.
   */
  static bool remove(const std::string& filename);

  static std::time_t getLastModifiedTime(const std::string& filename);

 private:
  FileUtil();  // Static class
  // Not implemented!
  FileUtil(const FileUtil& c);
  FileUtil& operator=(const FileUtil& c);
};

#endif  // UTIL_FILEUTIL_H
