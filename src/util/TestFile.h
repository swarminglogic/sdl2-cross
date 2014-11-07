#ifndef UTIL_TESTFILE_H
#define UTIL_TESTFILE_H

#include <chrono>
#include <thread>

#include <util/File.h>
#include <util/FileInfo.h>
#include <util/FileUtil.h>

#include <cxxtest/TestSuite.h>


//  #define SLOW_TESTS

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
    TS_ASSERT(!FileUtil::exists(filename,
                                FileInfo::TYPE_WRITABLE));
    File file(filename, FileInfo::TYPE_WRITABLE);
    TS_ASSERT(!file.exists());
    TS_ASSERT(!file.exists());

    const std::string content { "This is the content." };
    FileUtil::write(filename, content,
                    FileInfo::TYPE_WRITABLE);
    TS_ASSERT(file.exists());
    TS_ASSERT(FileUtil::exists(filename,
                               FileInfo::TYPE_WRITABLE));
    TS_ASSERT(FileUtil::remove(filename, FileInfo::TYPE_WRITABLE));
    TS_ASSERT(!file.exists());
  }


  void testRead()
  {
    const std::string filename("./certainlythisdoesnotexist.txt");
    TS_ASSERT(!FileUtil::exists(filename,
                                FileInfo::TYPE_WRITABLE));
    File file(filename, FileInfo::TYPE_WRITABLE);

    // Write some content, check if file exists now
    const std::string content { "This is the content." };
    FileUtil::write(filename, content, FileInfo::TYPE_WRITABLE);
    TS_ASSERT(FileUtil::exists(filename, FileInfo::TYPE_WRITABLE));

    // Read the file, check if it has updated since last read.
    const std::string& loaded = file.read();
    TS_ASSERT_EQUALS(loaded, content);
    TS_ASSERT(!file.isUpdated());
    TS_ASSERT(!file.update());
    TS_ASSERT(!file.update());
    TS_ASSERT_EQUALS(file.getLocalCopy(), content);

    // [modify], update, !update, [modify], getLocalCopy, !update
    const std::string text1 {"Text1"};
    FileUtil::write(filename, text1, FileInfo::TYPE_WRITABLE);
    TS_ASSERT(file.update());
    TS_ASSERT(!file.update());
    std::string getText1 = {file.getLocalCopy()};
    TS_ASSERT_EQUALS(text1, getText1);
    TS_ASSERT(!file.update());

    // [modify], update, [modify], read, !update
    const std::string text2 {"Text2"};
    FileUtil::write(filename, text2, FileInfo::TYPE_WRITABLE);
    TS_ASSERT(file.update());
    std::string getText2 = {file.read()};
    TS_ASSERT(!file.update());

#ifdef SLOW_TESTS
    msleep(1200);
    FileUtil::write(filename, "changed", FileInfo::TYPE_WRITABLE);
    TS_ASSERT(file.isUpdated());
    TS_ASSERT(file.update());
#endif

    TS_ASSERT(file.remove());
  }


  void testWriteAppend()
  {
    File file("./certainlythisdoesnotexist.txt",
              FileInfo::TYPE_WRITABLE);
    TS_ASSERT(!file.exists());
    file.write("foo");
    TS_ASSERT_EQUALS(file.read(), "foo");
    file.append("bar");
    TS_ASSERT_EQUALS(file.read(), "foobar");
    TS_ASSERT(file.remove());
  }

  void testRename()
  {
    const FileInfo fileInfo("foobarfile.txt", FileInfo::TYPE_WRITABLE);
    const FileInfo tempFileInfo("meeh.info", FileInfo::TYPE_WRITABLE);
    File file(fileInfo);
    File tmpFile(tempFileInfo);
    TS_ASSERT(!file.exists());
    TS_ASSERT(!tmpFile.exists());

    file.write("foom");
    TS_ASSERT(file.exists());
    TS_ASSERT(!tmpFile.exists());

    // Trying to rename non existing file to existing file.
    TS_ASSERT(!tmpFile.rename(fileInfo));
    TS_ASSERT(file.exists());
    TS_ASSERT(!tmpFile.exists());

    // Trying to rename to self:
    TS_ASSERT(!file.rename(fileInfo));
    TS_ASSERT(!file.rename(fileInfo.getFilename()));
    TS_ASSERT(file.exists());
    TS_ASSERT(!tmpFile.exists());

    // Renaming to new file
    TS_ASSERT(FileUtil::exists(fileInfo));
    TS_ASSERT(file.rename(tempFileInfo));
    TS_ASSERT_EQUALS(file.getFilename(), tempFileInfo.getFilename());
    TS_ASSERT(!FileUtil::exists(fileInfo));
    TS_ASSERT(tmpFile.exists());

    TS_ASSERT(tmpFile.remove());
    TS_ASSERT(!tmpFile.exists());
    TS_ASSERT(!file.exists());
  }

  void testRenameOverrite() {
    const FileInfo fileInfo("foobarfile.txt", FileInfo::TYPE_WRITABLE);
    const FileInfo tempFileInfo("foobarfile.txt.tmp", FileInfo::TYPE_WRITABLE);
    File file(fileInfo);
    File tmpFile(tempFileInfo);
    TS_ASSERT(!file.exists());
    TS_ASSERT(!tmpFile.exists());

    file.write("Content of old file.");
    tmpFile.write("New data in the file.");
    tmpFile.rename(file.getFilename());
    TS_ASSERT_EQUALS(file.read(), tmpFile.read());
    TS_ASSERT_EQUALS(file.read(), "New data in the file.");
    TS_ASSERT(tmpFile.remove());
    TS_ASSERT(!tmpFile.exists());
    TS_ASSERT(!file.exists());
  }


  void testSafeWrite() {
    // Hard to test that it fails correctly.
    File file("somefile.txt", FileInfo::TYPE_WRITABLE);
    TS_ASSERT(!file.exists());
    TS_ASSERT(file.safeWrite("Foo was written"));
    TS_ASSERT(file.exists());
    TS_ASSERT_EQUALS(file.read(), "Foo was written");

    TS_ASSERT(file.safeWrite("And it was good"));
    TS_ASSERT_EQUALS(file.read(), "And it was good");
    TS_ASSERT(file.remove());
    TS_ASSERT(!file.exists());
  }


  void testReadToLocal()
  {
    const std::string filename("./certainlythisdoesnotexist.txt");

    File file(filename, FileInfo::TYPE_WRITABLE);
    const File tmpFile(filename + ".tmp", FileInfo::TYPE_WRITABLE);
    TS_ASSERT(!file.exists());
    TS_ASSERT(!tmpFile.exists());
    const std::string content { "This is the content." };
    file.write(content);
    file.update();
    TS_ASSERT_EQUALS(file.getLocalCopy(), content);
    TS_ASSERT(file.remove());
  }

  void testCopy()
  {
    const std::string filename("./certainlythisdoesnotexist.txt");
    TS_ASSERT(!FileUtil::exists(filename,
                                FileInfo::TYPE_WRITABLE));

    // Basic
    File file(filename, FileInfo::TYPE_WRITABLE);
    File filecp = file;
    File fileas(file);
    TS_ASSERT_EQUALS(file.getFilename(), filecp.getFilename());
    TS_ASSERT_EQUALS(file.getFilename(), fileas.getFilename());
    TS_ASSERT_EQUALS(file.getFileInfo().getFilename(),
                     fileas.getFileInfo().getFilename());
    TS_ASSERT_EQUALS(file.getFileInfo().getFiletype(),
                     fileas.getFileInfo().getFiletype());
    TS_ASSERT_EQUALS(file.getFileInfo().getFilename(),
                     filecp.getFileInfo().getFilename());
    TS_ASSERT_EQUALS(file.getFileInfo().getFiletype(),
                     filecp.getFileInfo().getFiletype());


    // After copy
    const std::string text1 {"Text1"};
    file.write(text1);
    TS_ASSERT(file.update());
    File file3(file);
    File file4 = file;
    TS_ASSERT(!file3.update());
    TS_ASSERT(!file4.update());
    TS_ASSERT_EQUALS(file3.read(), text1);

    TS_ASSERT(file.exists());
    TS_ASSERT(file3.exists());
    TS_ASSERT(file4.exists());
    TS_ASSERT(file.remove());
    TS_ASSERT(!file.exists());
    TS_ASSERT(!file3.exists());
    TS_ASSERT(!file4.exists());
  }

  void testSetFilename()
  {
    const std::string filename("./thisdoesexist.txt");
    FileUtil::write(filename, "foobar", FileInfo::TYPE_WRITABLE);

    File file(filename, FileInfo::TYPE_WRITABLE);
    TS_ASSERT(file.exists());

    const std::string filename2("./certainlythisdoesnotexist.txt");
    file.setFilename(filename2);
    TS_ASSERT(!file.exists());

    file.setFilename(filename);
    TS_ASSERT(file.exists());
    TS_ASSERT(file.remove());
  }
};

#endif  // UTIL_TESTFILE_H
