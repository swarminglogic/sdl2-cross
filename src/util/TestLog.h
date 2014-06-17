#ifndef UTIL_TESTLOG_H
#define UTIL_TESTLOG_H

#include <sstream>

#include <math/Pointf.h>
#include <math/Rect.h>
#include <math/Rectf.h>
#include <math/Size.h>
#include <util/Exception.h>
#include <util/Log.h>
#include <util/LogManager.h>

#include <cxxtest/TestSuite.h>


/**
 * Test for the Log class.
 *
 * The log class relies on a singleton, which makes it important to
 * set its state, whenever testing.
 *
 * @author SwarmingLogic
 */
class TestLog : public CxxTest::TestSuite
{
public:
  void setUp() {
    LogManager& lm = LogManager::instance();
    lm.setFileLogLevel(LogManager::LEVEL_NONE);
    lm.setStreamLogLevel(LogManager::LEVEL_DEBUG);
    (void)getLogAndClear();
    lm.setStreamTarget(ss_);
  }

  void testBasics()
  {
    Log log_("FooTest1");
    TS_ASSERT(getLogAndClear().empty());

    log_.d("dbg1");
    const std::string retDgb = getLogAndClear();
    TS_ASSERT(isSubstring(retDgb, "dbg1"));
    TS_ASSERT(isSubstring(retDgb, "FooTest1"));
    TS_ASSERT(isSubstring(retDgb, "DEBUG"));
    TS_ASSERT(!isSubstring(retDgb, "INFO"));
    TS_ASSERT(!isSubstring(retDgb, "WARN"));
    TS_ASSERT(!isSubstring(retDgb, "ERROR"));
    log_.d() << "dbg1" << Log::end;
    const std::string retDgb2 = getLogAndClear();
    TS_ASSERT_EQUALS(retDgb, retDgb2);


    log_.i("info1");
    const std::string retInfo = getLogAndClear();
    TS_ASSERT(isSubstring(retDgb, "FooTest1"));
    TS_ASSERT(isSubstring(retInfo, "info1"));
    TS_ASSERT(!isSubstring(retInfo, "DEBUG"));
    TS_ASSERT(isSubstring(retInfo, "INFO"));
    TS_ASSERT(!isSubstring(retInfo, "WARN"));
    TS_ASSERT(!isSubstring(retInfo, "ERROR"));
    log_.i() << "info1" << Log::end;
    const std::string retInfo2 = getLogAndClear();
    TS_ASSERT_EQUALS(retInfo, retInfo2);


    log_.w("warn1");
    const std::string retWarn = getLogAndClear();
    TS_ASSERT(isSubstring(retDgb, "FooTest1"));
    TS_ASSERT(isSubstring(retWarn, "warn1"));
    TS_ASSERT(!isSubstring(retWarn, "DEBUG"));
    TS_ASSERT(!isSubstring(retWarn, "INFO"));
    TS_ASSERT(isSubstring(retWarn, "WARN"));
    TS_ASSERT(!isSubstring(retWarn, "ERROR"));
    log_.w() << "warn1" << Log::end;
    const std::string retWarn2 = getLogAndClear();
    TS_ASSERT_EQUALS(retWarn, retWarn2);


    log_.e("err1");
    const std::string retErr = getLogAndClear();
    TS_ASSERT(isSubstring(retDgb, "FooTest1"));
    TS_ASSERT(isSubstring(retErr, "err1"));
    TS_ASSERT(!isSubstring(retErr, "DEBUG"));
    TS_ASSERT(!isSubstring(retErr, "INFO"));
    TS_ASSERT(!isSubstring(retErr, "WARN"));
    TS_ASSERT(isSubstring(retErr, "ERROR"));
    log_.e() << "err1" << Log::end;
    const std::string retErr2 = getLogAndClear();
    TS_ASSERT_EQUALS(retErr, retErr2);
  }


