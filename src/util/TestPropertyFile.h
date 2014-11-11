#ifndef UTIL_TESTPROPERTYFILE_H
#define UTIL_TESTPROPERTYFILE_H

#include <boost/optional.hpp>
#include <boost/property_tree/exceptions.hpp>
#include <boost/property_tree/ptree.hpp>

#include <util/Exception.h>
#include <util/File.h>
#include <util/FileInfo.h>
#include <util/FileUtil.h>
#include <util/PropertyFile.h>

#include <cxxtest/TestSuite.h>


/**
 * Test for the PropertyFile class.
 *
 * @author SwarmingLogic
 */
class TestPropertyFile : public CxxTest::TestSuite
{
 public:
  void testBasicSaveLoad()
  {
    File file(FileInfo("temppropertyfile.conf",
                       FileInfo::TYPE_WRITABLE));
    PropertyFile pf(file.getFileInfo());
    TS_ASSERT(!file.exists());
    TS_ASSERT_THROWS(pf.load(), Exception);
    TS_ASSERT(pf.save());
    TS_ASSERT(file.exists());
    TS_ASSERT_THROWS_NOTHING(pf.load());

    // Cleanup
    TS_ASSERT(file.remove());
  }


  void testHasEntry() {
    File file(FileInfo("doesntmatter.conf",
                       FileInfo::TYPE_WRITABLE));
    PropertyFile pf(file.getFileInfo());
    std::string key = "foo.bar";
    TS_ASSERT(!pf.hasEntry<int>(key));
    TS_ASSERT(!pf.hasEntry(key));

    pf.put(key, 23);
    TS_ASSERT(pf.hasEntry<int>(key));
    TS_ASSERT(pf.hasEntry(key));

    TS_ASSERT_EQUALS(pf.get<int>(key), 23);
    TS_ASSERT_EQUALS(pf.get(key, 42), 23);
    TS_ASSERT_EQUALS(pf.get("foo.fii", 42), 42);

    key = std::string("sec.bar");
    TS_ASSERT(!pf.hasEntry(key));
    pf.put(key, "meh");
    TS_ASSERT(pf.hasEntry(key));
  }


