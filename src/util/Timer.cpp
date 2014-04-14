#include "Timer.h"

// USE SDL TIMING FUNCTIONS
#if defined USING_SDL
#include <util/SDL.h>
struct Timer::Timer_impl
{ unsigned int getTicksSinceStart() const { return SDL_GetTicks(); }};
#else

//USE C++11 CHRONO FALLBACK
#include <chrono>
class Timer::Timer_impl
{
public:
  Timer_impl() : start_(std::chrono::high_resolution_clock::now()) {}
  unsigned int getTicksSinceStart() const {
    using std::chrono::high_resolution_clock;
    using std::chrono::duration_cast;

    const auto now = high_resolution_clock::now();
    const unsigned int elapsedtics =
      duration_cast<std::chrono::milliseconds>(now - start_).count();
    return elapsedtics;
  }
private:
  std::chrono::time_point<std::chrono::high_resolution_clock> start_;
};
#endif


Timer::Timer()
  : impl_(new Timer_impl()),
    state_(STOPPED),
    ticksWhenStarted_(0),
    ticksAccum_(0)
{
}


Timer::~Timer()
{
  delete impl_;
}


void Timer::eventTriggered(Event event)
{
  // Global events
  if (event == RESTARTING) {
    state_ = STOPPED;
    return;
  }

  // State dependent events
  switch(state_) {
  case STOPPED:
    if (event == STARTING) {
      ticksWhenStarted_ = impl_->getTicksSinceStart();
      ticksAccum_ = 0;
      state_ = RUNNING;
    }
    break;
  case RUNNING:
    if (event == STOPPING) {
      state_ = STOPPED;
    }
    else if (event == PAUSING) {
      ticksAccum_ += (impl_->getTicksSinceStart() - ticksWhenStarted_);
      state_ = PAUSED;
    }
    break;
  case PAUSED:
    if (event == STOPPING)
      state_ = STOPPED;
    else if (event == RESUMING) {
      ticksWhenStarted_ = impl_->getTicksSinceStart();
      state_ = RUNNING;
    }
    break;
  }
}

void Timer::start()
{
  eventTriggered(STARTING);
}


void Timer::stop()
{
  eventTriggered(STOPPING);
}


void Timer::pause()
{
  eventTriggered(PAUSING);
}


void Timer::resume()
{
  eventTriggered(RESUMING);
}


unsigned int Timer::reset()
{
  const unsigned int ticks = getTicks();
  eventTriggered(RESTARTING);
  return ticks;
}


void Timer::togglePauseResume()
{
  if (isState(STOPPED))
    return;
  else if (isState(RUNNING))
    eventTriggered(PAUSING);
  else if (isState(PAUSED))
    eventTriggered(RESUMING);
}


void Timer::toggleStopStart()
{
  if (isState(STOPPED))
    eventTriggered(STARTING);
  else
    eventTriggered(STOPPING);
}



unsigned int Timer::getTicks() const
{
  if (isState(STOPPED))
    return 0;
  else if (isState(PAUSED))
    return ticksAccum_;
  else if (isState(RUNNING))
    return ticksAccum_ + (impl_->getTicksSinceStart() - ticksWhenStarted_);

  return 0;
}

float Timer::getSeconds() const
{
  return static_cast<float>(getTicks()) / 1000.0f;
}


bool Timer::isPaused() const
{
  return isState(PAUSED);
}


bool Timer::isStopped() const
{
  return isState(STOPPED);
}


bool Timer::isRunning() const
{
  return isState(RUNNING);
}

bool Timer::isState(State state) const
{
  return (state_ == state);
}
