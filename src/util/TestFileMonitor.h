#ifndef UTIL_TESTFILEMONITOR_H
#define UTIL_TESTFILEMONITOR_H

#include <chrono>
#include <thread>

#include <util/FileMonitor.h>
#include <util/FileUtil.h>

#include <cxxtest/TestSuite.h>


// #define SLOW_TESTS

/**
 * Test for the File class.
 *
 * @author SwarmingLogic (Roald Fernandez)
 */
class TestFileMonitor : public CxxTest::TestSuite
{
private:
  void msleep(unsigned milliseconds) {
    std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
  }

public:
  void testBasics()
  {
    const std::string filename("./certainlythisdoesnotexist.txt");
    FileMonitor file(filename);
    TS_ASSERT_EQUALS(filename, file.getFilename());
  }


  void testExists()
  {
    const std::string filename("./certainlythisdoesnotexist.txt");
    TS_ASSERT(!FileUtil::exists(filename));
    FileMonitor file(filename);
    TS_ASSERT(!file.exists());
    TS_ASSERT(!file.exists());

    const std::string content { "This is the content." };
    FileUtil::write(filename, content);
    TS_ASSERT(FileUtil::exists(filename));
    TS_ASSERT(FileUtil::remove(filename.c_str()));
  }


  void testIsUpdated()
  {
    const std::string filename("./certainlythisdoesnotexist.txt");
    TS_ASSERT(!FileUtil::exists(filename));

    FileMonitor file(filename);

    // Write some content, check if file exists now
    const std::string content { "This is the content." };
    FileUtil::write(filename, content);
    TS_ASSERT(FileUtil::exists(filename));
    TS_ASSERT(file.isUpdated());

    // Reset timestamp
    file.resetTimeStamp();
    TS_ASSERT(!file.isUpdated());

#ifdef SLOW_TESTS
    msleep(1200);
    FileUtil::write(filename, "changed");
    TS_ASSERT(file.isUpdated());
#endif

    TS_ASSERT(FileUtil::remove(filename.c_str()));
  }
};

#endif
