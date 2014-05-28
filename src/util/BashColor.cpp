#include <util/BashColor.h>

#include <sstream>


std::string BashColor::setColor(const std::string& text,
                                LogManager::LogLevel level)
{
  return setColor(text, getLogLevelColor(level));
}


std::string BashColor::setColor(const std::string& text,
                                BashColor::Color fg)
{
  return makeColorTag(fg) + text + makeColorTag(BashColor::NONE);
}


BashColor::Color BashColor::getLogLevelColor(LogManager::LogLevel level)
{
  switch (level) {
  case LogManager::LEVEL_DEBUG:   return BashColor::CYAN;
  case LogManager::LEVEL_INFO:    return BashColor::WHITE;
  case LogManager::LEVEL_WARNING: return BashColor::YELLOW;
  case LogManager::LEVEL_ERROR:   return BashColor::RED;

  case LogManager::LEVEL_NONE:
  default:
    return BashColor::NONE;
  }
}


std::string BashColor::makeColorTagVal(int foreground)
{
  std::string s = "\033[";
  if (foreground == 0) {
    s += "0";
    return s + "m";
  }
  if (foreground != 0) {
    std::stringstream ss;
    ss << 29 + foreground;
    s += ss.str();
  }
  return s + "m";
}


std::string BashColor::makeColorTag(BashColor::Color foreground)
{
  return makeColorTagVal(static_cast<int>(foreground));
}