  void testPutSaveLoadGet() {
    File file(FileInfo("temppropertyfile.conf",
                       FileInfo::TYPE_WRITABLE));

    PropertyFile pf(file.getFileInfo());
    TS_ASSERT(!file.exists());

    const int         prop1 = 5412;
    const std::string prop1Key("prop1");
    {
      // Checking when property is <int>
      // Integers can be read as int, float, double, and string.
      TS_ASSERT(!pf.hasEntry(prop1Key));
      pf.put(prop1Key, prop1);
      TS_ASSERT(pf.hasEntry(prop1Key));
      TS_ASSERT(pf.hasEntry<double>(prop1Key));
      TS_ASSERT(pf.hasEntry<int>(prop1Key));
      TS_ASSERT(pf.hasEntry<std::string>(prop1Key));
      TS_ASSERT_EQUALS(pf.get<int>(prop1Key), prop1);
      TS_ASSERT_EQUALS(pf.get<std::string>(prop1Key), "5412");
      TS_ASSERT_DELTA(pf.get<float>(prop1Key), 5412.0f, 0.00001f);
      TS_ASSERT_DELTA(pf.get<double>(prop1Key), 5412.0, 0.00001);
    }

    const std::string prop2("Nah man, it ain't worth it");
    const std::string prop2Key("other.prop2");
    {
      // Checking when property is <string>
      // Strings can only be read as strings, (unless they represent a number)
      TS_ASSERT(!pf.hasEntry(prop2Key));
      pf.put(prop2Key, prop2);
      TS_ASSERT(pf.hasEntry(prop2Key));
      TS_ASSERT(!pf.hasEntry<double>(prop2Key));
      TS_ASSERT(!pf.hasEntry<int>(prop2Key));
      TS_ASSERT(pf.hasEntry<std::string>(prop2Key));
      TS_ASSERT_THROWS(pf.get<int>(prop2Key),
                       boost::property_tree::ptree_error);
      TS_ASSERT_THROWS(pf.get<int>(prop2Key),
                       boost::property_tree::ptree_bad_data);
      TS_ASSERT_THROWS(pf.get<int>("other.nonexistant"),
                       boost::property_tree::ptree_bad_path);
      TS_ASSERT_THROWS_NOTHING(pf.get<std::string>(prop2Key));
      TS_ASSERT_EQUALS(pf.get<std::string>(prop2Key), prop2);
      TS_ASSERT_THROWS(pf.get<float>(prop2Key),
                       boost::property_tree::ptree_bad_data);
      TS_ASSERT_THROWS(pf.get<double>(prop2Key),
                       boost::property_tree::ptree_bad_data);
    }

    const double      prop3  = 124.56122;
    const std::string prop3Key("other.prop3");
    const double      prop3b = 124.0;
    const std::string prop3bKey("other.prop3b");
    {
      // Checking when property is <double>
      // Doubles can only be read as float, string, double, and only integer
      // when it is one.
      TS_ASSERT(!pf.hasEntry(prop3Key));
      pf.put(prop3Key, prop3);
      TS_ASSERT(pf.hasEntry(prop3Key));
      TS_ASSERT(pf.hasEntry<double>(prop3Key));
      TS_ASSERT(!pf.hasEntry<int>(prop3Key));
      TS_ASSERT(pf.hasEntry<std::string>(prop3Key));
      TS_ASSERT_THROWS(pf.get<int>(prop3Key),
                       boost::property_tree::ptree_bad_data);
      TS_ASSERT_EQUALS(pf.get<std::string>(prop3Key), "124.56122");
      TS_ASSERT_DELTA(pf.get<float>(prop3Key), 124.56122f, 0.00001f);
      TS_ASSERT_DELTA(pf.get<double>(prop3Key), 124.56122, 0.00001);

      // Converting this double to integer is ok.
      TS_ASSERT(!pf.hasEntry(prop3bKey));
      pf.put(prop3bKey, prop3b);
      TS_ASSERT(pf.hasEntry(prop3bKey));
      TS_ASSERT(pf.hasEntry<int>(prop3bKey));
    }


    TS_ASSERT(pf.save());
    TS_ASSERT(file.exists());
    TS_ASSERT_THROWS_NOTHING(pf.load());


    // Checking to see if stored file contains expected data.
    PropertyFile pf2(file.getFileInfo());
    TS_ASSERT(file.exists());
    TS_ASSERT(!pf2.hasEntry(prop1Key));
    TS_ASSERT(!pf2.hasEntry(prop2Key));
    TS_ASSERT(!pf2.hasEntry(prop3Key));
    TS_ASSERT(!pf2.hasEntry(prop3bKey));
    TS_ASSERT_THROWS_NOTHING(pf2.load());
    TS_ASSERT(pf2.hasEntry(prop1Key));
    TS_ASSERT(pf2.hasEntry(prop2Key));
    TS_ASSERT(pf2.hasEntry(prop3Key));
    TS_ASSERT(pf2.hasEntry(prop3bKey));

    // Consitancy with previous PropertyFile object
    // Prop1 tests
    TS_ASSERT(pf2.hasEntry(prop1Key));
    TS_ASSERT(pf2.hasEntry<double>(prop1Key));
    TS_ASSERT(pf2.hasEntry<int>(prop1Key));
    TS_ASSERT(pf2.hasEntry<std::string>(prop1Key));
    TS_ASSERT_EQUALS(pf2.get<int>(prop1Key), prop1);
    TS_ASSERT_EQUALS(pf2.get<std::string>(prop1Key), "5412");
    TS_ASSERT_DELTA(pf2.get<float>(prop1Key), 5412.0f, 0.00001f);
    TS_ASSERT_DELTA(pf2.get<double>(prop1Key), 5412.0, 0.00001);

    // Prop1 tests
    TS_ASSERT(!pf2.hasEntry<double>(prop2Key));
    TS_ASSERT(!pf2.hasEntry<int>(prop2Key));
    TS_ASSERT(pf2.hasEntry<std::string>(prop2Key));
    TS_ASSERT_THROWS(pf2.get<int>(prop2Key),
                     boost::property_tree::ptree_error);
    TS_ASSERT_THROWS(pf2.get<int>(prop2Key),
                     boost::property_tree::ptree_bad_data);
    TS_ASSERT_THROWS(pf2.get<int>("other.nonexistant"),
                     boost::property_tree::ptree_bad_path);
    TS_ASSERT_THROWS_NOTHING(pf2.get<std::string>(prop2Key));
    TS_ASSERT_EQUALS(pf2.get<std::string>(prop2Key), prop2);
    TS_ASSERT_THROWS(pf2.get<float>(prop2Key),
                     boost::property_tree::ptree_bad_data);
    TS_ASSERT_THROWS(pf2.get<double>(prop2Key),
                     boost::property_tree::ptree_bad_data);

    // Prop2 tests
    TS_ASSERT(!pf2.hasEntry<double>(prop2Key));
    TS_ASSERT(!pf2.hasEntry<int>(prop2Key));
    TS_ASSERT(pf2.hasEntry<std::string>(prop2Key));
    TS_ASSERT_THROWS(pf2.get<int>(prop2Key),
                     boost::property_tree::ptree_error);
    TS_ASSERT_THROWS(pf2.get<int>(prop2Key),
                     boost::property_tree::ptree_bad_data);
    TS_ASSERT_THROWS(pf2.get<int>("other.nonexistant"),
                     boost::property_tree::ptree_bad_path);
    TS_ASSERT_THROWS_NOTHING(pf2.get<std::string>(prop2Key));
    TS_ASSERT_EQUALS(pf2.get<std::string>(prop2Key), prop2);
    TS_ASSERT_THROWS(pf2.get<float>(prop2Key),
                     boost::property_tree::ptree_bad_data);
    TS_ASSERT_THROWS(pf2.get<double>(prop2Key),
                     boost::property_tree::ptree_bad_data);

    // Prop3, prop3b tests
    TS_ASSERT(pf2.hasEntry<double>(prop3Key));
    TS_ASSERT(!pf2.hasEntry<int>(prop3Key));
    TS_ASSERT(pf2.hasEntry<std::string>(prop3Key));
    TS_ASSERT_THROWS(pf2.get<int>(prop3Key),
                     boost::property_tree::ptree_bad_data);
    TS_ASSERT_EQUALS(pf2.get<std::string>(prop3Key), "124.56122");
    TS_ASSERT_DELTA(pf2.get<float>(prop3Key), 124.56122f, 0.00001f);
    TS_ASSERT_DELTA(pf2.get<double>(prop3Key), 124.56122, 0.00001);

    // Converting this double to integer is ok.
    TS_ASSERT(pf2.hasEntry(prop3bKey));
    TS_ASSERT(pf2.hasEntry<int>(prop3bKey));


    // Cleanup
    TS_ASSERT(file.remove());
  }

