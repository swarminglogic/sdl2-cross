#ifndef UTIL_TESTLOGMANAGER_H
#define UTIL_TESTLOGMANAGER_H

#include <sstream>

#include <util/File.h>
#include <util/FileUtil.h>
#include <util/LogManager.h>

#include <cxxtest/TestSuite.h>


/**
 * Test for the LogManager class.
 *
 * @author SwarmingLogic
 */
class TestLogManager : public CxxTest::TestSuite
{
 public:
  void testGettersSetters()
  {
    LogManager lmb;
    TS_ASSERT_EQUALS(lmb.getFileLogLevel(), LogManager::LEVEL_NONE);
    TS_ASSERT_EQUALS(lmb.getStreamLogLevel(), LogManager::LEVEL_NONE);
    TS_ASSERT_EQUALS(lmb.getStreamColorMode(), LogManager::COLORMODE_NONE);


    LogManager lm(LogManager::LEVEL_NONE,
                  LogManager::LEVEL_DEBUG,
                  LogManager::COLORMODE_BASH);
    TS_ASSERT_EQUALS(lm.getFileLogLevel(), LogManager::LEVEL_NONE);
    TS_ASSERT_EQUALS(lm.getStreamLogLevel(), LogManager::LEVEL_DEBUG);
    TS_ASSERT_EQUALS(lm.getStreamColorMode(), LogManager::COLORMODE_BASH);

    lm.setFileLogLevel(LogManager::LEVEL_INFO);
    TS_ASSERT_EQUALS(lm.getFileLogLevel(), LogManager::LEVEL_INFO);
    lm.setStreamLogLevel(LogManager::LEVEL_ERROR);
    TS_ASSERT_EQUALS(lm.getStreamLogLevel(), LogManager::LEVEL_ERROR);
    lm.setStreamColorMode(LogManager::COLORMODE_NONE);
    TS_ASSERT_EQUALS(lm.getStreamColorMode(), LogManager::COLORMODE_NONE);

    const std::string logfilePath = "./logfile.log";
    lm.setLogfilePath(logfilePath);
    TS_ASSERT_EQUALS(logfilePath, lm.getLogfilePath());

    const std::string logfilePath2 = "./foo.log";
    lm.setLogfilePath(logfilePath2);
    TS_ASSERT_EQUALS(logfilePath2, lm.getLogfilePath());
  }

  void testStreamLogging()
  {
#ifdef __ANDROID__
    TS_SKIP("Not yet implemented for android.");
#endif
    LogManager lm(LogManager::LEVEL_NONE,
                  LogManager::LEVEL_DEBUG,
                  LogManager::COLORMODE_NONE);
    std::stringstream ss;
    lm.setStreamTarget(ss);
    lm.logColumnHeaders();
    std::string res = ss.str();
    TS_ASSERT(isSusbtring(res, "GIT ID"));
    TS_ASSERT(isSusbtring(res, "TIME"));
    TS_ASSERT(isSusbtring(res, "LOGGER NAME"));
    TS_ASSERT(isSusbtring(res, "MESSAGE"));
  }


