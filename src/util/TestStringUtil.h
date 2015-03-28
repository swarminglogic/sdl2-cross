#ifndef UTIL_TESTSTRINGUTIL_H
#define UTIL_TESTSTRINGUTIL_H

#include <string>
#include <vector>

#include <util/StringUtil.h>

#include <cxxtest/TestSuite.h>


/**
 * Test for the StringUtil class.
 *
 * @author SwarmingLogic (Roald Fernandez)
 */
class TestStringUtil : public CxxTest::TestSuite
{
 public:
  void testSplit()
  {
    std::string txt {"Hello my \n   sweet world!"};
    std::vector<std::string> res = StringUtil::split(txt, '\n');
    TS_ASSERT(!res.empty());
    TS_ASSERT_EQUALS(res.size(), 2);
    TS_ASSERT_EQUALS(res[0], "Hello my ");
    TS_ASSERT_EQUALS(res[1], "   sweet world!");
  }


  void testTrimSplit()
  {
    std::string txt {R"(Hello
  my
 sweet
world!   )"};
    std::vector<std::string> res = StringUtil::trimSplit(txt);
    TS_ASSERT(!res.empty());
    TS_ASSERT_EQUALS(res.size(), 4);
    TS_ASSERT_EQUALS(res[0], "Hello");
    TS_ASSERT_EQUALS(res[1], "my");
    TS_ASSERT_EQUALS(res[2], "sweet");
    TS_ASSERT_EQUALS(res[3], "world!");

    std::string txt2 {"Hello   my   sweet  world!   "};
    std::vector<std::string> res2 = StringUtil::trimSplit(txt2, ' ');
    TS_ASSERT(!res2.empty());
    TS_ASSERT_EQUALS(res2.size(), 4);
    TS_ASSERT_EQUALS(res2[0], "Hello");
    TS_ASSERT_EQUALS(res2[1], "my");
    TS_ASSERT_EQUALS(res2[2], "sweet");
    TS_ASSERT_EQUALS(res2[3], "world!");
  }


  void testTrim()
  {
    std::string txt {"   Hello world!   "};
    TS_ASSERT_EQUALS(StringUtil::ltrimc(txt), "Hello world!   ");
    TS_ASSERT_EQUALS(StringUtil::rtrimc(txt), "   Hello world!");
    TS_ASSERT_EQUALS(StringUtil::trimc(txt), "Hello world!");

    std::string rt {"   Hello world!   "};
    StringUtil::rtrim(rt);
    TS_ASSERT_EQUALS(rt, "   Hello world!");
    std::string lt {"   Hello world!   "};
    StringUtil::ltrim(lt);
    TS_ASSERT_EQUALS(lt, "Hello world!   ");
    std::string ct {"   Hello world!   "};
    StringUtil::trim(ct);
    TS_ASSERT_EQUALS(ct, "Hello world!");
  }

  void testSuffixPrefix()
  {
    std::string txt {"abcdef012345"};

    // Prefix
    TS_ASSERT_EQUALS(StringUtil::prefix(txt, 0), "");
    TS_ASSERT_EQUALS(StringUtil::prefix(txt, 4), "abcd");
    TS_ASSERT_EQUALS(StringUtil::prefix(txt, 200), txt);

    // Suffix
    TS_ASSERT_EQUALS(StringUtil::suffix(txt, 0), "");
    TS_ASSERT_EQUALS(StringUtil::suffix(txt, 4), "2345");
    TS_ASSERT_EQUALS(StringUtil::suffix(txt, 200), txt);
  }


