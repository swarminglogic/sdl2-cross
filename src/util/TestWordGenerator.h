#ifndef UTIL_TESTWORDGENERATOR_H
#define UTIL_TESTWORDGENERATOR_H

#include <util/WordGenerator.h>
#include <util/WordGeneratorInterface.h>

#include <cxxtest/TestSuite.h>


/**
 * Test for the WordGenerator class.
 *
 * @author SwarmingLogic
 */
class TestWordGenerator : public CxxTest::TestSuite
{
 public:

  void testNoInputs() {
    WordGenerator<2> wg2;
    wg2.prepare();
    WordGenerator<5> wg5;
    wg5.prepare();
  }

  void testIsInputWord() {
    WordGenerator<3> wg;
    wg.addInputWords("CAT FISH potatoe", ' ');

    TS_ASSERT(wg.isInputWord("cat"));
    TS_ASSERT(wg.isInputWord("CAT"));
    TS_ASSERT(wg.isInputWord("cAt"));
    TS_ASSERT(wg.isInputWord("fish"));
    TS_ASSERT(wg.isInputWord("   potatoe   "));

    TS_ASSERT(!wg.isInputWord("fishh"));
    TS_ASSERT(!wg.isInputWord("potatoo"));
  }


  void testGenerate() {
    // Just adding a single word.
    WordGenerator<3> wg;
    wg.addInputWords("fantastic", ' ');
    wg.prepare();
    std::string word = wg.generate();
    TS_ASSERT_EQUALS(word, "fantastic");
  }

  // FRIENDLY TESTS
  void testAddingInputWords()
  {
    {
      WordGenerator<3> wg;
      wg.addInputWords(R"(This
  is
a
list
of
words)");
      auto lst = wg.getInputWords();
      TS_ASSERT(true);
      TS_ASSERT_EQUALS(lst.size(), 3u);
    }
    {
      WordGenerator<2> wg;
      wg.addInputWords("This is a list of words", ' ');
      auto lst = wg.getInputWords();
      TS_ASSERT(true);
      TS_ASSERT_EQUALS(lst.size(), 5u);
    }

    {
      WordGenerator<3> wg;
      wg.addInputWords("This is a\n"
                       "list of words");
      auto lst = wg.getInputWords();
      TS_ASSERT_EQUALS(lst.size(), 2u);
      TS_ASSERT_EQUALS(lst[0], "list of words");
      TS_ASSERT_EQUALS(lst[1], "this is a");
    }

    {
      WordGenerator<3> wg;
      wg.addInputWords("CAT FISH potatoe", ' ');
      auto lst = wg.getInputWords();
      TS_ASSERT_EQUALS(lst.size(), 3u);
      TS_ASSERT_EQUALS(lst[0], "cat");
      TS_ASSERT_EQUALS(lst[1], "fish");
      TS_ASSERT_EQUALS(lst[2], "potatoe");

      wg.addInputWords("OX HEN", ' ');
      lst = wg.getInputWords();
      TS_ASSERT_EQUALS(lst.size(), 4u);
      // (sorted alphabetically)
      TS_ASSERT_EQUALS(lst[0], "cat");
      TS_ASSERT_EQUALS(lst[1], "fish");
      TS_ASSERT_EQUALS(lst[2], "hen");
      TS_ASSERT_EQUALS(lst[3], "potatoe");
    }
  }

  void testAddingToShortInputWords() {
    WordGenerator<3> wg;
    // Words that have length < 3 should be ignored.

    wg.addInputWords("CAT FISH POTATOE OX HEN", ' ');
    auto lst = wg.getInputWords();
    TS_ASSERT_EQUALS(lst.size(), 4u);
    TS_ASSERT_EQUALS(lst[0], "cat");
    TS_ASSERT_EQUALS(lst[1], "fish");
    TS_ASSERT_EQUALS(lst[2], "hen");
    TS_ASSERT_EQUALS(lst[3], "potatoe");
  }


  void testIsSubkey() {
    std::array<char, 4> key { 'a', 'b', 'c', 'd'};

    { std::array<char, 4> sub = key;
      TS_ASSERT(WordGenerator<4>::isSubkey(sub, key)); }

    { std::array<char, 4> sub = { 'a', 'b', 'c', ' '};
      TS_ASSERT(WordGenerator<4>::isSubkey(sub, key)); }

    { std::array<char, 4> sub = { 'a', 'b', ' ', ' '};
      TS_ASSERT(WordGenerator<4>::isSubkey(sub, key)); }

    { std::array<char, 4> sub = { 'a', ' ', ' ', ' '};
      TS_ASSERT(WordGenerator<4>::isSubkey(sub, key)); }

    { std::array<char, 4> sub = { ' ', ' ', ' ', ' '};
      TS_ASSERT(WordGenerator<4>::isSubkey(sub, key)); }

    { std::array<char, 4> sub = { 'b', 'b', 'c', 'd'};
      TS_ASSERT(!WordGenerator<4>::isSubkey(sub, key)); }

    { std::array<char, 4> sub;
      sub.fill(' ');
      sub[0] = 'a';
      TS_ASSERT(WordGenerator<4>::isSubkey(sub, key)); }
  }


  void testComparisonAssumption() {
    {
      std::array<char, 4> key;
      key.fill('a');
      std::array<char, 4> ckey = key;
      TS_ASSERT_EQUALS(key, ckey);
    }
    {
      std::array<char, 3> key  { 'a', ' ', ' '};
      std::array<char, 3> key2 { 'a', 's', 'h'};
      TS_ASSERT(key < key2);
      TS_ASSERT(!(key > key2));
    }
    {
      std::array<char, 3> key  { 'h', 'e', 'r'};
      std::array<char, 3> pf  { 'a', ' ', ' '};
      TS_ASSERT(key > pf);
    }
  }

  void testConstructor()
  {
    WordGenerator<4> wg2;
    TS_ASSERT_EQUALS(wg2.getN(), 4);
    TS_ASSERT_EQUALS(wg2.getN_init(), 4);

    WordGenerator<2> wg2b(3);
    TS_ASSERT_EQUALS(wg2b.getN(), 2);
    TS_ASSERT_EQUALS(wg2b.getN_init(), 2);

    WordGenerator<4> wg3(2);
    TS_ASSERT_EQUALS(wg3.getN(), 4);
    TS_ASSERT_EQUALS(wg3.getN_init(), 2);

    WordGenerator<WordGeneratorInterface::MAX_ORDER> wg4(0);
    TS_ASSERT_EQUALS(wg4.getN(), WordGeneratorInterface::MAX_ORDER);
    TS_ASSERT_EQUALS(wg4.getN_init(), WordGeneratorInterface::MAX_ORDER);

    WordGenerator<WordGeneratorInterface::MAX_ORDER> wg5(5);
    TS_ASSERT_EQUALS(wg5.getN(), WordGeneratorInterface::MAX_ORDER);
    TS_ASSERT_EQUALS(wg5.getN_init(), 5);
  }
};

#endif  // UTIL_TESTWORDGENERATOR_H
