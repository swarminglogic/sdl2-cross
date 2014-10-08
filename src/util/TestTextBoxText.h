#ifndef UTIL_TESTTEXTBOXTEXT_H
#define UTIL_TESTTEXTBOXTEXT_H

#include <util/TextBoxText.h>

#include <cxxtest/TestSuite.h>


/**
 * Test for the TextBoxText class.
 *
 * @author SwarmingLogic (Roald Fernandez)
 */
class TestTextBoxText : public CxxTest::TestSuite
{
 public:
  void testBasic()
  {
    TextBoxText tbt;
    std::string txt {"Hello world!"};
    tbt.setText(txt);
    std::vector<std::string> formatted = tbt.getFormatted();
    TS_ASSERT(!formatted.empty());
    TS_ASSERT_EQUALS(tbt.getFormatted()[0], txt);
  }

  void testManualLinebreak()
  {
    TextBoxText tbt;
    std::string txt {"Hello my \nsweet world!"};
    tbt.setText(txt);
    std::vector<std::string> formatted = tbt.getFormatted();
    TS_ASSERT(!formatted.empty());
    TS_ASSERT_EQUALS(formatted.size(), 2);
    // It should strip away trailing space.
    TS_ASSERT_EQUALS(tbt.getFormatted()[0], "Hello my");
    TS_ASSERT_EQUALS(tbt.getFormatted()[1], "sweet world!");

    // but not leading spaces, as they are most likely intentional.
    tbt.setText("This \n  is spacey");
    TS_ASSERT(!tbt.getFormatted().empty());
    TS_ASSERT_EQUALS(tbt.getFormatted().size(), 2);
    TS_ASSERT_EQUALS(tbt.getFormatted()[0], "This");
    TS_ASSERT_EQUALS(tbt.getFormatted()[1], "  is spacey");
  }

  void testGetFormattedHeight()
  {
    TextBoxText tbt;
    tbt.setHeight(13);
    tbt.setText("Hello you");
    TS_ASSERT_EQUALS(tbt.getFormatted().size(), 1);
    TS_ASSERT_EQUALS(tbt.getFormattedHeight(), 13);

    tbt.setText("Hello\nyou");
    TS_ASSERT_EQUALS(tbt.getFormatted().size(), 2);
    TS_ASSERT_EQUALS(tbt.getFormattedHeight(), 26);
  }

  void testAutoFormatting()
  {
    TextBoxText tbt;
    tbt.setWidthFixed(10);
    tbt.setBoxWidth(100);
    // 10 characters pr line.
    //     My pony has a pink tail and like to yawn.
    //    0123456789
    // 1: My pony   |
    // 2: has a pink|
    // 3: tail and  |
    // 4: likes to  |
    // 5: yawn.     |
    tbt.setText("My pony has a pink tail and likes to yawn.");
    std::vector<std::string> formatted = tbt.getFormatted();
    TS_ASSERT_EQUALS(formatted.size(), 5);
    TS_ASSERT_EQUALS(formatted[0], "My pony");
    TS_ASSERT_EQUALS(formatted[1], "has a pink");
    TS_ASSERT_EQUALS(formatted[2], "tail and");
    TS_ASSERT_EQUALS(formatted[3], "likes to");
    TS_ASSERT_EQUALS(formatted[4], "yawn.");
  }

  void testAutoAndManualFormatting()
  {
    TextBoxText tbt;
    tbt.setWidthFixed(10);
    tbt.setBoxWidth(100);
    // 10 characters pr line.
    //     My pony \nhas a pink tail\nand like to yawn.
    //    0123456789
    // 1: My pony   | Same break as before, should be no different.
    // 2: has a pink|
    // 3: tail      | Manual break!
    // 4:  and likes| Note starting space.
    // 5: to yawn.  |
    tbt.setText("My pony \nhas a pink tail\n and likes to yawn.");
    std::vector<std::string> formatted = tbt.getFormatted();
    TS_ASSERT_EQUALS(formatted.size(), 5);
    TS_ASSERT_EQUALS(formatted[0], "My pony");
    TS_ASSERT_EQUALS(formatted[1], "has a pink");
    TS_ASSERT_EQUALS(formatted[2], "tail");
    TS_ASSERT_EQUALS(formatted[3], " and likes");
    TS_ASSERT_EQUALS(formatted[4], "to yawn.");
  }

  void testPartial()
  {
    TextBoxText tbt;
    tbt.setWidthFixed(10);
    tbt.setBoxWidth(100);
    tbt.setText("My pony \nhas a pink tail\nand likes to yawn.");
    // Test full
    std::vector<std::string> formatted = tbt.getFormatted(1.0f);
    TS_ASSERT_EQUALS(formatted.size(), 5);
    TS_ASSERT_EQUALS(formatted[0], "My pony");
    TS_ASSERT_EQUALS(formatted[1], "has a pink");
    TS_ASSERT_EQUALS(formatted[2], "tail");
    TS_ASSERT_EQUALS(formatted[3], "and likes");
    TS_ASSERT_EQUALS(formatted[4], "to yawn.");

    // [My pony][has a pink][tail][ and likes][to yawn.]
    // ->
    // My ponyhas a pinktail and likesto yawn. 39 characters.
    // 0.0 ->  0 -> []
    formatted = tbt.getFormatted(0.0f);
    TS_ASSERT_EQUALS(formatted.size(), 1);
    TS_ASSERT_EQUALS(formatted[0], "");

    // 0.5 -> 19 -> [My pony][has a pink][ta]
    formatted = tbt.getFormatted(0.5f);
    TS_ASSERT_EQUALS(formatted.size(), 3);
    TS_ASSERT_EQUALS(formatted[0], "My pony");
    TS_ASSERT_EQUALS(formatted[1], "has a pink");
    TS_ASSERT_EQUALS(formatted[2], "ta");

    // 0.17949 -> 7 -> [My pony]
    formatted = tbt.getFormatted(0.17949f);
    TS_ASSERT_EQUALS(formatted.size(), 1);
    TS_ASSERT_EQUALS(formatted[0], "My pony");

    // 0.99999 -> ?? -> [My pony]...[to yawn.]
    formatted = tbt.getFormatted(0.99999f);
    TS_ASSERT_EQUALS(formatted.size(), 5);
    TS_ASSERT_EQUALS(formatted[4], "to yawn.");
  }

  void testCopy()
  {
    TextBoxText tbt;
    tbt.setWidthFixed(10);
    tbt.setBoxWidth(100);
    tbt.setText("My pony \nhas a pink tail\nand likes to yawn.");

    TextBoxText tbtCopy(tbt);
    std::vector<std::string> formatted = tbt.getFormatted(0.5f);
    TS_ASSERT_EQUALS(formatted.size(), 3);
    TS_ASSERT_EQUALS(formatted[0], "My pony");
    TS_ASSERT_EQUALS(formatted[1], "has a pink");
    TS_ASSERT_EQUALS(formatted[2], "ta");
  }

 private:
};

#endif  // UTIL_TESTTEXTBOXTEXT_H
