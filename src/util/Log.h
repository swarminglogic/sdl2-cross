#ifndef UTIL_LOG_H
#define UTIL_LOG_H

#include <iomanip>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include <math/Pointf.h>
#include <math/Rect.h>
#include <math/Rectf.h>
#include <math/Size.h>
#include <util/Exception.h>
#include <util/LogManager.h>


/**
 * Log class.
 * Used for writing log calls.
 *
 * Allows streaming logging. For example:
 * log_.i() << "Log this numer: " << 42 << Log::end;
 *
 * Change stream log level using d(), i(), w(), e() for
 * the respective debug, info, warning, error levels.
 *
 * Finalize log entry by passing Log::end; (important!)
 *
 * @author SwarmingLogic (Roald Fernandez)
 */
class Log
{
 public:
  virtual ~Log();
  explicit Log(const std::string& loggerName);

  // Log functions for the various log levels
  void d(const std::string& message) const;
  void i(const std::string& message) const;
  void w(const std::string& message) const;
  void e(const std::string& message) const;

  typedef const char*(*FunctionPtr)();

  /**
   * Logs an exception.
   * It optionally calls and logs result of a function.
   * Typically, it does not itself throw exception.
   *
   * @param message   Error message to log.
   * @param func      optional function pointer call and log.
   */
  Exception exception(const std::string& message,
                      FunctionPtr func = nullptr) const;

  const Log& d() const;
  const Log& i() const;
  const Log& w() const;
  const Log& e() const;
  const Log& level(LogManager::LogLevel logLevel) const;

  // Special static function that is uses as a global instance to pass to the
  // logger to mark the end of the line to log.
  // It allows: logger << "Hello" << Log::end;
  // The function itself is never actually executed.
  static void end(){}  // LCOV_EXCL_LINE

  friend const Log& operator<<(const Log& lhs,
                               void(*pf)() ) {
    if (pf == end) {
      lhs.log(lhs.streamLogLevel_, lhs.streamLog_.str());
      lhs.streamLog_.str(std::string());
    }
    return lhs;
  }

  template<class T>
  friend const Log& operator<<(const Log& lhs, const T& t)
  { lhs.streamLog_ << t;
    return lhs; }

  friend const Log& operator<<(const Log& lhs, const Pointf& p)
  { lhs.streamLog_ << "(" << p.x() << ", " << p.y() << ") [Pointf]";
    return lhs; }
  friend const Log& operator<<(const Log& lhs, const Size& p)
  { lhs.streamLog_ << "(" << p.w() << ", " << p.h() << ") [Size]";
    return lhs; }
  friend const Log& operator<<(const Log& lhs, const Rect& p)
  { lhs.streamLog_ << "(" << p.x() << ", " << p.y() << ", "
                   << p.w() << ", " << p.h() << ") [Rect]";
    return lhs; }
  friend const Log& operator<<(const Log& lhs, const Rectf& p)
  { lhs.streamLog_ << "(" << p.x() << ", " << p.y() << ", "
                   << p.w() << ", " << p.h() << ") [Rectf]";
    return lhs; }

  template<class T>
  friend const Log& operator<<(const Log& lhs, const std::vector<T>& data)
  {
    if (!data.empty()) {
      lhs.streamLog_ << "std::vector<" <<  typeid(data[0]).name() << ">"
                     << "  #elements: " << data.size();
      for (size_t i = 0 ; i < data.size() ; ++i)
        lhs.streamLog_ << "\n\t[" << i << "]: " << data[i];
    }
    return lhs;
  }


 private:
  // Can only be instantiated through non-default constructor
  Log();

  void log(LogManager::LogLevel logLevel,
           const std::string& message) const;

  // Denotes globally set log levels.
  const std::string loggerName_;

  mutable LogManager::LogLevel streamLogLevel_;
  mutable std::stringstream streamLog_;

  // NonCopyable
  Log(const Log& c);
  Log& operator=(const Log& c);
};

#endif  // UTIL_LOG_H
