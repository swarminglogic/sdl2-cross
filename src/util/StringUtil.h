#ifndef UTIL_STRINGUTIL_H
#define UTIL_STRINGUTIL_H

#include <string>
#include <vector>


/**
 * StringUtil class.
 * Static utility functions for manipulating strings.
 *
 * See unit test for uses.
 *
 * @author SwarmingLogic (Roald Fernandez)
 */
class StringUtil
{
 public:
  static void split(const std::string &text,
                    std::vector<std::string>& elements,
                    char delimiter);

  static std::vector<std::string> split(const std::string &text,
                                        char delimiter);


  /**
   * Extracts last #length chars from #text
   * suffix("abcdef012345", 3) == "345"
   * suffix("abcdef012345", -2) == "cdef012345"
   */
  static std::string suffix(const std::string& text, int length);

  /**
   * Extracts first #length chars from #text
   * prefix("abcdef012345", 3) == "abc"
   * prefix("abcdef012345", -2) == "abccdef0123"
   */
  static std::string prefix(const std::string& text, int length);

  // Creating a copy of the string removing whitespace
  static std::string ltrimc(const std::string& text);
  static std::string rtrimc(const std::string& text);
  static std::string trimc(const std::string& text);

  // Trimming in place
  static void ltrim(std::string& text);
  static void rtrim(std::string& text);
  static void trim(std::string& text);

  // Prepend in place
  static void prepend(const std::string& pre,
                      std::string& text);
  // Prepend in copy place
  static std::string prependc(const std::string& pre,
                              const std::string& text);

  /**
   * Processes a text for #if X, #else, #endif keywords
   *
   * The output text does not contain lines starting with
   * #if X, #else and #endif
   *
   * If keyword X mathes in #if Y, it keeps lines until it hits
   * either #else or #endif. If it hits #else, it removes lines
   * until #endif
   *
   * The opposite is done if X does not match Y:
   * Remove until #else or #endif, keep between #else and #endif.
   *
   */
  static std::string processIfEndif(const std::string& text,
                                    const std::string& keyword);

 private:
  StringUtil();
  virtual ~StringUtil();

  // NonCopyable
  StringUtil(const StringUtil& c);
  StringUtil& operator=(const StringUtil& c);
};

#endif  // UTIL_STRINGUTIL_H
