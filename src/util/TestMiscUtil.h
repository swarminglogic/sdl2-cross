#ifndef UTIL_TESTMISCUTIL_H
#define UTIL_TESTMISCUTIL_H

#include <util/TestMiscUtil.h>

#include <vector>

#include <util/MiscUtil.h>

#include <cxxtest/TestSuite.h>


/**
 * Test for the MiscUtil class.
 *
 * @author SwarmingLogic
 */
class TestMiscUtil : public CxxTest::TestSuite
{
 public:
  void testGetRandomIntElement() {
    for (size_t i = 0 ; i < 100 ; ++i) {
      std::vector<int> c { 51 };
      int v = MiscUtil::getRandomElement(c);
      TS_ASSERT_EQUALS(v, 51);
      v = 42;
      TS_ASSERT_EQUALS(c[0], 51);

      int& v2 = MiscUtil::getRandomElement(c);
      TS_ASSERT_EQUALS(v2, 51);
      v2 = 42;
      TS_ASSERT_EQUALS(c[0], 42);
    }

    std::vector<int> modify(100, 123);
    int& v = MiscUtil::getRandomElement(modify);
    v = 1024;
    bool atLeastOne = false;
    for (auto& x : modify)
      if ( x == 1024 ) atLeastOne = true;
    TS_ASSERT(atLeastOne);

    const std::vector<int> cvec { 42 };
    const int& cv2 = MiscUtil::getRandomElement(cvec);
    TS_ASSERT_EQUALS(cv2, 42);
  }


  void testGetRandomElement() {
    std::vector<std::string> foo { "FOO" };
    std::string& fooref = MiscUtil::getRandomElement(foo);
    fooref = "modifiedfoo";
    TS_ASSERT_EQUALS(foo[0], "modifiedfoo");

    std::vector<std::string> c { "Foo" };
    for (size_t i = 0 ; i < 100 ; ++i) {
      std::string v = MiscUtil::getRandomElement(c);
      TS_ASSERT_EQUALS(v, "Foo");
    }

    std::vector<std::string> modify(100, "bar");
    std::string& v = MiscUtil::getRandomElement(modify);
    v = std::string("tux");
    bool atLeastOne = false;
    for (const auto& x : modify)
      if ( x == std::string("tux") ) atLeastOne = true;
    TS_ASSERT(atLeastOne);

    const std::vector<std::string> cvec { "magic" };
    const std::string& cv2 = MiscUtil::getRandomElement(cvec);
    TS_ASSERT_EQUALS(cv2, "magic");
  }
};

#endif  // UTIL_TESTMISCUTIL_H
