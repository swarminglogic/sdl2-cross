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
  enum Color {
    NONE = 0,
    BLACK, RED, GREEN,
    YELLOW, BLUE, MAGENTA,
    CYAN, WHITE
  };

  static std::string setColor(const std::string& text,
                              LogManager::LogLevel level);

  ~BashColor();

private:
  static BashColor::Color getLogLevelColor(LogManager::LogLevel level);

  static std::string setColor(const std::string& text,
                              BashColor::Color foregroundColor,
                              BashColor::Color backgroundColor = NONE);

  static std::string makeColorTagVal(int foreground, int background);
  static std::string makeColorTag(BashColor::Color foreground,
                                  BashColor::Color background);

  BashColor();
  BashColor(const BashColor& c);
  BashColor& operator=(const BashColor& c);
};


#endif
