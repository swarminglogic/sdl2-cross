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


  static std::string suffix(const std::string& text, size_t length);
  static std::string prefix(const std::string& text, size_t length);

  // Creating a copy of the string
  static std::string ltrimc(const std::string& text);
  static std::string rtrimc(const std::string& text);
  static std::string trimc(const std::string& text);

  // Trimming in place
  static void ltrim(std::string& text);
  static void rtrim(std::string& text);
  static void trim(std::string& text);

private:
  StringUtil();
  virtual ~StringUtil();

  // NonCopyable
  StringUtil(const StringUtil& c);
  StringUtil& operator=(const StringUtil& c);
};

#endif
