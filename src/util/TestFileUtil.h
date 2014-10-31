#ifndef UTIL_TESTFILEUTIL_H
#define UTIL_TESTFILEUTIL_H

#include <cstdio>
#include <fstream>

#include <util/Exception.h>
#include <util/FileUtil.h>

#include <cxxtest/TestSuite.h>


/**
 * Test for the FileUtil class.
 *
 * @author Roald Fernandez
 */
class TestFileUtil : public CxxTest::TestSuite
{
 public:
  void testReadFile()
  {
#ifdef __ANDROID__
    TS_SKIP("Not yet implemented for android.");
#endif
    const std::string text("This is the file content");
    const std::string fileName("./temporary_testfile.txt");

    writeFile(text, fileName);
    std::string readText = FileUtil::read(fileName);
    TS_ASSERT_EQUALS(text, readText);
    TS_ASSERT_THROWS(FileUtil::read("thisFileDoesNotExist"),
                     Exception);
    TS_ASSERT_EQUALS(std::remove(fileName.c_str()), 0);
  }


  void testWriteException()
  {
#ifdef __ANDROID__
    TS_SKIP("Not yet implemented for android.");
#endif
    TS_ASSERT_THROWS(FileUtil::write("/foo/bar", "Hello"), Exception e);
    TS_ASSERT_THROWS(FileUtil::append("/foo/bar", "Hello"), Exception e);
  }

  void testWriteAppend()
  {
#ifdef __ANDROID__
    TS_SKIP("Not yet implemented for android.");
#endif
    const std::string filename("./certainlythisdoesnotexist.txt");
    FileUtil::write(filename, "foo");
    TS_ASSERT_EQUALS(FileUtil::read(filename), "foo");
    FileUtil::append(filename, "bar");
    TS_ASSERT_EQUALS(FileUtil::read(filename), "foobar");
    TS_ASSERT(FileUtil::remove(filename.c_str()));
  }

 private:
  void writeFile(const std::string& text,
                 const std::string& fileName) {
    std::ofstream file(fileName);
    TS_ASSERT(file);
    file << text;
  }
};

#endif  // UTIL_TESTFILEUTIL_H
