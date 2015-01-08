#ifndef UTIL_TESTSHAREDPOOL_H
#define UTIL_TESTSHAREDPOOL_H

#include <util/SharedPool.h>

#include <cxxtest/TestSuite.h>

namespace _TestSharedPool {
int counter;
int lastValueDeleted;

class Widget
{
 public:
  explicit Widget(int val) : val_(val) {}
  virtual ~Widget() {
    _TestSharedPool::lastValueDeleted = val_;
    ++_TestSharedPool::counter;
  }

  int val_;
};

}

/**
 * Test for the SharedPool class.
 *
 * @author SwarmingLogic
 */
class TestSharedPool : public CxxTest::TestSuite
{
 public:
  using UPW = std::unique_ptr<_TestSharedPool::Widget >;
  using UPW_d = SharedPool<_TestSharedPool::Widget>::ptr_type;
  using SPW = std::shared_ptr<_TestSharedPool::Widget>;

  void setUp() {
    _TestSharedPool::counter = 0;
    _TestSharedPool::lastValueDeleted = -1;
  }


  void testBasics()
  {
    SharedPool<int> pool;
    TS_ASSERT(pool.empty());
    TS_ASSERT_EQUALS(pool.size(), 0u);

    pool.add(std::unique_ptr<int>(new int(42)));
    pool.add(std::unique_ptr<int>(new int(94)));
    pool.add(std::unique_ptr<int>(new int(15)));
    TS_ASSERT(!pool.empty());
    TS_ASSERT_EQUALS(pool.size(), 3u);

    {
      std::shared_ptr<int> v = pool.acquire();
      TS_ASSERT_EQUALS(*v, 15);
      TS_ASSERT_EQUALS(pool.size(), 2u);
    }

    // Various ways to denote pointer type
    {
      auto v = pool.acquire();
      SharedPool<int>::ptr_type v2 = pool.acquire();
      std::unique_ptr<int, std::function<void(int*)> > v3 = pool.acquire();

      TS_ASSERT_EQUALS(*v, 15);
      TS_ASSERT_EQUALS(*v2, 94);
      TS_ASSERT_EQUALS(*v3, 42);
    }

    // Automatically returned to pool when acquired object is deleted
    TS_ASSERT(!pool.empty());
    TS_ASSERT(true);
  }


  // Objects in the pool should not be created/destroyed
  // due to usage in and out of the pool.
  void testPoolPersistence() {
    {
      SharedPool<_TestSharedPool::Widget> pool;
      pool.add(UPW(new _TestSharedPool::Widget(42)));
      pool.add(UPW(new _TestSharedPool::Widget(82)));
      pool.add(UPW(new _TestSharedPool::Widget(1024)));

      TS_ASSERT_EQUALS(_TestSharedPool::counter, 0);
      TS_ASSERT_EQUALS(_TestSharedPool::lastValueDeleted, -1);
      TS_ASSERT_EQUALS(pool.size(), 3u);
      auto p1 = pool.acquire();
      TS_ASSERT_EQUALS(p1->val_, 1024);
      TS_ASSERT_EQUALS(pool.size(), 2u);

      p1.reset(nullptr);
      TS_ASSERT_EQUALS(pool.size(), 3u);
      p1 = pool.acquire();
      TS_ASSERT_EQUALS(pool.size(), 2u);
      TS_ASSERT_EQUALS(p1->val_, 1024);

      TS_ASSERT_EQUALS(_TestSharedPool::counter, 0);
      TS_ASSERT_EQUALS(_TestSharedPool::lastValueDeleted, -1);
    }
    TS_ASSERT_EQUALS(_TestSharedPool::counter, 3);
    TS_ASSERT_DIFFERS(_TestSharedPool::lastValueDeleted, -1);
  }


  void testLIFOasShared() {
    // Objects in the pool should not be created/destroyed
    // due to usage in and out of the pool.
    SharedPool<_TestSharedPool::Widget> pool;
    TS_ASSERT(pool.empty());

    pool.add(UPW(new _TestSharedPool::Widget(1)));
    pool.add(UPW(new _TestSharedPool::Widget(2)));
    pool.add(UPW(new _TestSharedPool::Widget(10)));
    pool.add(UPW(new _TestSharedPool::Widget(20)));


    // <BOTTOM>  1, 2, 10, 20  <TOP>
    TS_ASSERT_EQUALS(pool.size(), 4u);

    SPW p1 = pool.acquire();
    TS_ASSERT_EQUALS(p1->val_, 20);
    TS_ASSERT_EQUALS(pool.size(), 3u);

    SPW p2 = pool.acquire();
    TS_ASSERT_EQUALS(p2->val_, 10);
    TS_ASSERT_EQUALS(pool.size(), 2u);

    SPW p3 = pool.acquire();
    TS_ASSERT_EQUALS(p3->val_, 2);
    TS_ASSERT_EQUALS(pool.size(), 1u);

    SPW p4 = pool.acquire();
    TS_ASSERT_EQUALS(p4->val_, 1);
    TS_ASSERT_EQUALS(pool.size(), 0u);
    TS_ASSERT(pool.empty());
  }

  void testLIFOasUnique() {
    // Objects in the pool should not be created/destroyed
    // due to usage in and out of the pool.
    SharedPool<_TestSharedPool::Widget> pool;
    TS_ASSERT(pool.empty());

    pool.add(UPW(new _TestSharedPool::Widget(1)));
    pool.add(UPW(new _TestSharedPool::Widget(2)));
    pool.add(UPW(new _TestSharedPool::Widget(10)));
    pool.add(UPW(new _TestSharedPool::Widget(20)));


    // <BOTTOM>  1, 2, 10, 20  <TOP>
    TS_ASSERT_EQUALS(pool.size(), 4u);

    UPW_d p1 = pool.acquire();
    TS_ASSERT_EQUALS(p1->val_, 20);
    TS_ASSERT_EQUALS(pool.size(), 3u);

    UPW_d p2 = pool.acquire();
    TS_ASSERT_EQUALS(p2->val_, 10);
    TS_ASSERT_EQUALS(pool.size(), 2u);

    UPW_d p3 = pool.acquire();
    TS_ASSERT_EQUALS(p3->val_, 2);
    TS_ASSERT_EQUALS(pool.size(), 1u);

    UPW_d p4 = pool.acquire();
    TS_ASSERT_EQUALS(p4->val_, 1);
    TS_ASSERT_EQUALS(pool.size(), 0u);
    TS_ASSERT(pool.empty());
  }

};

#endif  // UTIL_TESTSHAREDPOOL_H
