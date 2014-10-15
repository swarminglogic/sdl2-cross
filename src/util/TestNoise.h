#ifndef UTIL_TESTNOISE_H
#define UTIL_TESTNOISE_H

#include <util/Noise.h>

#include <cxxtest/TestSuite.h>


/**
 * Test for the Noise class.
 *
 * @author SwarmingLogic
 */
class TestNoise : public CxxTest::TestSuite
{
 public:
  void testLibraryBasics()
  {
    noise::module::Perlin myModule;
    double value = myModule.GetValue(1.25, 0.75, 0.50);
    TS_ASSERT(value >= 0.0);
  }

 private:
};

#endif
