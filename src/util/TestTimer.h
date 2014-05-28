#ifndef UTIL_TESTTIMER_H
#define UTIL_TESTTIMER_H

#include <chrono>
#include <thread>

#include <util/Timer.h>

#include <cxxtest/TestSuite.h>


class TestTimer : public CxxTest::TestSuite
{
private:
  void msleep(unsigned milliseconds) {
    std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
  }

public:

  void testTimerInit()
  {
    Timer timer;
    TS_ASSERT_EQUALS(timer.getTicks(), 0u);
    TS_ASSERT_DELTA(timer.getSeconds(), 0.0f, 0.00001f);
  }


  void testBasicTimer()
  {
    Timer timer;
    timer.start();

    // Sleep for a little bit
    unsigned int milliseconds = 15;
    msleep(milliseconds);

    TS_ASSERT_LESS_THAN_EQUALS(milliseconds, timer.getTicks());
    TS_ASSERT_LESS_THAN(timer.getTicks(), milliseconds + 100u);
    TS_ASSERT_LESS_THAN(timer.getSeconds(),
                        (float)(milliseconds + 100u)/1000.0f);

    timer.pause();
    unsigned int timeWhenPaused = timer.getTicks();
    float timeWhenPausedInSeconds = timer.getSeconds();
    msleep(5);
    TS_ASSERT_EQUALS(timeWhenPaused, timer.getTicks());
    TS_ASSERT_DELTA(timeWhenPausedInSeconds,
                    timer.getSeconds(),
                    0.00001f);
  }


  void testReset()
  {
    Timer timer;
    timer.start();
    TS_ASSERT(!timer.isStopped());
    TS_ASSERT(!timer.isPaused());
    TS_ASSERT(timer.isRunning());

    unsigned int milliseconds = 5;
    msleep(milliseconds);

    unsigned int timeWhenRunning = timer.getTicks();
    unsigned int timeWhenReset = timer.reset();
    TS_ASSERT(timer.isStopped());
    TS_ASSERT(!timer.isPaused());
    TS_ASSERT(!timer.isRunning());
    TS_ASSERT_LESS_THAN_EQUALS(timeWhenRunning, timeWhenReset);
    TS_ASSERT_LESS_THAN_EQUALS(milliseconds, timeWhenReset);
    TS_ASSERT_EQUALS(0u, timer.getTicks());

    // Reset is global event, and should be possible regardless of state
    timer.start();
    TS_ASSERT(timer.isRunning());
    timer.reset();
    TS_ASSERT(timer.isStopped());
    TS_ASSERT_EQUALS(0u, timer.getTicks());

    timer.start();
    timer.pause();
    TS_ASSERT(timer.isPaused());
    timer.reset();
    TS_ASSERT(timer.isStopped());
    TS_ASSERT_EQUALS(0u, timer.getTicks());

    timer.start();
    timer.pause();
    timer.resume();
    TS_ASSERT(timer.isRunning());
    timer.reset();
    TS_ASSERT(timer.isStopped());
    TS_ASSERT_EQUALS(0u, timer.getTicks());

    timer.stop();
    TS_ASSERT(timer.isStopped());
    timer.reset();
    TS_ASSERT(timer.isStopped());
    TS_ASSERT_EQUALS(0u, timer.getTicks());
  }


  void testTimerStates()
  {
    Timer timer;
    TS_ASSERT(timer.isStopped());
    TS_ASSERT(!timer.isPaused());
    TS_ASSERT(!timer.isRunning());

    timer.start();
    TS_ASSERT(!timer.isStopped());
    TS_ASSERT(!timer.isPaused());
    TS_ASSERT(timer.isRunning());

    timer.pause();
    TS_ASSERT(!timer.isStopped());
    TS_ASSERT(timer.isPaused());
    TS_ASSERT(!timer.isRunning());

    timer.resume();
    TS_ASSERT(!timer.isStopped());
    TS_ASSERT(!timer.isPaused());
    TS_ASSERT(timer.isRunning());

    timer.stop();
    TS_ASSERT(timer.isStopped());
    TS_ASSERT(!timer.isPaused());
    TS_ASSERT(!timer.isRunning());

    timer.start();
    TS_ASSERT(!timer.isStopped());
    TS_ASSERT(!timer.isPaused());
    TS_ASSERT(timer.isRunning());

    timer.reset();
    TS_ASSERT(timer.isStopped());
    TS_ASSERT(!timer.isPaused());
    TS_ASSERT(!timer.isRunning());

    // Resume does nothing when stopped
    timer.resume();
    TS_ASSERT(timer.isStopped());

    TS_ASSERT(timer.isStopped());
    timer.toggleStopStart();
    TS_ASSERT(timer.isRunning());

    timer.pause();
    // Paused is considered as 'running' in terms of StopStart
    timer.toggleStopStart();
    TS_ASSERT(timer.isStopped());

    // PauseResume does nothing when stopped
    timer.togglePauseResume();
    TS_ASSERT(timer.isStopped());
    timer.togglePauseResume();
    TS_ASSERT(timer.isStopped());

    timer.start();
    TS_ASSERT(!timer.isPaused());
    timer.togglePauseResume();
    TS_ASSERT(timer.isPaused());
    timer.togglePauseResume();
    TS_ASSERT(!timer.isPaused());
  }
};

#endif
