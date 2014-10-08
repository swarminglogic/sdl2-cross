#ifndef MATH_TESTRANDOM_H
#define MATH_TESTRANDOM_H

#include <math/Random.h>

#include <cxxtest/TestSuite.h>

/**
 * Test for the Random class.
 *
 * @author SwarmingLogic (Roald Fernandez)
 */
class TestRandom : public CxxTest::TestSuite
{
 public:
  void testDefaultGet()
  {
    for (size_t i = 0 ; i < 100000 ; ++i) {
      const float v = Random::get();
      TS_ASSERT(v >= 0.0f && v <= 1.0f);
    }
  }

  void testFloatGet()
  {
    for (size_t i = 0 ; i < 100000 ; ++i) {
      const float v = Random::get(-21.0f, 21.0f);
      TS_ASSERT(v >= -21.0f && v <= 21.0f);
    }
  }


  void testIntGet()
  {
    for (size_t i = 0 ; i < 1000 ; ++i) {
      const float v = static_cast<float>(Random::getInt(0, 10));
      TS_ASSERT(v >= 0 && v <= 10);
    }

    for (size_t i = 0 ; i < 1000 ; ++i) {
      const float v = static_cast<float>(Random::getInt(-60, -30));
      TS_ASSERT(v >= -60 && v <= -30);
    }
  }

 private:
};

#endif  // MATH_TESTRANDOM_H
