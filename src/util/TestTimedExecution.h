#ifndef UTIL_TESTTIMEDEXECUTION_H
#define UTIL_TESTTIMEDEXECUTION_H

#include <chrono>
#include <functional>
#include <thread>

#include <util/TimedExecution.h>

#include <cxxtest/TestSuite.h>


namespace Mock {
  class Dummy
  {
  public:
    Dummy() : v_(0) {}
    virtual ~Dummy() {}
    void functionToExecute() { ++v_; }
    int get() { return v_; }
  private:
    int v_;
  };
}

/**
 * Test for the TimedExecution class.
 *
 * @author SwarmingLogic (Roald Fernandez)
 */
class TestTimedExecution : public CxxTest::TestSuite
{
 private:
  void msleep(unsigned milliseconds) {
    std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
  }
 public:
  void testBasicOneShot()
  {
    Mock::Dummy mock;
    TimedExecution t(std::bind(&Mock::Dummy::functionToExecute, &mock),
                     5, 1);
    TS_ASSERT(!t.exec());
    TS_ASSERT_EQUALS(mock.get(), 0);
    TS_ASSERT(!t.isDone());
    msleep(5);
    TS_ASSERT(t.exec());
    TS_ASSERT_EQUALS(mock.get(), 1);
    TS_ASSERT(t.isDone());
    msleep(5);
    TS_ASSERT(!t.exec());
    TS_ASSERT(t.isDone());
    TS_ASSERT_EQUALS(mock.get(), 1);
  }

  void testInfinite()
  {
    Mock::Dummy mock;
    TimedExecution t(std::bind(&Mock::Dummy::functionToExecute, &mock),
                     5);
    TS_ASSERT(!t.exec());
    TS_ASSERT_EQUALS(mock.get(), 0);
    TS_ASSERT(!t.isDone());
    msleep(5);
    TS_ASSERT(t.exec());
    TS_ASSERT_EQUALS(mock.get(), 1);
    TS_ASSERT(!t.isDone());
    msleep(5);
    TS_ASSERT(t.exec());
    TS_ASSERT(!t.isDone());
    TS_ASSERT_EQUALS(mock.get(), 2);
  }

 private:
};

#endif  // UTIL_TESTTIMEDEXECUTION_H
