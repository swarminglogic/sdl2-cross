#ifndef UTIL_TESTEXCEPTION_H
#define UTIL_TESTEXCEPTION_H

#include <vector>

#include <util/Exception.h>

#include <cxxtest/TestSuite.h>


/**
 * Test for the Exception class.
 *
 * @author Swarming Logic
 */
class TestException : public CxxTest::TestSuite
{
public:
  void testExceptionBasic( void )
  {
    const std::string& msg = "mymessage";
    Exception exc(msg);
    TS_ASSERT_EQUALS(exc.toString(), msg);

    // Copy
    Exception e2(exc);
    TS_ASSERT_EQUALS(exc.toString(), msg);

    // Assign construct
    Exception e3 = exc;
    TS_ASSERT_EQUALS(exc.toString(), msg);

    // What()
    TS_ASSERT_SAME_DATA(exc.what(), msg.c_str(), (uint)msg.size());

    // What() extended.
    const std::string msgCopy(exc.what());
    exc = Exception("something else");
    TS_ASSERT_EQUALS(msg, msgCopy);
  }

  void testExceptionThrowCatch( void )
  {
    const std::string& msg = "mymessage";
    bool exceptionCought = false;
    try {
      this->throwException(msg);
    }
    catch (const Exception& exc) {
      TS_ASSERT_EQUALS(exc.toString(), msg);
      exceptionCought = true;
    }
    TS_ASSERT(exceptionCought);
    TS_ASSERT_THROWS(this->throwException(msg), Exception);
  }


  void testExceptionRaise() {
    const std::string& msg = "mymessage";
    bool exceptionCought = true;

   // Raise
    Exception exception(msg);
    TS_ASSERT_THROWS(exception.raise(), Exception);
    try {
      exception.raise();
    }
    catch (const Exception& exc) {
      TS_ASSERT_EQUALS(exc.toString(), msg);
      exceptionCought = true;
    }
    TS_ASSERT(exceptionCought);
  }

private:
  void throwException(const std::string& msg) {
    throw Exception(msg);
  }
};

#endif
