#ifndef MATH_TESTAVERAGER_H
#define MATH_TESTAVERAGER_H

#include <cstdlib>

#include <math/Averager.h>

#include <cxxtest/TestSuite.h>


/**
 * Test for the Averager class.
 *
 * @author Roald Fernandez
 */
class TestAverager : public CxxTest::TestSuite
{
 private:
  float _getRandFloat() {
    return (static_cast<float>(rand()) /
            static_cast<float>(RAND_MAX));
  }

 public:
  void testSingleValue()
  {
    const float delta = 0.00001f;
    Averager averager(1);
    averager.insertValue(0.0f);
    TS_ASSERT_DELTA(averager.getAverage(), 0.0f, delta);
    averager.insertValue(10.0f);
    TS_ASSERT_DELTA(averager.getAverage(), 10.0f, delta);

    // 10 different random values
    for (size_t i = 0 ; i < 10 ; ++i) {
      const float val = _getRandFloat();
      averager.insertValue(val);
      TS_ASSERT_DELTA(averager.getAverage(), val, delta);
    }
  }

  void testNonFull()
  {
    const float delta = 0.00001f;
    Averager averager(10);
    averager.insertValue(1.0f);
    TS_ASSERT_DELTA(averager.getAverage(), 1.0f, delta);
    averager.insertValue(2.0f);
    TS_ASSERT_DELTA(averager.getAverage(), (1.0f + 2.0f) / 2.0f, delta);
    averager.insertValue(7.0f);
    TS_ASSERT_DELTA(averager.getAverage(), (1.0f + 2.0f + 7.0f) / 3.0f, delta);
    averager.insertValue(14.0f);
    TS_ASSERT_DELTA(averager.getAverage(), (1.0f + 2.0f + 7.0f + 14.0f) / 4.0f,
                    delta);
  }


  void testIncrementFull()
  {
    const float delta = 0.00001f;
    Averager averager(3);
    averager.insertValue(1.0f);
    TS_ASSERT_DELTA(averager.getAverage(), 1.0f, delta);
    averager.insertValue(2.0f);
    TS_ASSERT_DELTA(averager.getAverage(), (1.0f + 2.0f) / 2.0f, delta);
    averager.insertValue(7.0f);
    TS_ASSERT_DELTA(averager.getAverage(), (1.0f + 2.0f + 7.0f) / 3.0f, delta);
    averager.insertValue(14.0f);
    TS_ASSERT_DELTA(averager.getAverage(), (2.0f + 7.0f + 14.0f) / 3.0f, delta);
    averager.insertValue(24.0f);
    TS_ASSERT_DELTA(averager.getAverage(), (7.0f + 14.0f + 24.0f) / 3.0f,
                    delta);
  }

  void testIncrementMany()
  {
    const float delta = 0.00001f;
    Averager averager(100000);
    std::vector<float> vals;
    float sum = 0.0f;
    for (size_t i = 0 ; i < 100000 ; ++i) {
      const float val = _getRandFloat();
      averager.insertValue(val);
      sum += val;
      TS_ASSERT_DELTA(averager.getAverage(),
                      sum / static_cast<float>(i + 1),
                      delta);
    }
  }


  void testCopy()
  {
    const float delta = 0.00001f;
    Averager averager(3);
    averager.insertValue(1.0f);
    TS_ASSERT_DELTA(averager.getAverage(), 1.0f, delta);
    averager.insertValue(2.0f);
    TS_ASSERT_DELTA(averager.getAverage(), (1.0f + 2.0f) / 2.0f, delta);

    Averager copy(averager);
    TS_ASSERT_DELTA(copy.getAverage(), (1.0f + 2.0f) / 2.0f, delta);
    averager.insertValue(7.0f);
    TS_ASSERT_DELTA(averager.getAverage(), (1.0f + 2.0f + 7.0f) / 3.0f, delta);
    TS_ASSERT_DELTA(copy.getAverage(), (1.0f + 2.0f) / 2.0f, delta);
    copy.insertValue(7.0f);
    TS_ASSERT_DELTA(copy.getAverage(), (1.0f + 2.0f + 7.0f) / 3.0f, delta);
  }

 private:
};

#endif  // MATH_TESTAVERAGER_H
