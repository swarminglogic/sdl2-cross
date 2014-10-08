#ifndef UTIL_TIMEDEXECUTION_H
#define UTIL_TIMEDEXECUTION_H

#include <functional>

#include <util/Timer.h>


/**
 * TimedExecution class.
 *
 * Is initalized with a function pointer which manages time, which can only be
 * executed if a certain timeout has passed.
 *
 * This does NOT execute the function every N milliseconds, but functions as
 * guard against being executed more often than that.
 *
 * It can be configured to only be executed a certain number of times,
 * at which isDone() will return true.
 *
 * Note: Passing on function pointers that modifies the instance should be done
 *   with great care with multi-threading. If all you want to do is protect
 *   a function with a timer, wrap it inside this an instance of this class.
 *
 * TODO swarminglogic, 2013-09-21: option to accumulate execution.
 *    You then need to maintain a uint leftOverTime.
 *    Note then: leftOverTime = timer_.getTicks() % timeout.
 *    "If timeout is x, and 3x has passed, it performs exec() until
 *     accumulation falls below threshold."
 *
 * @author SwarmingLogic (Roald Fernandez)
 */
class TimedExecution
{
 public:
  /**
   * Timed execution for function fun,
   *
   * @param fun The function to register.
   * @param timeout  Number of milliseconds as exec() threshold.
   * @param nExecutions Number of times to execute. -1 for unlimited.
   */
  TimedExecution(std::function<void(void)> fun,
                 uint timeout,
                 int nExecutions = -1);

  bool exec();

  virtual ~TimedExecution();

  bool isDone();

  /**
   * Returns # milliseconds until it will be ready, and exec() have effect.
   */
  int getRemainingTimeout();

 private:
  std::function<void(void)> fun_;
  uint  timeout_;
  int   nExecutions_;
  Timer timer_;

  // NonCopyable
  TimedExecution(const TimedExecution& c);
  TimedExecution& operator=(const TimedExecution& c);
};

#endif  // UTIL_TIMEDEXECUTION_H
