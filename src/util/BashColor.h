#ifndef UTIL_BASHCOLOR_H
#define UTIL_BASHCOLOR_H

#include <string>

#include <util/LogManager.h>


/**
 * BashColor class.
 *
 * @author SwarmingLogic (Roald Fernandez)
 */
class BashColor
{
 public:
  static std::string setColor(const std::string& text,
                              LogManager::LogLevel level);

 private:
  enum Color {
    NONE = 0,
    BLACK, RED, GREEN,
    YELLOW, BLUE, MAGENTA,
    CYAN, WHITE
  };

  static BashColor::Color getLogLevelColor(LogManager::LogLevel level);

  static std::string setColor(const std::string& text,
                              BashColor::Color foregroundColor);

  static std::string makeColorTagVal(int foreground);
  static std::string makeColorTag(BashColor::Color foreground);

  BashColor();
  BashColor(const BashColor& c);
  BashColor& operator=(const BashColor& c);
};


#endif  // UTIL_BASHCOLOR_H
