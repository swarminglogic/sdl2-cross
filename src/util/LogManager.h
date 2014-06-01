#ifndef UTIL_LOGMANAGER_H
#define UTIL_LOGMANAGER_H

#include <iosfwd>
#include <string>


/**
 * LogManager class.
 *
 * Manages settings regarding log-levels, and log file name.
 *
 * TODO swarminglogic, 2014-05-29: Separate Android and Stream logging,
 *                               common interface, virtualization for selection.
 * @author SwarmingLogic (Roald Fernandez)
 */
class LogManager
{
public:
  enum LogLevel {
    LEVEL_DEBUG = 0,
    LEVEL_INFO,
    LEVEL_WARNING,
    LEVEL_ERROR,
    LEVEL_NONE
  };

  enum ColorMode {
    COLORMODE_NONE = 0,
    COLORMODE_BASH
  };


  LogManager();

  LogManager(LogLevel fileLogLevel,
             LogLevel streamLogLevel,
             ColorMode colorMode);

  void log(LogLevel level,
           const std::string& loggerName,
           const std::string& message) const;

  void logColumnHeaders() const;

  virtual ~LogManager();

  LogLevel getFileLogLevel() const;
  void setFileLogLevel(LogLevel fileLogLevel);
  LogLevel getStreamLogLevel() const;
  void setStreamLogLevel(LogLevel streamLogLevel);
  const std::string& getLogfilePath() const;
  void setLogfilePath(std::string logfilePath);

  LogManager::ColorMode getStreamColorMode() const;
  void setStreamColorMode(LogManager::ColorMode streamColorMode);

  void setStreamTarget(std::ostream& out);

  static LogManager& instance();

private:
  void log2Stream(LogLevel level,
                  const std::string& formatted) const;
  void log2AndroidStream(LogLevel level,
                         const std::string& formatted) const;

  void log2File(const std::string& formatted) const;

  static std::string logLevelAsString(LogLevel level);

  LogLevel fileLogLevel_;
  LogLevel streamLogLevel_;
  ColorMode streamColorMode_;

  std::string logfilePath_;
  const std::string androidLoggerTag_;
  mutable std::ostream* out_;

  // NonCopyable
  LogManager(const LogManager& c);
  LogManager& operator=(const LogManager& c);
};

#endif