  void testSuffixPrefixNegative()
  {
    std::string txt {"abcdef012345"};

    // Prefix
    TS_ASSERT_EQUALS(StringUtil::prefix(txt, 0),    "");
    TS_ASSERT_EQUALS(StringUtil::prefix(txt, -0),   "");
    TS_ASSERT_EQUALS(StringUtil::prefix(txt, -1),   "abcdef01234");
    TS_ASSERT_EQUALS(StringUtil::prefix(txt, -4),   "abcdef01");
    TS_ASSERT_EQUALS(StringUtil::prefix(txt, -11),  "a");
    TS_ASSERT_EQUALS(StringUtil::prefix(txt, -12),  "");
    TS_ASSERT_EQUALS(StringUtil::prefix(txt, -13),  "");
    TS_ASSERT_EQUALS(StringUtil::prefix(txt, -200), "");

    // Suffix
    TS_ASSERT_EQUALS(StringUtil::suffix(txt, 0),    "");
    TS_ASSERT_EQUALS(StringUtil::suffix(txt, -0),   "");
    TS_ASSERT_EQUALS(StringUtil::suffix(txt, -1),   "bcdef012345");
    TS_ASSERT_EQUALS(StringUtil::suffix(txt, -4),   "ef012345");
    TS_ASSERT_EQUALS(StringUtil::suffix(txt, -11),  "5");
    TS_ASSERT_EQUALS(StringUtil::suffix(txt, -12),  "");
    TS_ASSERT_EQUALS(StringUtil::suffix(txt, -13),  "");
    TS_ASSERT_EQUALS(StringUtil::suffix(txt, -200), "");
  }

  void testPrepend() {
    // In place
    {
      std::string txt {"abcdef012345"};
      std::string pre {"Yo dude!"};
      StringUtil::prepend(pre, txt);
      TS_ASSERT_EQUALS(txt, "Yo dude!abcdef012345");
    }
    // Copy
    {
      std::string txt {"abcdef012345"};
      std::string pre {"Yo dude!"};
      TS_ASSERT_EQUALS(StringUtil::prependc(pre, txt),
                       "Yo dude!abcdef012345");
    }
  }


  void testPrependInPlace() {
    // In place
    {
      std::string txt {"abcdef012345"};
      std::string pre {"Yo dude!"};
      StringUtil::prepend(pre, txt);
      TS_ASSERT_EQUALS(txt, "Yo dude!abcdef012345");
    }
    // Copy
    {
      std::string txt {"abcdef012345"};
      std::string pre {"Yo dude!"};
      TS_ASSERT_EQUALS(StringUtil::prependc(pre, txt),
                       "Yo dude!abcdef012345");
    }
  }




  void testProcessIfEndifBasic() {
    const std::string text = R"(This is my line
there are many like it
but this one is mine
)";
    TS_ASSERT_EQUALS(StringUtil::processIfEndif(text, "foo"),
                     text);
    TS_ASSERT_EQUALS(StringUtil::processIfEndif(text, ""),
                     text);
  }


  void testProcessIfEndifKeyword()  {
    const std::string text = R"(This is my line
#if foo
there are many like it
#endif
but this one is mine
)";

    const std::string textMatch = R"(This is my line
there are many like it
but this one is mine
)";

    const std::string textNoMatch = R"(This is my line
but this one is mine
)";

    TS_ASSERT_EQUALS(StringUtil::processIfEndif(text, "foo"),
                     textMatch);
    TS_ASSERT_EQUALS(StringUtil::processIfEndif(text, "bar"),
                     textNoMatch);
    TS_ASSERT_EQUALS(StringUtil::processIfEndif(text, ""),
                     textNoMatch);
  }



  void testProcessIfEndifAdvanced()  {
    const std::string text = R"(This is my line
#if foo
there are many like it
#endif
but this one is mine

I got a big tooth
in the sky

#if bar
fishing for fire
on the coast of moon
#else
journey that tire
are we there soon?
#endif

#if foo
so smooth
#endif

#if N
MIIIK
#else
take it, to try
#endif
)";


    const std::string textFoo = R"(This is my line
there are many like it
but this one is mine

I got a big tooth
in the sky

journey that tire
are we there soon?

so smooth

take it, to try
)";

    const std::string textBar = R"(This is my line
but this one is mine

I got a big tooth
in the sky

fishing for fire
on the coast of moon


take it, to try
)";

    const std::string textNoMatch = R"(This is my line
but this one is mine

I got a big tooth
in the sky

journey that tire
are we there soon?


take it, to try
)";

    TS_ASSERT_EQUALS(StringUtil::processIfEndif(text, "foo"),
                     textFoo);
    TS_ASSERT_EQUALS(StringUtil::processIfEndif(text, "bar"),
                     textBar);
    TS_ASSERT_EQUALS(StringUtil::processIfEndif(text, ""),
                     textNoMatch);
  }



 private:
};

#endif  // UTIL_TESTSTRINGUTIL_H
