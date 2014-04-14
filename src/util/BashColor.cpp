#include <util/BashColor.h>

#include <sstream>


BashColor::~BashColor()
{
}


std::string BashColor::setColor(const std::string& text,
                                LogManager::LogLevel level)
{
  return setColor(text, getLogLevelColor(level));
}


std::string BashColor::setColor(const std::string& text,
                                BashColor::Color fg,
                                BashColor::Color bg)
{
  return makeColorTag(fg,bg) + text + makeColorTag(BashColor::NONE,
                                                   BashColor::NONE);
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


std::string BashColor::makeColorTagVal(int foreground, int background)
{
  std::string s = "\033[";
  if (foreground == 0 && background == 0) {
    s += "0";
    return s + "m";
  }
  if (foreground != 0) {
    std::stringstream ss;
    ss << 29 + foreground;
    s += ss.str();
    if (background != 0)
      s += ";";
  }
  if (background != 0) {
    std::stringstream ss;
    ss << 39 + background;
    s += ss.str();
  }
  return s + "m";
}


std::string BashColor::makeColorTag(BashColor::Color foreground,
                                    BashColor::Color background)
{
  return makeColorTagVal(static_cast<int>(foreground),
                         static_cast<int>(background));
}
