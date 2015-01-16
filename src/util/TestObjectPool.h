#ifndef UTIL_TESTOBJECTPOOL_H
#define UTIL_TESTOBJECTPOOL_H

#include <util/ObjectPool.h>

#include <cxxtest/TestSuite.h>


namespace _TestObjectPool {
int counter;
int lastValueDeleted;

class Widget
{
 public:
  explicit Widget(int val) : val_(val) {}
  virtual ~Widget() {
    _TestObjectPool::lastValueDeleted = val_;
    ++_TestObjectPool::counter;
  }

  int val_;
};

}

/**
 * Test for the ObjectPool class.
 *
 * @author SwarmingLogic
 */
class TestObjectPool : public CxxTest::TestSuite
{
 public:
  using UPW = std::unique_ptr<_TestObjectPool::Widget >;
  using UPW_d = ObjectPool<_TestObjectPool::Widget>::ptr_type;
  using SPW = std::shared_ptr<_TestObjectPool::Widget>;

  void setUp() {
    _TestObjectPool::counter = 0;
    _TestObjectPool::lastValueDeleted = -1;
  }


  void testBasics()
  {
    ObjectPool<int> pool;
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
      ObjectPool<int>::ptr_type v2 = pool.acquire();
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
      ObjectPool<_TestObjectPool::Widget> pool;
      pool.add(UPW(new _TestObjectPool::Widget(42)));
      pool.add(UPW(new _TestObjectPool::Widget(82)));
      pool.add(UPW(new _TestObjectPool::Widget(1024)));

      TS_ASSERT_EQUALS(_TestObjectPool::counter, 0);
      TS_ASSERT_EQUALS(_TestObjectPool::lastValueDeleted, -1);
      TS_ASSERT_EQUALS(pool.size(), 3u);
      auto p1 = pool.acquire();
      TS_ASSERT_EQUALS(p1->val_, 1024);
      TS_ASSERT_EQUALS(pool.size(), 2u);

      p1.reset(nullptr);
      TS_ASSERT_EQUALS(pool.size(), 3u);
      p1 = pool.acquire();
      TS_ASSERT_EQUALS(pool.size(), 2u);
      TS_ASSERT_EQUALS(p1->val_, 1024);

      TS_ASSERT_EQUALS(_TestObjectPool::counter, 0);
      TS_ASSERT_EQUALS(_TestObjectPool::lastValueDeleted, -1);
    }
    TS_ASSERT_EQUALS(_TestObjectPool::counter, 3);
    TS_ASSERT_DIFFERS(_TestObjectPool::lastValueDeleted, -1);
  }


  void testLIFOasShared() {
    // Objects in the pool should not be created/destroyed
    // due to usage in and out of the pool.
    ObjectPool<_TestObjectPool::Widget> pool;
    TS_ASSERT(pool.empty());

    pool.add(UPW(new _TestObjectPool::Widget(1)));
    pool.add(UPW(new _TestObjectPool::Widget(2)));
    pool.add(UPW(new _TestObjectPool::Widget(10)));
    pool.add(UPW(new _TestObjectPool::Widget(20)));


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
    ObjectPool<_TestObjectPool::Widget> pool;
    TS_ASSERT(pool.empty());

    pool.add(UPW(new _TestObjectPool::Widget(1)));
    pool.add(UPW(new _TestObjectPool::Widget(2)));
    pool.add(UPW(new _TestObjectPool::Widget(10)));
    pool.add(UPW(new _TestObjectPool::Widget(20)));


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

  // Objects in the pool should not be created/destroyed
  // due to usage in and out of the pool.
  //
  void testLeakOfNonReturnedItems() {
    std::unique_ptr< ObjectPool<_TestObjectPool::Widget> >
        pool(new ObjectPool<_TestObjectPool::Widget>);

    pool->add(std::unique_ptr<_TestObjectPool::Widget> (
        new _TestObjectPool::Widget(42)));
    pool->add(std::unique_ptr<_TestObjectPool::Widget> (
        new _TestObjectPool::Widget(84)));
    pool->add(std::unique_ptr<_TestObjectPool::Widget> (
        new _TestObjectPool::Widget(1024)));
    pool->add(std::unique_ptr<_TestObjectPool::Widget> (
        new _TestObjectPool::Widget(1337)));

    auto v1 = pool->acquire();
    auto v2 = pool->acquire();

    // All items originally in pool should be cleaned up
    pool.reset(nullptr);
    v1.reset(nullptr);
    v2.reset(nullptr);

    // Verify this by counting the number of cleared items as 4
    TS_ASSERT_EQUALS(_TestObjectPool::counter, 4);
    TS_ASSERT_DIFFERS(_TestObjectPool::lastValueDeleted, -1);
  }
};

#endif  // UTIL_TESTOBJECTPOOL_H
