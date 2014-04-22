#ifndef UTIL_TESTCLOCK_H
#define UTIL_TESTCLOCK_H

#include <chrono>
#include <string>

#include <util/Clock.h>

#include <cxxtest/TestSuite.h>


/**
 * Test for the Clock class.
 *
 * @author SwarmingLogic (Roald Fernandez)
 */
class TestClock : public CxxTest::TestSuite
{
public:
  void testClock()
  {
    Clock clock;
    std::chrono::system_clock::time_point now{std::chrono::system_clock::now()};
    std::time_t tnow = std::chrono::system_clock::to_time_t(now);
    std::tm time = *std::localtime(&tnow);


    std::string timestamp = clock.getTimeStamp();
    TS_ASSERT(!timestamp.empty());

    // To avoid introducing delay, only interface is tested.
    TS_ASSERT_EQUALS(clock.getYear(),    time.tm_year + 1900);
    TS_ASSERT_EQUALS(clock.getMonth(),   time.tm_mon);
    TS_ASSERT_EQUALS(clock.getDay(),     time.tm_mday);
    TS_ASSERT_EQUALS(clock.getHours(),   time.tm_hour);
    TS_ASSERT_EQUALS(clock.getMinutes(), time.tm_min);
    TS_ASSERT_EQUALS(clock.getSeconds(), time.tm_sec);
  }

private:
};

#endif
