#ifndef UTIL_TESTWORDGENERATOR_H
#define UTIL_TESTWORDGENERATOR_H

#include <util/WordGenerator.h>

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


  void testBasicInput() {
    WordGenerator<4> wg;
    wg.addInputWords({"fantastic"});
    wg.prepare();
    std::string word = wg.generate();
    TS_ASSERT_EQUALS(word, "fantastic");
  }

  void testBasicInput2() {
    WordGenerator<3> wg;
    wg.addInputWords({"fantastic"});
    wg.prepare();
    std::string word = wg.generate();
    TS_ASSERT_EQUALS(word, "fantastic");
  }

  void testBasicInput3() {
    WordGenerator<3> wg;
    wg.addInputWords({"croatia"});
    wg.prepare();

    std::string word = wg.generate();
    TS_ASSERT_EQUALS(word, "croatia");
  }

  void testBasicInput4() {
    WordGenerator<4> wg;
    wg.addInputWords({"croatia"});
    wg.prepare();
    std::string word = wg.generate();
    TS_ASSERT_EQUALS(word, "croatia");
  }

  void testBasicInput5() {
    WordGenerator<4> wg;
    wg.addInputWords({"fantasti"});
    wg.prepare();
    std::string word = wg.generate();
    TS_ASSERT_EQUALS(word, "fantasti");
  }

  // TEST FOR <4> CATON, C -> ATON -> #
  void testBasicInput6() {
    WordGenerator<4> wg;
    wg.addInputWords({"caton"});
    wg.prepare();
    std::string word = wg.generate();
    TS_ASSERT_EQUALS(word, "caton");
  }

  // ADD TEST FOR <4> CATONI,  C -> ATON -> TONI -> #
  void testBasicInput7() {
    WordGenerator<4> wg;
    wg.addInputWords({"catoni"});
    wg.prepare();
    std::string word = wg.generate();
    TS_ASSERT_EQUALS(word, "catoni");
  }

  // ADD TEST FOR <4> CATO,  C -> ATO_ -> #
  void testShortWord() {
    WordGenerator<4> wg;
    wg.addInputWords({"cato"});
    wg.prepare();
    std::string word = wg.generate();
    TS_ASSERT_EQUALS(word, "cato");
  }

  void testShortWord2() {
    WordGenerator<4> wg;
    wg.addInputWords({"cat"});
    wg.prepare();
    std::string word = wg.generate();
    TS_ASSERT_EQUALS(word, "cat");
  }

  // FRIENDLY
  void testMediumInput1() {
    WordGenerator<2> wg;
    wg.addInputWords({"okeskena"});
    wg.prepare();

    // o -> ke -> es -> sk -> ke -> en -> na -> END
    // INIT: o -> ke
    // KEYS:
    // ke -> es, en
    // es -> sk
    // sk -> ke
    // en -> na
    // na -> END

    TS_ASSERT_EQUALS(wg.markov_.initList_.size(), 1u);
    TS_ASSERT_EQUALS(wg.markov_.keyNodes_.size(), 5u);

    for (size_t i = 0 ; i < 50 ; ++i) {
      std::string word = wg.generate();
      if (word.size() == 5) {
        TS_ASSERT_EQUALS(word, "okena");
      }  else if (word.size() == 8) {
        TS_ASSERT_EQUALS(word, "okeskena");
      } else if (word.size() == 11) {
        TS_ASSERT_EQUALS(word, "okeskeskena");
      }
    }
  }


  // FRIENDLY
  void testSmallerInitKey() {
    // KEYS: c -> ata -> tar -> ara -> rat -> ate -> tes -> #
    {  // N_INIT = N
      WordGenerator<3> wg;
      wg.addInputWords({"catarates"});
      wg.prepare();
      TS_ASSERT_EQUALS(wg.markov_.initList_.size(), 1u);
    }

    {  // N_INIT = 2 < N
      WordGenerator<3> wg(2);
      wg.addInputWords({"catarates"});
      wg.prepare();
      // When preparing the initial key table
      // it now should extpand 'at', to all matches, which is 'ata', and 'ate'

      TS_ASSERT_EQUALS(wg.markov_.initList_.size(), 2u);
      TS_ASSERT_EQUALS(wg.markov_.initList_[0].first, 'c');
      if ( wg.markov_.initList_.size() > 1 ) {
        TS_ASSERT_EQUALS(wg.markov_.initList_[1].first, 'c');
      }
    }
  }


  // FRIENDLY
  void testSmallerInitKey2() {
    WordGenerator<3> wg(2);
    wg.addInputWords(
        StringUtil::trimSplit("dink dino gnasher laska sandy snitter",
                              ' '));
    wg.prepare();
    TS_ASSERT_EQUALS(wg.markov_.keyNodes_.size(), 14u);
    TS_ASSERT_EQUALS(wg.markov_.initList_.size(), 7u);
  }


  void testIsInputWord() {
    WordGenerator<3> wg;
    wg.addInputWords(
        StringUtil::trimSplit("CAT FISH potatoe", ' '));

    TS_ASSERT(wg.isInputWord("CAT"));
    TS_ASSERT(wg.isInputWord("FISH"));
    TS_ASSERT(wg.isInputWord("  potatoe  "));

    TS_ASSERT(!wg.isInputWord("fishh"));
    TS_ASSERT(!wg.isInputWord("potatoo"));
  }


  void testGenerate() {
    // Just adding a single word.
    WordGenerator<3> wg;
    wg.addInputWords({"fantastic"});
    wg.prepare();
    std::string word = wg.generate();
    TS_ASSERT_EQUALS(word, "fantastic");
  }


  // FRIENDLY
  void testAddingInputWords()
  {
    {
      WordGenerator<3> wg;
      wg.addInputWords({"This is a", "list of words"});
      TS_ASSERT(wg.isInputWord("list of words"));
      TS_ASSERT(wg.isInputWord("this is a"));
    }

    {
      WordGenerator<3> wg;
      wg.addInputWords(
          StringUtil::trimSplit("CAT FISH potatoe", ' '));
      TS_ASSERT(wg.isInputWord("cat"));
      TS_ASSERT(wg.isInputWord("fish"));
      TS_ASSERT(wg.isInputWord("potatoe"));

      // OX ignored for being too short.addInputWords
      wg.addInputWords(
          StringUtil::trimSplit("OX HEN", ' '));
      TS_ASSERT(wg.isInputWord("cat"));
      TS_ASSERT(wg.isInputWord("fish"));
      TS_ASSERT(wg.isInputWord("hen"));
      TS_ASSERT(wg.isInputWord("potatoe"));
    }
  }

  // FRIENDLY
  void testAddingToShortInputWords() {
    WordGenerator<3> wg;
    // Words that have length < 3 should be ignored.

    wg.addInputWords(
        StringUtil::trimSplit("CAT FISH POTATOE OX HEN", ' '));
    TS_ASSERT(wg.isInputWord("cat"));
    TS_ASSERT(wg.isInputWord("fish"));
    TS_ASSERT(wg.isInputWord("hen"));
    TS_ASSERT(wg.isInputWord("potatoe"));
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

  // Friendly
  void testConstructor()
  {
    WordGenerator<4> wg2;
    TS_ASSERT_EQUALS(wg2.markov_.N_init_, 4);

    WordGenerator<2> wg2b(3);
    TS_ASSERT_EQUALS(wg2b.markov_.N_init_, 2);

    WordGenerator<4> wg3(2);
    TS_ASSERT_EQUALS(wg3.markov_.N_init_, 2);

    WordGenerator<1337> wg4(0);
    TS_ASSERT_EQUALS(wg4.markov_.N_init_, 1337);

    WordGenerator<1337> wg5(5);
    TS_ASSERT_EQUALS(wg5.markov_.N_init_, 5);
  }

};

#endif  // UTIL_TESTWORDGENERATOR_H
