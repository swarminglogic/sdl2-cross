#include <util/TimedExecution.h>

#include <cassert>


TimedExecution::TimedExecution(std::function<void(void)> fun,
                               uint timeout,
                               int nExecutions)
  : fun_(fun),
    timeout_(timeout),
    nExecutions_(nExecutions),
    timer_()
{
  assert(timeout > 0 && "Can't have zero or negative timeout");
  assert(nExecutions == -1 || nExecutions > 0);

  timer_.start();
}


TimedExecution::~TimedExecution()
{
}


bool TimedExecution::exec()
{
  if (!isDone() && timer_.getTicks() >= timeout_) {
    --nExecutions_;
    fun_();
    timer_.reset();
    timer_.start();
    return true;
  }
  return false;
}


bool TimedExecution::isDone()
{
  return nExecutions_ == 0;
}


int TimedExecution::getRemainingTimeout()
{
  return timeout_ - timer_.getTicks();
}

