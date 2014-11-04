#ifndef UTIL_TESTFILEUTIL_H
#define UTIL_TESTFILEUTIL_H

#include <cstdio>

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
    const std::string text("This is the file content");
    const std::string fileName("./temporary_testfile.txt");

    writeFile(text, fileName);
    std::string readText = FileUtil::read(fileName,
                                          FileUtil::FILETYPE_WRITABLE);
    TS_ASSERT_EQUALS(text, readText);
    TS_ASSERT(FileUtil::remove(fileName, FileUtil::FILETYPE_WRITABLE));
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
    const std::string filename("./certainlythisdoesnotexist.txt");
    const FileUtil::FileType fileType = FileUtil::FILETYPE_WRITABLE;
    FileUtil::write(filename, "foo", fileType);
    TS_ASSERT_EQUALS(FileUtil::read(filename, fileType), "foo");
    FileUtil::append(filename, "bar", fileType);
    TS_ASSERT_EQUALS(FileUtil::read(filename, fileType), "foobar");
    TS_ASSERT(FileUtil::remove(filename, fileType));
  }

 private:
  void writeFile(const std::string& text,
                 const std::string& fileName) {
    FileUtil::write(fileName, text, FileUtil::FILETYPE_WRITABLE);
  }
};

#endif  // UTIL_TESTFILEUTIL_H
