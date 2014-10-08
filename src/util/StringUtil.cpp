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

std::string StringUtil::suffix(const std::string& text, int length)
{
  if (length >= 0) {
    if (length >= static_cast<int>(text.size()))
      return text;
    return text.substr(text.size() - length, length);
  } else {
    if (-length >= static_cast<int>(text.size()))
      return "";
    else
      return text.substr(-length, text.size());
  }
}

std::string StringUtil::prefix(const std::string& text, int length)
{
  if (length >= 0) {
    return text.substr(0, length);
  } else {
    if (-length >= static_cast<int>(text.size()))
      return "";
    else
      return text.substr(0, text.size() + length);
  }
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


void StringUtil::prepend(const std::string& pre,
                         std::string& text)
{
  text.insert(0, pre);
}


std::string StringUtil::prependc(const std::string& pre,
                                 const std::string& text)
{
  std::string ret {pre};
  ret.append(text);
  return ret;
}



std::string StringUtil::processIfEndif(const std::string& text,
                                       const std::string& keyword)
{
  enum State {
    ADD_UNTIL_IF,
    ADD_UNTIL_ENDIF,
    ADD_UNTIL_ELSE_ENDIF,
    SKIP_UNTIL_ENDIF,
    SKIP_UNTIL_ELSE_ENDIF
  };

  State state = ADD_UNTIL_IF;
  std::string ret;
  ret.reserve(text.size());

  StringVector lines = split(text, '\n');
  for (auto& line : lines) {
    switch (state) {
    case ADD_UNTIL_IF:
      if (prefix(trimc(line), 3) == "#if") {
        if (suffix(trimc(line), -4) == keyword)
          state = ADD_UNTIL_ELSE_ENDIF;
        else
          state = SKIP_UNTIL_ELSE_ENDIF;
        continue;
      }
      break;

    case ADD_UNTIL_ELSE_ENDIF:
      if (prefix(trimc(line), 5) == "#else") {
        state = SKIP_UNTIL_ENDIF;
        continue;
      } else if (prefix(trimc(line), 6) == "#endif") {
        state = ADD_UNTIL_IF;
        continue;
      }
      break;

    case SKIP_UNTIL_ENDIF:
      if (prefix(trimc(line), 6) == "#endif") {
        state = ADD_UNTIL_IF;
      }
      continue;

    case SKIP_UNTIL_ELSE_ENDIF:
      if (prefix(trimc(line), 5) == "#else") {
        state = ADD_UNTIL_ENDIF;
      } else if (prefix(trimc(line), 6) == "#endif") {
        state = ADD_UNTIL_IF;
      }
      continue;

    case ADD_UNTIL_ENDIF:
      if (prefix(trimc(line), 6) == "#endif") {
        state = ADD_UNTIL_IF;
        continue;
      }
      break;
    }
    ret.append(line);
    ret.append("\n");
  }

  return ret;
}
