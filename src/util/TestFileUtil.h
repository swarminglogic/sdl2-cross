#ifndef UTIL_TESTFILEUTIL_H
#define UTIL_TESTFILEUTIL_H

#include <cstdio>

#include <util/Exception.h>
#include <util/FileInfo.h>
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
                                          FileInfo::TYPE_WRITABLE);
    TS_ASSERT_EQUALS(text, readText);
    TS_ASSERT(FileUtil::remove(fileName, FileInfo::TYPE_WRITABLE));
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
    const FileInfo::FileType fileType = FileInfo::TYPE_WRITABLE;
    FileUtil::write(filename, "foo", fileType);
    TS_ASSERT_EQUALS(FileUtil::read(filename, fileType), "foo");
    FileUtil::append(filename, "bar", fileType);
    TS_ASSERT_EQUALS(FileUtil::read(filename, fileType), "foobar");
    TS_ASSERT(FileUtil::remove(filename, fileType));
  }

  void testSafeWrite() {
    // Hard to test that it fails correctly.
    const FileInfo::FileType ft = FileInfo::TYPE_WRITABLE;
    const std::string fn("somefile.txt");
    const FileInfo fi(fn, ft);
    TS_ASSERT(!FileUtil::exists(fi));
    TS_ASSERT(FileUtil::safeWrite(fi, "Foo was written"));
    TS_ASSERT(FileUtil::exists(fi));
    TS_ASSERT_EQUALS(FileUtil::read(fi), "Foo was written");

    TS_ASSERT(FileUtil::safeWrite(fi, "And it was good"));
    TS_ASSERT_EQUALS(FileUtil::read(fi), "And it was good");
    TS_ASSERT(FileUtil::remove(fi));
    TS_ASSERT(!FileUtil::exists(fi));
    TS_ASSERT(!FileUtil::exists(fn + ".tmp", ft));
  }


  void testExistsRemove()
  {
    const std::string filename("./certainlythisdoesnotexist.txt");
    const FileInfo::FileType fileType = FileInfo::TYPE_WRITABLE;
    TS_ASSERT(!FileUtil::exists(filename, fileType));
    FileUtil::write(filename, "foo", fileType);
    TS_ASSERT(FileUtil::exists(filename, fileType));
    TS_ASSERT(FileUtil::remove(filename, fileType));
    TS_ASSERT(!FileUtil::exists(filename, fileType));
  }

  void testRename()
  {
    const std::string filename("foobarfile.txt");
    const std::string newFilename("mofbanana.txt");
    const FileInfo::FileType fileType = FileInfo::TYPE_WRITABLE;
    TS_ASSERT(!FileUtil::exists(filename, fileType));
    TS_ASSERT(!FileUtil::exists(newFilename, fileType));

    FileUtil::write(filename, "foom", fileType);
    TS_ASSERT(FileUtil::exists(filename, fileType));
    TS_ASSERT(!FileUtil::exists(newFilename, fileType));

    // Trying to rename non existing file to existing file.
    TS_ASSERT(!FileUtil::rename(newFilename, filename, fileType));
    TS_ASSERT(FileUtil::exists(filename, fileType));
    TS_ASSERT(!FileUtil::exists(newFilename, fileType));

    // Renaming to self
    TS_ASSERT(!FileUtil::rename(filename, filename, fileType));
    TS_ASSERT(FileUtil::exists(filename, fileType));
    TS_ASSERT(!FileUtil::exists(newFilename, fileType));

    // Renaming to new file
    TS_ASSERT(FileUtil::rename(filename, newFilename, fileType));
    TS_ASSERT(!FileUtil::exists(filename, fileType));
    TS_ASSERT(FileUtil::exists(newFilename, fileType));
    TS_ASSERT_EQUALS(FileUtil::read(newFilename, fileType), "foom");

    TS_ASSERT(!FileUtil::remove(filename, fileType));
    TS_ASSERT(FileUtil::remove(newFilename, fileType));
    TS_ASSERT(!FileUtil::exists(filename, fileType));
    TS_ASSERT(!FileUtil::exists(newFilename, fileType));
  }


 private:
  void writeFile(const std::string& text,
                 const std::string& fileName) {
    FileUtil::write(fileName, text, FileInfo::TYPE_WRITABLE);
  }
};

#endif  // UTIL_TESTFILEUTIL_H
