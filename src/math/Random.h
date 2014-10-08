#ifndef MATH_RANDOM_H
#define MATH_RANDOM_H

#include <cassert>
#include <random>


/**
 * Random class, for generating random values.
 * Currently not thread safe.
 *
 * @author SwarmingLogic (Roald Fernandez)
 */
class Random
{
 public:
  static float get() {
    return dist_(device_);
  }

  static float get(float min, float max) {
    std::uniform_real_distribution<float> dist(min, max);
    return dist(device_);
  }

  static bool getBool() {
    return get() > 0.5f;
  }

  static int getInt(int min, int max) {
    assert(min <= max);  // LCOV_EXCL_LINE
    std::uniform_int_distribution<int> dist(min, max);
    return dist(device_);
  }

  static void seed(unsigned int s) {
    seed_ = s;
    device_.seed(s);
  }

  static unsigned int seed() {
    return seed_;
  }

 private:
  Random(){}
  ~Random();

  static std::mt19937 device_;
  static std::uniform_real_distribution<float> dist_;
  static unsigned int seed_;

  // NonCopyable
  Random(const Random& c);
  Random& operator=(const Random& c);
};

#endif  // MATH_RANDOM_H