  void testStreamLevels()
  {
#ifdef __ANDROID__
    TS_SKIP("Not yet implemented for android.");
#endif
    LogManager lm(LogManager::LEVEL_NONE,
                  LogManager::LEVEL_DEBUG,
                  LogManager::COLORMODE_NONE);
    std::stringstream ss;
    lm.setStreamTarget(ss);

    // Stream level DEBUG: ALL
    lm.setStreamLogLevel(LogManager::LEVEL_DEBUG);
    TS_ASSERT(!isSusbtring(ss.str(), "dgb1"));
    lm.log(LogManager::LEVEL_DEBUG, "header", "dgb1");
    TS_ASSERT(isSusbtring(ss.str(), "dgb1"));
    lm.log(LogManager::LEVEL_INFO, "header", "info1");
    TS_ASSERT(isSusbtring(ss.str(), "info1"));
    lm.log(LogManager::LEVEL_WARNING, "header", "warn1");
    TS_ASSERT(isSusbtring(ss.str(), "warn1"));
    lm.log(LogManager::LEVEL_ERROR, "header", "err1");
    TS_ASSERT(isSusbtring(ss.str(), "err1"));

    // Stream level INFO: INFO, WARNING, ERROR
    lm.setStreamLogLevel(LogManager::LEVEL_INFO);
    TS_ASSERT(!isSusbtring(ss.str(), "dgb2"));
    lm.log(LogManager::LEVEL_DEBUG, "header", "dgb2");
    TS_ASSERT(!isSusbtring(ss.str(), "dgb2"));
    lm.log(LogManager::LEVEL_INFO, "header", "info2");
    TS_ASSERT(isSusbtring(ss.str(), "info2"));
    lm.log(LogManager::LEVEL_WARNING, "header", "warn2");
    TS_ASSERT(isSusbtring(ss.str(), "warn2"));
    lm.log(LogManager::LEVEL_ERROR, "header", "err2");
    TS_ASSERT(isSusbtring(ss.str(), "err2"));

    // Stream level WARNING: WARNING, ERROR
    lm.setStreamLogLevel(LogManager::LEVEL_WARNING);
    TS_ASSERT(!isSusbtring(ss.str(), "dgb3"));
    lm.log(LogManager::LEVEL_DEBUG, "header", "dgb3");
    TS_ASSERT(!isSusbtring(ss.str(), "dgb3"));
    lm.log(LogManager::LEVEL_INFO, "header", "info3");
    TS_ASSERT(!isSusbtring(ss.str(), "info3"));
    lm.log(LogManager::LEVEL_WARNING, "header", "warn3");
    TS_ASSERT(isSusbtring(ss.str(), "warn3"));
    lm.log(LogManager::LEVEL_ERROR, "header", "err3");
    TS_ASSERT(isSusbtring(ss.str(), "err3"));

    // Stream level ERROR: ERROR
    lm.setStreamLogLevel(LogManager::LEVEL_ERROR);
    TS_ASSERT(!isSusbtring(ss.str(), "dgb4"));
    lm.log(LogManager::LEVEL_DEBUG, "header", "dgb4");
    TS_ASSERT(!isSusbtring(ss.str(), "dgb4"));
    lm.log(LogManager::LEVEL_INFO, "header", "info4");
    TS_ASSERT(!isSusbtring(ss.str(), "info4"));
    lm.log(LogManager::LEVEL_WARNING, "header", "warn4");
    TS_ASSERT(!isSusbtring(ss.str(), "warn4"));
    lm.log(LogManager::LEVEL_ERROR, "header", "err4");
    TS_ASSERT(isSusbtring(ss.str(), "err4"));
  }

