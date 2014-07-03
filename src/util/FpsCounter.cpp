#include <util/FpsCounter.h>


FpsCounter::FpsCounter(int n)
  : timer_(),
    averageSpf_(n),
    lastTime_(0.0f)
{
  timer_.start();
  tic();
}


FpsCounter::~FpsCounter()
{
}


double FpsCounter::getFps() const
{
  return 1.0 / averageSpf_.getAverage();
}

double FpsCounter::getSpf() const
{
  return averageSpf_.getAverage();
}

void FpsCounter::tic()
{
  const double now = timer_.getTicks() / 1000.0;
  averageSpf_.insertValue(now - lastTime_);
  lastTime_ = now;
}
