#ifndef UTIL_CLOCK_H
#define UTIL_CLOCK_H

#include <ctime>
#include <string>


/**
 * Static Clock class.
 * This is a absolute timer, which measures the wall time.
 * Wrapper for some chrono.
 *
 * @author SwarmingLogic (Roald Fernandez)
 */
class Clock
{
public:
  /**
   * Generates a timestamp with the following format "YYYY-MM-DD HH:MM:SS'
   * using a 24 hour.
   */
  std::string getTimeStamp();

  /**
   * Call this function to update to current time.
   */
  void update();

  int getYear() const;
  int getMonth() const;
  int getDay() const;
  int getHours() const;
  int getMinutes() const;
  int getSeconds() const;

  static std::time_t now();

  Clock();
  virtual ~Clock();

private:

  std::tm time_;

  // NonCopyable
  Clock(const Clock& c);
  Clock& operator=(const Clock& c);
};

#endif
