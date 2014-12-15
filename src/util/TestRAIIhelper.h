#ifndef UTIL_TESTRAIIHELPER_H
#define UTIL_TESTRAIIHELPER_H

#include <cassert>

#include <util/RAIIhelper.h>

#include <cxxtest/TestSuite.h>


namespace _TestRAIIhelper {

int counter;
int lastValueDeleted;

template <class T>
void deleterFunction(T* ptr) {
  ++counter;
  if (ptr != nullptr) {
    lastValueDeleted = *ptr;
  } else {
    lastValueDeleted = -1;
  }
  delete ptr;
}

template <typename T>
struct Deleter_Test {
  // NB!
  // unique_ptr calls deleter only if ptr is != null
  // shared_ptr calls deleter regardless
  void operator()(T*  ptr) { deleterFunction(ptr); }
};
}


/**
 * Test for the RAIIhelper class.
 *
 * @author SwarmingLogic
 */
class TestRAIIhelper : public CxxTest::TestSuite
{
  using UniquePtr  = std::unique_ptr<int,
                                     _TestRAIIhelper::Deleter_Test<int> >;
  using SharedPtr  =
      shared_ptr_with_deleter<int, _TestRAIIhelper::Deleter_Test<int> >;

 public:
  void setUp() {
    _TestRAIIhelper::counter = 0;
    _TestRAIIhelper::lastValueDeleted = -1;
  }

  void testUniquePointer()
  {
    TS_ASSERT_EQUALS(_TestRAIIhelper::counter, 0);
    TS_ASSERT_EQUALS(_TestRAIIhelper::lastValueDeleted, -1);

    // Create unique pointer in it's own scope so that it is deleted
    { UniquePtr up(new int(42)); }

    // Check that deleter has been called, and counter incremented
    TS_ASSERT_EQUALS(_TestRAIIhelper::counter, 1);
    TS_ASSERT_EQUALS(_TestRAIIhelper::lastValueDeleted, 42);
  }

  void testUniquePointerNull()
  {
    TS_ASSERT_EQUALS(_TestRAIIhelper::counter, 0);
    TS_ASSERT_EQUALS(_TestRAIIhelper::lastValueDeleted, -1);

    { UniquePtr up; }
    // shared_ptr w/nullptr doesn't call deleter upon destruction
    TS_ASSERT_EQUALS(_TestRAIIhelper::counter, 0);
    TS_ASSERT_EQUALS(_TestRAIIhelper::lastValueDeleted, -1);

    { UniquePtr up(new int(42)); }
    TS_ASSERT_EQUALS(_TestRAIIhelper::counter, 1);
    TS_ASSERT_EQUALS(_TestRAIIhelper::lastValueDeleted, 42);

    { UniquePtr up(nullptr); }
    TS_ASSERT_EQUALS(_TestRAIIhelper::counter, 1);
    TS_ASSERT_EQUALS(_TestRAIIhelper::lastValueDeleted, 42);
}


  // Test that resetting does in fact call deleter
  void testUniquePointerMove()
  {
    // Create unique pointer in it's own scope so that it is deleted
    { UniquePtr up(new int(43));
      UniquePtr up2 = std::move(up);
    }

    TS_ASSERT_EQUALS(_TestRAIIhelper::counter, 1);
    TS_ASSERT_EQUALS(_TestRAIIhelper::lastValueDeleted, 43);
  }

  void testUniquePointerReset()
  {
    // Create unique pointer in it's own scope so that it is deleted
    { UniquePtr up(new int(43));
      up.reset(new int(86));
      TS_ASSERT_EQUALS(_TestRAIIhelper::counter, 1);
      TS_ASSERT_EQUALS(_TestRAIIhelper::lastValueDeleted, 43);
    }

    // Check that deleter has been called, and counter incremented
    TS_ASSERT_EQUALS(_TestRAIIhelper::counter, 2);
    TS_ASSERT_EQUALS(_TestRAIIhelper::lastValueDeleted, 86);
  }

  void testUniquePointerRelease()
  {
    // Create unique pointer in it's own scope so that it is deleted
    UniquePtr up(new int(43));
    int* ip = up.release();
    TS_ASSERT_DIFFERS(ip, nullptr);
    TS_ASSERT_EQUALS(*ip, 43);
    TS_ASSERT_EQUALS(up.get(), nullptr);
  }

  void testSharedPointer()
  {
    // Create shared pointer in it's own scope so that it is deleted
    { SharedPtr up(new int(44)); }

    // Check that deleter has been called, and counter incremented
    TS_ASSERT_EQUALS(_TestRAIIhelper::counter, 1);
    TS_ASSERT_EQUALS(_TestRAIIhelper::lastValueDeleted, 44);
  }

  void testSharedPointerNull()
  {
    TS_ASSERT_EQUALS(_TestRAIIhelper::counter, 0);
    TS_ASSERT_EQUALS(_TestRAIIhelper::lastValueDeleted, -1);

    { SharedPtr up; }
    // shared_ptr w/nullptr DOES call deleter upon destruction
    TS_ASSERT_EQUALS(_TestRAIIhelper::counter, 1);
    TS_ASSERT_EQUALS(_TestRAIIhelper::lastValueDeleted, -1);

    { SharedPtr up(new int(42)); }
    TS_ASSERT_EQUALS(_TestRAIIhelper::counter, 2);
    TS_ASSERT_EQUALS(_TestRAIIhelper::lastValueDeleted, 42);

    { SharedPtr up(nullptr); }
    TS_ASSERT_EQUALS(_TestRAIIhelper::counter, 3);
    TS_ASSERT_EQUALS(_TestRAIIhelper::lastValueDeleted, -1);
  }

  void testSharedPointerMultipleOwners()
  {
    // Create shared pointer in it's own scope so that it is deleted
    { SharedPtr up(new int(45));
      TS_ASSERT_EQUALS(up.unique(), true);

      SharedPtr up2(up);
      TS_ASSERT_EQUALS(up.use_count(), 2);

      SharedPtr up3 = up2;
      TS_ASSERT_EQUALS(up.use_count(), 3);
    }

    // Check that deleter has been called, and counter incremented
    TS_ASSERT_EQUALS(_TestRAIIhelper::counter, 1);
    TS_ASSERT_EQUALS(_TestRAIIhelper::lastValueDeleted, 45);
  }


  void testSharedPointerReset()
  {
    // Create unique pointer in it's own scope so that it is deleted
    { SharedPtr up(new int(43));
      up.reset(new int(86));
      TS_ASSERT_EQUALS(_TestRAIIhelper::counter, 1);
      TS_ASSERT_EQUALS(_TestRAIIhelper::lastValueDeleted, 43);
    }

    // Check that deleter has been called, and counter incremented
    TS_ASSERT_EQUALS(_TestRAIIhelper::counter, 2);
    TS_ASSERT_EQUALS(_TestRAIIhelper::lastValueDeleted, 86);
  }
};

#endif  // UTIL_TESTRAIIHELPER_H
