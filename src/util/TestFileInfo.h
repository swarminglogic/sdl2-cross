#ifndef UTIL_TESTFILEINFO_H
#define UTIL_TESTFILEINFO_H

#include <util/FileInfo.h>

#include <cxxtest/TestSuite.h>


/**
 * Test for the FileInfo class.
 *
 * @author SwarmingLogic
 */
class TestFileInfo : public CxxTest::TestSuite
{
 public:
  void testBasics()
  {
    {
      FileInfo fi;
      TS_ASSERT_EQUALS(fi.getFilename(), "");
      TS_ASSERT_EQUALS(fi.getFiletype(), FileInfo::TYPE_UNSPECIFIED);
    }

    {
      FileInfo fi("barf");
      TS_ASSERT_EQUALS(fi.getFilename(), "barf");
      TS_ASSERT_EQUALS(fi.getFiletype(), FileInfo::TYPE_UNSPECIFIED);
    }

    {
      FileInfo fi("moomoo", FileInfo::TYPE_WRITABLE);
      TS_ASSERT_EQUALS(fi.getFilename(), "moomoo");
      TS_ASSERT_EQUALS(fi.getFiletype(), FileInfo::TYPE_WRITABLE);
    }
  }

  void testGetterSetter() {
    FileInfo fi;
    TS_ASSERT_EQUALS(fi.getFilename(), "");
    TS_ASSERT_EQUALS(fi.getFiletype(), FileInfo::TYPE_UNSPECIFIED);
    fi.setFilename("mehrah");
    TS_ASSERT_EQUALS(fi.getFilename(), "mehrah");
    fi.setFiletype(FileInfo::TYPE_ASSET);
    TS_ASSERT_EQUALS(fi.getFiletype(), FileInfo::TYPE_ASSET);
    fi.setFiletype(FileInfo::TYPE_WRITABLE);
    TS_ASSERT_EQUALS(fi.getFiletype(), FileInfo::TYPE_WRITABLE);
    fi.setFiletype(FileInfo::TYPE_UNSPECIFIED);
    TS_ASSERT_EQUALS(fi.getFiletype(), FileInfo::TYPE_UNSPECIFIED);
  }

 private:
};

#endif  // UTIL_TESTFILEINFO_H
