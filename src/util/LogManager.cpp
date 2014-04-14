#include <util/LogManager.h>

#include <cassert>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

#include <util/BashColor.h>
#include <util/Clock.h>
#include <util/FileUtil.h>
#include <util/SDL.h>
#include <util/gitrev.h>


#ifdef __ANDROID__
#include <android/log.h>
#endif

LogManager::LogManager()
  : LogManager(LEVEL_NONE,
               LEVEL_NONE,
               COLORMODE_NONE)
{
}


LogManager::LogManager(LogLevel fileLogLevel,
                       LogLevel streamLogLevel,
                       ColorMode colorMode)
  : fileLogLevel_(fileLogLevel),
    streamLogLevel_(streamLogLevel),
    streamColorMode_(colorMode),
    logfilePath_("")
{
}


LogManager::~LogManager()
{
}


void LogManager::log(LogLevel level,
                     const std::string& loggerName,
                     const std::string& message) const
{
#ifndef LOGGING_DISABLED
  if (level < streamLogLevel_ &&
      level < fileLogLevel_)
    return;

  Clock clock;
  std::stringstream ss;

  ss << " "
     << CURRENT_GIT_REV << " "
     << logLevelAsString(level) << clock.getTimeStamp() << "  "
     << std::left << std::setw(18)
     << loggerName  << "  " << message;
  const std::string formatted(ss.str());

  if (level >= fileLogLevel_)
    log2File(formatted);

  if (level >= streamLogLevel_) {
    if (streamColorMode_ == COLORMODE_BASH ) {
      const std::string bashFormatted =
        BashColor::setColor(formatted, level);
      log2Stream(level, bashFormatted);
    }
    else {
      log2Stream(level, formatted);
    }
  }
#else
  (void)level; (void)loggerName; (void)message;
#endif
}


void LogManager::log2Stream(
      LogLevel level,
      const std::string& formatted) const
{
#ifndef LOG2STREAM_DISABLED
#ifdef __ANDROID__
  int priority = 0;
  switch (level) {
  case LEVEL_DEBUG:   priority = ANDROID_LOG_DEBUG;  break;
  case LEVEL_INFO:    priority = ANDROID_LOG_INFO;   break;
  case LEVEL_WARNING: priority = ANDROID_LOG_WARN;   break;
  case LEVEL_ERROR:   priority = ANDROID_LOG_ERROR;  break;
  case LEVEL_NONE:    priority = ANDROID_LOG_SILENT; break;
  }
  __android_log_write(priority, "SWL", formatted.c_str());
#else
  std::cout << formatted << std::endl;
  (void)level;
#endif
#else
  (void)formatted;
#endif
}

void LogManager::log2File(const std::string& formatted) const
{
#ifndef LOG2FILE_DISABLED
  if (logfilePath_.empty())
    return;
  FileUtil::append(logfilePath_, formatted);
#else
  (void)formatted;
#endif
}


LogManager::LogLevel LogManager::getFileLogLevel() const
{
  return fileLogLevel_;
}


void LogManager::setFileLogLevel(LogManager::LogLevel fileLogLevel)
{
  fileLogLevel_ = fileLogLevel;
}


LogManager::LogLevel LogManager::getStreamLogLevel() const
{
  return streamLogLevel_;
}


void LogManager::setStreamLogLevel(LogLevel streamLogLevel)
{
  streamLogLevel_ = streamLogLevel;
}


const std::string& LogManager::getLogfilePath() const
{
  return logfilePath_;
}


void LogManager::setLogfilePath(std::string logfilePath)
{
  FileUtil::write(logfilePath, "");
  logfilePath_ = logfilePath;
}


LogManager::ColorMode LogManager::getStreamColorMode() const
{
  return streamColorMode_;
}


void LogManager::setStreamColorMode(LogManager::ColorMode streamColorMode)
{
  streamColorMode_ = streamColorMode;
}


std::string LogManager::logLevelAsString(LogLevel level)
{
  switch (level) {
  case LEVEL_DEBUG:
    return std::string("DEBUG   ");
    break;
  case LEVEL_INFO:
    return std::string("INFO    ");
    break;
  case LEVEL_WARNING:
    return std::string("WARNING ");
    break;
  case LEVEL_ERROR:
    return std::string("ERROR   ");
    break;
  case LEVEL_NONE:
    assert(false && "Should never be queried");
    return "";
    break;
  default:
    return "";
  }
}


LogManager& LogManager::instance()
{
  static LogManager instance(LogManager::LEVEL_DEBUG,
                             LogManager::LEVEL_DEBUG,
                             LogManager::COLORMODE_BASH);
  return instance;
}