  void testGetOptional() {
    File file(FileInfo("temppropertyfile.conf",
                       FileInfo::TYPE_WRITABLE));

    PropertyFile pf(file.getFileInfo());
    TS_ASSERT(!file.exists());

    const int         prop1 = 5412;
    const std::string prop1Key("prop1");
    {
      // Checking when property is <int>
      // Integers can be read as int, float, double, and string.
      pf.put(prop1Key, prop1);
      TS_ASSERT(pf.getOptional<double>(prop1Key));
      TS_ASSERT(pf.getOptional<int>(prop1Key));
      TS_ASSERT(pf.getOptional<std::string>(prop1Key));
      TS_ASSERT_EQUALS(pf.getOptional<int>(prop1Key).get(), prop1);
      TS_ASSERT_EQUALS(pf.getOptional<std::string>(prop1Key).get(), "5412");
      TS_ASSERT_DELTA(pf.getOptional<float>(prop1Key).get(), 5412.0f, 0.00001f);
      TS_ASSERT_DELTA(pf.getOptional<double>(prop1Key).get(), 5412.0, 0.00001);
    }

    const std::string prop2("Nah man, it ain't worth it");
    const std::string prop2Key("other.prop2");
    {
      // Checking when property is <string>
      // Strings can only be read as strings, (unless they represent a number)
      pf.put(prop2Key, prop2);
      TS_ASSERT(!pf.getOptional<double>(prop2Key));
      TS_ASSERT(!pf.getOptional<int>(prop2Key));
      TS_ASSERT(pf.getOptional<std::string>(prop2Key));
      TS_ASSERT_THROWS(pf.get<int>(prop2Key),
                       boost::property_tree::ptree_bad_data);
      TS_ASSERT(!pf.getOptional<int>("other.nonexistant"));
      TS_ASSERT_EQUALS(pf.getOptional<std::string>(prop2Key).get(), prop2);
      TS_ASSERT(!pf.getOptional<float>(prop2Key));
      TS_ASSERT(!pf.getOptional<double>(prop2Key));
    }
  }

  void testSampleInfoFromFile() {
    const std::string content = R"(; This is a comment

Graphics
{
    ScreenResolution
    {
        w 1196
        h 768
    }
}
)";
    File file("tmpinfo.info", FileInfo::TYPE_WRITABLE);
    TS_ASSERT(file.safeWrite(content));
    TS_ASSERT(file.exists());

    PropertyFile pf(file.getFileInfo());
    TS_ASSERT(!pf.hasEntry("Graphics"));
    TS_ASSERT(!pf.hasEntry("Graphics.ScreenResolution"));
    TS_ASSERT(!pf.hasEntry("Graphics.ScreenResolution.w"));
    TS_ASSERT(!pf.hasEntry("Graphics.ScreenResolution.h"));
    pf.load();
    TS_ASSERT(pf.hasEntry("Graphics"));
    TS_ASSERT(pf.hasEntry("Graphics.ScreenResolution"));
    TS_ASSERT(pf.hasEntry("Graphics.ScreenResolution.w"));
    TS_ASSERT(pf.hasEntry("Graphics.ScreenResolution.h"));
    TS_ASSERT(!pf.hasEntry("Graphics.ScreenResolution.z"));

    TS_ASSERT_EQUALS(pf.get<int>("Graphics.ScreenResolution.w"), 1196);
    TS_ASSERT_EQUALS(pf.get<int>("Graphics.ScreenResolution.h"), 768);

    TS_ASSERT(file.remove());
  }
};

#endif  // UTIL_TESTPROPERTYFILE_H
