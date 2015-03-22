#ifndef MATH_RANDOM_H
#define MATH_RANDOM_H

#include <ctime>
#include <random>
#include <thread>

#include <util/Assert.h>


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

  static void seed(const std::seed_seq&  s) {
    seed_ = s;
    device_.seed(seed_);
  }

  static const std::seed_seq& seed() {
    return seed_;
  }

  static void init() {
    const auto time_seed = static_cast<size_t>(std::time(0));
    const auto clock_seed = static_cast<size_t>(std::clock());
    const size_t pid_seed =
        std::hash<std::thread::id>()(std::this_thread::get_id());
    seed({time_seed, clock_seed, pid_seed });
  }

 private:
  Random(){}
  ~Random();

  static std::mt19937 device_;
  static std::uniform_real_distribution<float> dist_;
  static std::seed_seq seed_;

  // NonCopyable
  Random(const Random& c);
  Random& operator=(const Random& c);
};

#endif  // MATH_RANDOM_H
