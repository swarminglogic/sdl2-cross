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

  void testReadFile( void )
  {
    const std::string text("This is the file content");
    const std::string fileName("./temporary_testfile.txt");

    writeFile(text, fileName);
    std::string readText = FileUtil::read(fileName);
    TS_ASSERT_EQUALS(text, readText);
    TS_ASSERT_THROWS(FileUtil::read("thisFileDoesNotExist"),
                     Exception);
    TS_ASSERT_EQUALS(std::remove(fileName.c_str()), 0);
  }

private:

  void writeFile(const std::string& text,
                 const std::string& fileName) {
    std::ofstream file(fileName);
    TS_ASSERT(file);
    file << text;
  }
};

#endif
