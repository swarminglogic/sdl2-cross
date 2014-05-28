#ifndef UTIL_TESTBASHCOLOR_H
#define UTIL_TESTBASHCOLOR_H

#include <util/BashColor.h>

#include <cxxtest/TestSuite.h>


/**
 * Test for the BashColor class.
 *
 * @author SwarmingLogic
 */
class TestBashColor : public CxxTest::TestSuite
{
public:

  void testFunctionName()
  {
    const std::string txt("Hello");
    TS_ASSERT_EQUALS(std::string("\x1B[0mHello\x1B[0m"),
                     BashColor::setColor(txt, LogManager::LEVEL_NONE));
    TS_ASSERT_DIFFERS(txt, BashColor::setColor(txt, LogManager::LEVEL_ERROR));


    TS_ASSERT_EQUALS(std::string("\x1B[36mHello\x1B[0m"),
                     BashColor::setColor(txt, LogManager::LEVEL_DEBUG));
    TS_ASSERT_EQUALS(std::string("\x1B[37mHello\x1B[0m"),
                     BashColor::setColor(txt, LogManager::LEVEL_INFO));
    TS_ASSERT_EQUALS(std::string("\x1B[33mHello\x1B[0m"),
                     BashColor::setColor(txt, LogManager::LEVEL_WARNING));
    TS_ASSERT_EQUALS(std::string("\x1B[31mHello\x1B[0m"),
                     BashColor::setColor(txt, LogManager::LEVEL_ERROR));
  }

private:
};

#endif