  void testStreamConversions()
  {
    Log log_("BarTest1");
    TS_ASSERT(getLogAndClear().empty());

    log_.d("dbg1");
    const std::string retDgb = getLogAndClear();
    TS_ASSERT(!isSubstring(retDgb, "FooTest1"));
    TS_ASSERT(isSubstring(retDgb, "BarTest1"));

    {
      // const std::string&
      const std::string myConstString("Hurray!");
      log_.i() << myConstString << Log::end;
      const std::string retConstStr = getLogAndClear();
      TS_ASSERT(isSubstring(retConstStr, myConstString));
    }

    {
      // int
      const int myInt = 12342;
      log_.i() << myInt<< Log::end;
      const std::string retInt = getLogAndClear();
      TS_ASSERT(isSubstring(retInt, "12342"));
    }

    {
      // uint
      const unsigned int myUint = 4242u;
      log_.i() << myUint<< Log::end;
      const std::string retUint = getLogAndClear();
      TS_ASSERT(isSubstring(retUint, "4242"));
    }

    {
      // float
      const float myFloat = 42.42f;
      log_.i() << myFloat<< Log::end;
      const std::string retFloat = getLogAndClear();
      TS_ASSERT(isSubstring(retFloat, "42.42"));
    }

    {
      // Pointf
      const Pointf myPoint(3.42f, 9.23f);
      log_.i() << myPoint << Log::end;
      const std::string retPoint = getLogAndClear();
      TS_ASSERT(isSubstring(retPoint, "(3.42, 9.23) [Pointf]"));
    }

    {
      // Size
      const Size mySize(22, 95);
      log_.i() << mySize << Log::end;
      const std::string retSize = getLogAndClear();
      TS_ASSERT(isSubstring(retSize, "(22, 95) [Size]"));
    }

    {
      // Rect
      const Rect myRect(42, 23, 85, 11);
      log_.i() << myRect << Log::end;
      const std::string retRect = getLogAndClear();
      TS_ASSERT(isSubstring(retRect, "(42, 23, 85, 11) [Rect]"));
    }

    {
      // Rectf
      const Rectf myRect(42.23f, 23.85f, 85.11f, 11.24f);
      log_.i() << myRect << Log::end;
      const std::string retRect = getLogAndClear();
      TS_ASSERT(isSubstring(retRect, "(42.23, 23.85, 85.11, 11.24) [Rectf]"));
    }

    {
      log_.level(LogManager::LEVEL_ERROR);
      log_ << "Blue ";
      log_ << 42 << " hut!" << Log::end;
      const std::string ret = getLogAndClear();
      TS_ASSERT(isSubstring(ret, "ERROR"));
      TS_ASSERT(isSubstring(ret, "Blue 42 hut!"));

      log_.level(LogManager::LEVEL_WARNING);
      log_ << "tst warn" << Log::end;
      const std::string ret2 = getLogAndClear();
      TS_ASSERT(isSubstring(ret2, "WARNING"));
      TS_ASSERT(isSubstring(ret2, "tst warn"));
    }
  }

  void testLogException()
  {
    Log log_("ExceptionTest1");
    const std::string exceptionString = "Throw it around";
    bool requiredSwitch = false;
    try {
      throw log_.exception(exceptionString, fakeErrorFunction);
      TS_ASSERT(false);
    }
    catch (const Exception& exc) {
      TS_ASSERT_EQUALS(exc.what(), exceptionString);
      const std::string retExc = getLogAndClear();
      TS_ASSERT(isSubstring(retExc, exceptionString));
      TS_ASSERT(isSubstring(retExc, "potatoes"));
      requiredSwitch = true;
    }

    TS_ASSERT(requiredSwitch);
  }

private:
  static const char* fakeErrorFunction() {
    static const std::string err("potatoes");
    return err.c_str();
  }

  bool isSubstring(const std::string& hay,
                   const std::string& needle) {
    return hay.find(needle) != std::string::npos;
  }

  std::string getLogAndClear() {
    const std::string tmp = ss_.str();
    ss_.clear();
    ss_.str(std::string(""));
    return tmp;
  }

  std::stringstream ss_;
};

#endif