  void testFileLogging()
  {
#ifdef __ANDROID__
    TS_SKIP("Not yet implemented for android.");
#endif
    File logfile("./tmplogfile.log");
    LogManager lm(LogManager::LEVEL_DEBUG,
                  LogManager::LEVEL_NONE,
                  LogManager::COLORMODE_NONE);
    TS_ASSERT(!logfile.exists());
    lm.setLogfilePath(logfile.getFilename());
    TS_ASSERT(!logfile.exists());

    lm.logColumnHeaders();
    TS_ASSERT(logfile.exists());

    std::string res = logfile.read();
    TS_ASSERT(isSusbtring(res, "GIT ID"));
    TS_ASSERT(isSusbtring(res, "TIME"));
    TS_ASSERT(isSusbtring(res, "LOGGER NAME"));
    TS_ASSERT(isSusbtring(res, "MESSAGE"));


    // Stream level DEBUG: ALL
    lm.setFileLogLevel(LogManager::LEVEL_DEBUG);
    TS_ASSERT(!logfile.update());
    TS_ASSERT(!isSusbtring(logfile.read(), "dgb1"));
    lm.log(LogManager::LEVEL_DEBUG, "header", "dgb1");
    TS_ASSERT(logfile.update());
    TS_ASSERT(isSusbtring(logfile.read(), "dgb1"));
    TS_ASSERT(!logfile.update());
    lm.log(LogManager::LEVEL_INFO, "header", "info1");
    TS_ASSERT(logfile.update());
    TS_ASSERT(isSusbtring(logfile.read(), "info1"));
    TS_ASSERT(!logfile.update());
    lm.log(LogManager::LEVEL_WARNING, "header", "warn1");
    TS_ASSERT(logfile.update());
    TS_ASSERT(isSusbtring(logfile.read(), "warn1"));
    TS_ASSERT(!logfile.update());
    lm.log(LogManager::LEVEL_ERROR, "header", "err1");
    TS_ASSERT(logfile.update());
    TS_ASSERT(isSusbtring(logfile.read(), "err1"));
    TS_ASSERT(!logfile.update());

    // Stream level INFO: INFO,WARN,ERR
    lm.setFileLogLevel(LogManager::LEVEL_INFO);
    TS_ASSERT(!logfile.update());
    TS_ASSERT(!isSusbtring(logfile.read(), "dgb2"));
    lm.log(LogManager::LEVEL_DEBUG, "header", "dgb2");
    TS_ASSERT(!logfile.update());
    TS_ASSERT(!isSusbtring(logfile.read(), "dgb2"));
    TS_ASSERT(!logfile.update());
    lm.log(LogManager::LEVEL_INFO, "header", "info2");
    TS_ASSERT(logfile.update());
    TS_ASSERT(isSusbtring(logfile.read(), "info2"));
    TS_ASSERT(!logfile.update());
    lm.log(LogManager::LEVEL_WARNING, "header", "warn2");
    TS_ASSERT(logfile.update());
    TS_ASSERT(isSusbtring(logfile.read(), "warn2"));
    TS_ASSERT(!logfile.update());
    lm.log(LogManager::LEVEL_ERROR, "header", "err2");
    TS_ASSERT(logfile.update());
    TS_ASSERT(isSusbtring(logfile.read(), "err2"));
    TS_ASSERT(!logfile.update());

    // Stream level WARN: WARN,ERR
    lm.setFileLogLevel(LogManager::LEVEL_WARNING);
    TS_ASSERT(!logfile.update());
    TS_ASSERT(!isSusbtring(logfile.read(), "dgb3"));
    lm.log(LogManager::LEVEL_DEBUG, "header", "dgb3");
    TS_ASSERT(!logfile.update());
    TS_ASSERT(!isSusbtring(logfile.read(), "dgb3"));
    TS_ASSERT(!logfile.update());
    lm.log(LogManager::LEVEL_INFO, "header", "info3");
    TS_ASSERT(!logfile.update());
    TS_ASSERT(!isSusbtring(logfile.read(), "info3"));
    TS_ASSERT(!logfile.update());
    lm.log(LogManager::LEVEL_WARNING, "header", "warn3");
    TS_ASSERT(logfile.update());
    TS_ASSERT(isSusbtring(logfile.read(), "warn3"));
    TS_ASSERT(!logfile.update());
    lm.log(LogManager::LEVEL_ERROR, "header", "err3");
    TS_ASSERT(logfile.update());
    TS_ASSERT(isSusbtring(logfile.read(), "err3"));
    TS_ASSERT(!logfile.update());


    // Stream level ERR:,ERR
    lm.setFileLogLevel(LogManager::LEVEL_ERROR);
    TS_ASSERT(!logfile.update());
    TS_ASSERT(!isSusbtring(logfile.read(), "dgb4"));
    lm.log(LogManager::LEVEL_DEBUG, "header", "dgb4");
    TS_ASSERT(!logfile.update());
    TS_ASSERT(!isSusbtring(logfile.read(), "dgb4"));
    TS_ASSERT(!logfile.update());
    lm.log(LogManager::LEVEL_INFO, "header", "info4");
    TS_ASSERT(!logfile.update());
    TS_ASSERT(!isSusbtring(logfile.read(), "info4"));
    TS_ASSERT(!logfile.update());
    lm.log(LogManager::LEVEL_WARNING, "header", "warn4");
    TS_ASSERT(!logfile.update());
    TS_ASSERT(!isSusbtring(logfile.read(), "warn4"));
    TS_ASSERT(!logfile.update());
    lm.log(LogManager::LEVEL_ERROR, "header", "err4");
    TS_ASSERT(logfile.update());
    TS_ASSERT(isSusbtring(logfile.read(), "err4"));
    TS_ASSERT(!logfile.update());

    TS_ASSERT(FileUtil::remove(logfile.getFilename()));
  }

  void testStaticObject()
  {
    // TODO swarminglogic, 2014-05-29: Consider replacing the singleton usage
    LogManager& lm = LogManager::instance();
    // State is undeterministic, because it is a global variable.
    // Set the state now, and do a basic test.
    lm.setFileLogLevel(LogManager::LEVEL_DEBUG);
    lm.setStreamLogLevel(LogManager::LEVEL_DEBUG);
    lm.setStreamColorMode(LogManager::COLORMODE_BASH);

    TS_ASSERT_EQUALS(lm.getFileLogLevel(), LogManager::LEVEL_DEBUG);
    TS_ASSERT_EQUALS(lm.getStreamLogLevel(), LogManager::LEVEL_DEBUG);
    TS_ASSERT_EQUALS(lm.getStreamColorMode(), LogManager::COLORMODE_BASH);

    LogManager& lm2 = LogManager::instance();
    lm.setFileLogLevel(LogManager::LEVEL_NONE);
    TS_ASSERT_EQUALS(lm.getFileLogLevel(), LogManager::LEVEL_NONE);
    TS_ASSERT_EQUALS(lm2.getFileLogLevel(), LogManager::LEVEL_NONE);
    TS_ASSERT(&lm == &lm2);
  }

 private:
  bool isSusbtring(const std::string& hay,
                   const std::string& needle) {
    return hay.find(needle) != std::string::npos;
  }
};

#endif  // UTIL_TESTLOGMANAGER_H
