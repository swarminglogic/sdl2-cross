#ifndef UTIL_TESTFILEMONITOR_H
#define UTIL_TESTFILEMONITOR_H

#include <chrono>
#include <thread>

#include <util/FileInfo.h>
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
    FileMonitor fileMon(filename);
    TS_ASSERT_EQUALS(filename, fileMon.getFilename());
  }


  void testExists()
  {
    const std::string filename("./certainlythisdoesnotexist.txt");
    TS_ASSERT(!FileUtil::exists(filename, FileInfo::TYPE_WRITABLE));
    FileMonitor fileMon(filename, FileInfo::TYPE_WRITABLE);
    TS_ASSERT(!fileMon.exists());
    TS_ASSERT(!fileMon.exists());

    const std::string content { "This is the content." };
    FileUtil::write(filename, content, FileInfo::TYPE_WRITABLE);
    TS_ASSERT(FileUtil::exists(filename, FileInfo::TYPE_WRITABLE));
    TS_ASSERT(FileUtil::remove(filename, FileInfo::TYPE_WRITABLE));
  }


  void testIsUpdated()
  {
    const std::string filename("./certainlythisdoesnotexist.txt");
    TS_ASSERT(!FileUtil::exists(filename, FileInfo::TYPE_WRITABLE));

    FileMonitor fileMon(filename, FileInfo::TYPE_WRITABLE);

    // Write some content, check if fileMon exists now
    const std::string content { "This is the content." };
    FileUtil::write(filename, content, FileInfo::TYPE_WRITABLE);
    TS_ASSERT(FileUtil::exists(filename, FileInfo::TYPE_WRITABLE));
    TS_ASSERT(fileMon.isUpdated());

    // Reset timestamp
    fileMon.resetTimeStamp();
    TS_ASSERT(!fileMon.isUpdated());

#ifdef SLOW_TESTS
    msleep(1200);
    FileUtil::write(filename, "changed", FileInfo::TYPE_WRITABLE);
    TS_ASSERT(fileMon.isUpdated());
#endif

    TS_ASSERT(FileUtil::remove(filename, FileInfo::TYPE_WRITABLE));
  }
};

#endif  // UTIL_TESTFILEMONITOR_H
