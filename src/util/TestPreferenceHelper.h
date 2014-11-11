#ifndef UTIL_TESTPREFERENCEHELPER_H
#define UTIL_TESTPREFERENCEHELPER_H

#include <math/Size.h>
#include <util/Exception.h>
#include <util/FileInfo.h>
#include <util/FileUtil.h>
#include <util/PreferenceHelper.h>
#include <util/PropertyFile.h>

#include <cxxtest/TestSuite.h>


/**
 * Test for the PreferenceHelper class.
 *
 * @author SwarmingLogic
 */
class TestPreferenceHelper : public CxxTest::TestSuite
{
 public:
  void testBasic() {
    // Test accessors to data being handled
    const std::string userPrefsFilename("foo_user_preferences.info");
    const FileInfo userPrefsInfo(userPrefsFilename, FileInfo::TYPE_WRITABLE);
    PropertyFile userPrefs(userPrefsInfo);

    const std::string systemPrefsFilename("foo_linux_preferences.info");
    const FileInfo systemPrefsInfo(systemPrefsFilename,
                                   FileInfo::TYPE_WRITABLE);
    PropertyFile systemPrefs(systemPrefsInfo);

    const std::string defaultPrefsFilename("foo_default_preferences.info");
    const FileInfo defaultPrefsInfo(defaultPrefsFilename,
                                    FileInfo::TYPE_WRITABLE);
    PropertyFile defaultPrefs(defaultPrefsInfo);

    // Empty userprefs file should have been created.
    createSystemPrefs(systemPrefsInfo);
    createDefaultPrefs(defaultPrefsInfo);

    // The class instance to test.
    TS_ASSERT(FileUtil::exists(defaultPrefsInfo));
    TS_ASSERT(FileUtil::exists(systemPrefsInfo));
    TS_ASSERT(!FileUtil::exists(userPrefsInfo));
    PreferenceHelper ph(defaultPrefsInfo, systemPrefsInfo, userPrefsInfo);

    // Empty userprefs file should have been created.
    TS_ASSERT(FileUtil::exists(userPrefsInfo));
    createUserPrefs(userPrefsInfo);
    ph.load();

    TS_ASSERT_THROWS(ph.get<float>("key.does.not.exist"), Exception);

    TS_ASSERT(ph.hasEntry("Graphics.OtherNode"));
    TS_ASSERT(ph.hasEntry("Graphics.OtherNode.key"));
    TS_ASSERT(ph.hasEntry("Graphics.ScreenResolution.z"));
    TS_ASSERT(!ph.hasEntry("nope"));
    TS_ASSERT_EQUALS(ph.get<bool>("Graphics.OtherNode.key"), true);
    TS_ASSERT_EQUALS(ph.get<int>("Graphics.ScreenResolution.z"), 1337);
    TS_ASSERT_EQUALS(ph.get<int>("Graphics.ScreenResolution.w"), 1024);
    TS_ASSERT_EQUALS(ph.get<int>("Graphics.ScreenResolution.h"), 768);
    TS_ASSERT_EQUALS(ph.get<std::string>("Graphics.p"), "OK");

    // Set a value, save, then load with PropertyFile, compare
    userPrefs.load();
    ph.put("Graphics.ScreenResolution.w", 1112);
    TS_ASSERT_EQUALS(ph.get<int>("Graphics.ScreenResolution.w"), 1112);
    TS_ASSERT_EQUALS(userPrefs.get<int>("Graphics.ScreenResolution.w"), 1024);
    ph.save();
    userPrefs.load();
    TS_ASSERT_EQUALS(ph.get<int>("Graphics.ScreenResolution.w"),
                     userPrefs.get<int>("Graphics.ScreenResolution.w"));

    // // const Size res = ph.getCurrentResolution();
    TS_ASSERT(FileUtil::remove(userPrefsInfo));
    TS_ASSERT(FileUtil::remove(systemPrefsInfo));
    TS_ASSERT(FileUtil::remove(defaultPrefsInfo));
  }

 private:
  void createDefaultPrefs(const FileInfo& fileInfo) {
    const std::string content = R"(; This is a comment

Graphics
{
    ScreenResolution
    {
        w 640
        h 480
        z 1337
    }
    OtherNode
    {
        key true
    }
    p MEH
}
)";
    FileUtil::write(fileInfo, content);
  }

  void createSystemPrefs(const FileInfo& fileInfo) {
    const std::string content = R"(; This is a comment

Graphics
{
    ScreenResolution
    {
        w 1196
        h 768
    }
    p OK
}
)";
    FileUtil::write(fileInfo, content);
  }


  void createUserPrefs(const FileInfo& fileInfo) {
    const std::string content = R"(; This is a comment

Graphics
{
    ScreenResolution
    {
        w 1024
        h 768
    }
}
)";
    FileUtil::write(fileInfo, content);
  }


};


#endif  // UTIL_TESTPREFERENCEHELPER_H
