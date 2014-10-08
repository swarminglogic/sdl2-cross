#ifndef UTIL_TIMER_H
#define UTIL_TIMER_H

/**
 * Timer class for doing timing functions.
 *
 * Has wrap-around at approx 1193 hours (49 days).
 *
 * Nomenclature: tic = millisecond.
 *
 * @author  Roald Fernandez (roaldfernandez@gmail.com)
 * @date    2013-06-26
 * @license CC-BY
 */
class Timer
{
 public:
  Timer();

  virtual ~Timer();

  void start();
  void stop();
  void pause();
  void resume();
  unsigned int reset();
  void togglePauseResume();
  void toggleStopStart();

  unsigned int getTicks() const;
  float getSeconds() const;
  bool isPaused() const;
  bool isStopped() const;
  bool isRunning() const;

 private:
  enum Event{
    STOPPING = 0,
    STARTING,
    PAUSING,
    RESUMING,
    RESTARTING
  };

  enum State{
    STOPPED = 0,
    RUNNING,
    PAUSED
  };

  bool isState(State state) const;
  void eventTriggered(Event event);

  // Hiding external library dependant timing code (pimpl pattern)
  class Timer_impl;
  Timer_impl* impl_;

  State state_;
  unsigned int ticksWhenStarted_;
  unsigned int ticksAccum_;

  // Disabled copy, implement and test when needed.
  Timer(const Timer& c);
  Timer& operator=(const Timer& c);
};

#endif  // UTIL_TIMER_H
