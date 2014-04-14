#include <util/StringUtil.h>

#include <sstream>

#include <boost/algorithm/string.hpp>


typedef std::vector<std::string> StringVector;

void StringUtil::split(const std::string &text,
                       StringVector& elements,
                       char delimiter)
{
  std::stringstream ss(text);
  std::string token;
  while (std::getline(ss, token, delimiter))
    elements.push_back(token);
}


StringVector StringUtil::split(const std::string &text,
                               char delimiter)
{
  StringVector elements;
  split(text, elements, delimiter);
  return elements;
}

std::string StringUtil::suffix(const std::string& text, size_t length)
{
  if (length >= text.size())
    return text;
  return text.substr(text.size() - length, length);
}

std::string StringUtil::prefix(const std::string& text, size_t length)
{
  return text.substr(0, length);
}


std::string StringUtil::ltrimc(const std::string& text)
{return boost::trim_left_copy(text);}
std::string StringUtil::rtrimc(const std::string& text)
{return boost::trim_right_copy(text);}
std::string StringUtil::trimc(const std::string& text)
{return boost::trim_copy(text);}

void StringUtil::ltrim(std::string& text)
{boost::trim_left(text);}
void StringUtil::rtrim(std::string& text)
{boost::trim_right(text);}
void StringUtil::trim(std::string& text)
{boost::trim(text);}
