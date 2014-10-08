#include <util/Log.h>

#include <cassert>

#include <util/Exception.h>
#include <util/LogManager.h>


Log::Log(const std::string& loggerName)
  : loggerName_(loggerName),
    streamLogLevel_(LogManager::LEVEL_DEBUG),
    streamLog_()
{
}

void Log::d(const std::string& message) const
{
  log(LogManager::LEVEL_DEBUG, message);
}

void Log::i(const std::string& message) const
{
  log(LogManager::LEVEL_INFO, message);
}

void Log::w(const std::string& message) const
{
  log(LogManager::LEVEL_WARNING, message);
}

void Log::e(const std::string& message) const
{
  log(LogManager::LEVEL_ERROR, message);
}

const Log& Log::d() const {
  assert(!streamLog_.rdbuf()->in_avail());  // LCOV_EXCL_LINE
  this->streamLogLevel_ = LogManager::LEVEL_DEBUG;
  return *this;
}

const Log& Log::i() const {
  assert(!streamLog_.rdbuf()->in_avail());  // LCOV_EXCL_LINE
  this->streamLogLevel_ = LogManager::LEVEL_INFO;
  return *this;
}

const Log& Log::w() const {
  assert(!streamLog_.rdbuf()->in_avail());  // LCOV_EXCL_LINE
  this->streamLogLevel_ = LogManager::LEVEL_WARNING;
  return *this;
}

const Log& Log::e() const {
  assert(!streamLog_.rdbuf()->in_avail());  // LCOV_EXCL_LINE
  this->streamLogLevel_ = LogManager::LEVEL_ERROR;
  return *this;
}

const Log& Log::level(LogManager::LogLevel logLevel) const {
  assert(!streamLog_.rdbuf()->in_avail());  // LCOV_EXCL_LINE
  this->streamLogLevel_ = logLevel;
  return *this;
}

Exception Log::exception(const std::string& message,
                         FunctionPtr func) const
{
#ifndef LOGGING_DISABLED
  this->e(message);
  if (func)
    this->e(func());
#endif
  return Exception(message);
}


void Log::log(LogManager::LogLevel logLevel,
              const std::string& message) const
{
#ifndef LOGGING_DISABLED
  LogManager::instance().log(logLevel, loggerName_, message);
#endif
}


Log::~Log()
{
}
