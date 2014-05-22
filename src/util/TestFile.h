#ifndef UTIL_TESTFILE_H
#define UTIL_TESTFILE_H

#include <chrono>
#include <thread>

#include <util/File.h>
#include <util/FileUtil.h>

#include <cxxtest/TestSuite.h>

//#define SLOW_TESTS

/**
 * Test for the File class.
 *
 * @author SwarmingLogic (Roald Fernandez)
 */
class TestFile : public CxxTest::TestSuite
{
private:
  void msleep(unsigned milliseconds) {
    std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
  }

public:
  void testBasics()
  {
    const std::string filename("./certainlythisdoesnotexist.txt");
    File file(filename);
    TS_ASSERT_EQUALS(filename, file.getFilename());

    // Default constructor
    File file3;
    TS_ASSERT(!file3.exists());
  }


  void testExists()
  {
    const std::string filename("./certainlythisdoesnotexist.txt");
    TS_ASSERT(!FileUtil::exists(filename));
    File file(filename);
    TS_ASSERT(!file.exists());
    TS_ASSERT(!file.exists());

    const std::string content { "This is the content." };
    FileUtil::write(filename, content);
    TS_ASSERT(FileUtil::exists(filename));
    TS_ASSERT(FileUtil::remove(filename.c_str()));
  }


  void testRead()
  {
    const std::string filename("./certainlythisdoesnotexist.txt");
    TS_ASSERT(!FileUtil::exists(filename));

    File file(filename);

    // Write some content, check if file exists now
    const std::string content { "This is the content." };
    FileUtil::write(filename, content);
    TS_ASSERT(FileUtil::exists(filename));

    // Read the file, check if it has updated since last read.
    const std::string& loaded = file.read();
    TS_ASSERT_EQUALS(loaded, content);
    TS_ASSERT(!file.isUpdated());
    TS_ASSERT(!file.isModified());
    TS_ASSERT(!file.isModified());
    TS_ASSERT_EQUALS(file.readCopy(), content);

    // [modify], isModified, [modify], readCopy, !isModified
    const std::string text1 {"Text1"};
    FileUtil::write(filename, text1);
    TS_ASSERT(file.isModified());
    TS_ASSERT(file.isModified());
    std::string getText1 = {file.readCopy()};
    TS_ASSERT_EQUALS(text1, getText1);
    TS_ASSERT(!file.isModified());

    // [modify], isModified, [modify], read, !isModified
    const std::string text2 {"Text2"};
    FileUtil::write(filename, text2);
    TS_ASSERT(file.isModified());
    std::string getText2 = {file.read()};
    TS_ASSERT(!file.isModified());


#ifdef SLOW_TESTS
    msleep(1200);
    FileUtil::write(filename, "changed");
    TS_ASSERT(file.isUpdated());
    TS_ASSERT(file.isModified());
#endif

    TS_ASSERT(FileUtil::remove(filename.c_str()));
  }

  void testReadToLocal()
  {
    const std::string filename("./certainlythisdoesnotexist.txt");
    TS_ASSERT(!FileUtil::exists(filename));

    File file(filename);
    const std::string content { "This is the content." };
    FileUtil::write(filename, content);
    file.readToLocal();
    TS_ASSERT_EQUALS(file.readCopy(), content);
    TS_ASSERT(FileUtil::remove(filename.c_str()));
  }

  void testCopy()
  {
    const std::string filename("./certainlythisdoesnotexist.txt");
    TS_ASSERT(!FileUtil::exists(filename));

    // Basic
    File file(filename);
    File filecp = file;
    File fileas(file);
    TS_ASSERT_EQUALS(file.getFilename(), filecp.getFilename());
    TS_ASSERT_EQUALS(file.getFilename(), fileas.getFilename());

    // After copy
    const std::string text1 {"Text1"};
    FileUtil::write(filename, text1);
    TS_ASSERT(file.isModified());
    File file3(file);
    File file4 = file;
    TS_ASSERT(file3.isModified());
    TS_ASSERT(file4.isModified());
    TS_ASSERT_EQUALS(file3.read(), text1);
    TS_ASSERT(!file3.isModified());
    TS_ASSERT(file4.isModified());

    TS_ASSERT(FileUtil::remove(filename.c_str()));
  }
};

#endif
