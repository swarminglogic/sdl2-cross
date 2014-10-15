#ifndef UTIL_TESTNOISEUTIL_H
#define UTIL_TESTNOISEUTIL_H

#include <util/NoiseUtils.h>
#include <cxxtest/TestSuite.h>

/**
 * Test for the NoiseUtil class.
 *
 * @author SwarmingLogic
 */
class TestNoiseUtil : public CxxTest::TestSuite
{
 public:
  void testLibraryBasics()
  {
    noise::utils::NoiseMap heightMap;
    TS_ASSERT_EQUALS(0, heightMap.GetHeight());
  }


 private:
};

#endif  // UTIL_TESTNOISEUTIL_H
