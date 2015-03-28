#ifndef UTIL_TESTWORDGENERATOR_H
#define UTIL_TESTWORDGENERATOR_H

#include <util/StringUtil.h>
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


  // FRIENDLY
  void testCreateNGram() {
    const std::string input = "Quickfox";
    auto v2 = WordGenerator<2>::createNGram(input, 0);
    std::array<char, 2> exp2 {'Q', 'u'};
    TS_ASSERT_EQUALS(v2, exp2);
    TS_ASSERT_EQUALS(v2.size(), 2u);

    auto v3 = WordGenerator<3>::createNGram(input, 1);
    TS_ASSERT_EQUALS(v3.size(), 3u);
    std::array<char, 3> exp3 {'u', 'i', 'c'};
    TS_ASSERT_EQUALS(v3, exp3);

    auto v4 = WordGenerator<4>::createNGram(input, 4);
    TS_ASSERT_EQUALS(v4.size(), 4u);
    std::array<char, 4> exp4 {'k', 'f', 'o', 'x'};
    TS_ASSERT_EQUALS(v4, exp4);

    auto v5 = WordGenerator<4>::createNGram(input, 5);
    TS_ASSERT_EQUALS(v5.size(), 4u);
    std::array<char, 4> exp5 {'f', 'o', 'x', ' '};
    TS_ASSERT_EQUALS(v5, exp5);

    auto v6 = WordGenerator<4>::createNGram(input, 6);
    TS_ASSERT_EQUALS(v6.size(), 4u);
    std::array<char, 4> exp6 {'o', 'x', ' ', ' '};
    TS_ASSERT_EQUALS(v6, exp6);
  }

  // FRIENDLY
  template <int N>
  void linearKeyTesting(const WordGenerator<N>& wg,
                        char first,
                        size_t nNodes,
                        const std::vector<std::string>& keys) {
    TS_ASSERT_EQUALS(wg.initList_.size(), 1u);
    TS_ASSERT_EQUALS(wg.keyNodes_.size(), nNodes);
    TS_ASSERT_EQUALS(wg.initList_[0].first, first);
    TS_ASSERT_EQUALS(wg.initList_[0].second->key,
                     WordGenerator<N>::createNGram(keys[0], 0));
    auto currentChildren = wg.initList_[0].second->children;
    for (size_t i = 1 ; i < keys.size() ; ++i) {
      TS_ASSERT_EQUALS(currentChildren.size(), 1u);
      TS_ASSERT_EQUALS(currentChildren[0]->key,
                       WordGenerator<N>::createNGram(keys[i], 0));
      currentChildren = currentChildren[0]->children;
    }
    TS_ASSERT_EQUALS(currentChildren[0], &WordGenerator<N>::END_NODE);
  }

  // FRIENDLY
  void testBasicInput() {
    WordGenerator<4> wg;
    wg.addInputWords({"fantastic"});
    wg.prepare();

    linearKeyTesting(wg, 'f', 5u,
                     {"anta", "ntas", "tast", "asti", "stic"});
    std::string word = wg.generate();
    TS_ASSERT_EQUALS(word, "fantastic");
  }

  // FRIENDLY
  void testBasicInput2() {
    WordGenerator<3> wg;
    wg.addInputWords({"fantastic"});
    wg.prepare();

    linearKeyTesting(wg, 'f', 6u,
                     {"ant", "nta", "tas", "ast", "sti", "tic"});
    std::string word = wg.generate();
    TS_ASSERT_EQUALS(word, "fantastic");
  }

  // FRIENDLY
  void testBasicInput3() {
    WordGenerator<3> wg;
    wg.addInputWords({"croatia"});
    wg.prepare();

    // c -> roa -> oat -> ati -> tia -> END
    linearKeyTesting(wg, 'c', 4,
                     {"roa", "oat", "ati", "tia"});
    std::string word = wg.generate();
    TS_ASSERT_EQUALS(word, "croatia");
  }

  // FRIENDLY
  void testBasicInput4() {
    WordGenerator<4> wg;
    wg.addInputWords({"croatia"});
    wg.prepare();

    // c -> roa -> oat -> ati -> tia -> END
    linearKeyTesting(wg, 'c', 3,
                     {"roat", "oati", "atia"});

    std::string word = wg.generate();
    TS_ASSERT_EQUALS(word, "croatia");
  }

  // FRIENDLY
  void testBasicInput5() {
    WordGenerator<4> wg;
    wg.addInputWords({"fantasti"});
    wg.prepare();

    // f -> anta -> ntas -> tast -> asti -> END
    linearKeyTesting(wg, 'f', 4,
                     {"anta", "ntas", "tast", "asti"});

    std::string word = wg.generate();
    TS_ASSERT_EQUALS(word, "fantasti");
  }

  // FRIENDLY
  // TEST FOR <4> CATON, C -> ATON -> #
  void testBasicInput6() {
    WordGenerator<4> wg;
    wg.addInputWords({"caton"});
    wg.prepare();
    linearKeyTesting(wg, 'c', 1, {"aton"});
    std::string word = wg.generate();
    TS_ASSERT_EQUALS(word, "caton");
  }

  // FRIENDLY
  // ADD TEST FOR <4> CATONI,  C -> ATON -> TONI -> #
  void testBasicInput7() {
    WordGenerator<4> wg;
    wg.addInputWords({"catoni"});
    wg.prepare();
    linearKeyTesting(wg, 'c', 2, {"aton", "toni"});

    std::string word = wg.generate();
    TS_ASSERT_EQUALS(word, "catoni");
  }

  // FRIENDLY
  // ADD TEST FOR <4> CATO,  C -> ATO_ -> #
  void testShortWord() {
    WordGenerator<4> wg;
    wg.addInputWords({"cato"});
    wg.prepare();
    linearKeyTesting(wg, 'c', 1, {"ato "});

    std::string word = wg.generate();
    TS_ASSERT_EQUALS(word, "cato");
  }

  // FRIENDLY
  void testShortWord2() {
    WordGenerator<4> wg;
    wg.addInputWords({"cat"});
    wg.prepare();
    linearKeyTesting(wg, 'c', 1, {"at  "});

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

    TS_ASSERT_EQUALS(wg.initList_.size(), 1u);
    TS_ASSERT_EQUALS(wg.keyNodes_.size(), 5u);
    TS_ASSERT_EQUALS(wg.getEntry("ke", 0).children.size(), 2);
    TS_ASSERT_EQUALS(wg.getEntry("es", 0).children.size(), 1);
    TS_ASSERT_EQUALS(wg.getEntry("es", 0).children[0]->key,
                     WordGenerator<2>::createNGram("sk", 0));
    TS_ASSERT_EQUALS(wg.getEntry("sk", 0).children[0]->key,
                     WordGenerator<2>::createNGram("ke", 0));
    TS_ASSERT_EQUALS(wg.getEntry("en", 0).children[0]->key,
                     WordGenerator<2>::createNGram("na", 0));
    TS_ASSERT_EQUALS(wg.getEntry("na", 0).children[0],
                     &WordGenerator<2>::END_NODE);

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
      linearKeyTesting(wg, 'c', 6,
                       {"ata", "tar", "ara", "rat", "ate", "tes"});
      // N_init = N, so it should only have one entry
      TS_ASSERT_EQUALS(wg.initList_.size(), 1u);
    }

    {  // N_INIT = 2 < N
    WordGenerator<3> wg(2);
    wg.addInputWords({"catarates"});
    wg.prepare();
    // When preparing the initial key table
    // it now should extpand 'at', to all matches, which is 'ata', and 'ate'
    TS_ASSERT_EQUALS(wg.initList_.size(), 2u);
    TS_ASSERT_EQUALS(wg.initList_[0].first, 'c');
    TS_ASSERT_EQUALS(wg.initList_[0].second->key,
                     WordGenerator<3>::createNGram("ata", 0));
    if ( wg.initList_.size() > 1 ) {
      TS_ASSERT_EQUALS(wg.initList_[1].first, 'c');
      TS_ASSERT_EQUALS(wg.initList_[1].second->key,
                       WordGenerator<3>::createNGram("ate", 0));
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

    // N = 3 keys
    // ------------
    // | AND | Y, |
    // | ASH | E, |
    // | ASK | A, |
    // | HER | #, |
    // | INK | #, |
    // | INO | #, |
    // | ITT | E, |
    // | NAS | H, |
    // | NDY | #, |
    // | NIT | T, |
    // | SHE | R, |
    // | SKA | #, |
    // | TER | #, |
    // | TTE | R, |

    // You can make this by just traversing through the map
    // and building a vector out of it
    // sort this vector, and you're good to go.

    // Init list, prior to exp, swapped left, and right
    // | INK | D |
    // | INO | D |
    // | NAS | G |
    // | ASK | L |
    // | AND | S |
    // | NIT | S |


    // DESIRED RESULT:
    // | D | INK |
    // | D | INO |
    // | G | NAS |
    // | L | ASH | <- NEW!
    // | L | ASK |
    // | S | AND |
    // | S | NIT |
    TS_ASSERT_EQUALS(wg.keyNodes_.size(), 14u);
    TS_ASSERT_EQUALS(wg.initList_.size(), 7u);
  }


  void testIsInputWord() {
    WordGenerator<3> wg;
    wg.addInputWords(
        StringUtil::trimSplit("CAT FISH potatoe", ' '));

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
      wg.addInputWords({"This",
              "  is",
              "a",
              "list",
              "of",
              "words"});
      auto lst = wg.getInputWords();
      TS_ASSERT(true);
      TS_ASSERT_EQUALS(lst.size(), 3u);
    }
    {
      WordGenerator<2> wg;
      wg.addInputWords(
          StringUtil::trimSplit("This is a list of words", ' '));
      auto lst = wg.getInputWords();
      TS_ASSERT(true);
      TS_ASSERT_EQUALS(lst.size(), 3u);
    }

    {
      WordGenerator<3> wg;
      wg.addInputWords({"This is a", "list of words"});
      auto lst = wg.getInputWords();
      TS_ASSERT_EQUALS(lst.size(), 2u);
      TS_ASSERT(wg.isInputWord("list of words"));
      TS_ASSERT(wg.isInputWord("this is a"));
    }

    {
      WordGenerator<3> wg;
      wg.addInputWords(
          StringUtil::trimSplit("CAT FISH potatoe", ' '));
      auto lst = wg.getInputWords();
      TS_ASSERT_EQUALS(lst.size(), 3u);
      TS_ASSERT(wg.isInputWord("cat"));
      TS_ASSERT(wg.isInputWord("fish"));
      TS_ASSERT(wg.isInputWord("potatoe"));
      TS_ASSERT_EQUALS(lst.size(), 3u);

      // OX ignored for being too short.addInputWords
      wg.addInputWords(
          StringUtil::trimSplit("OX HEN", ' '));
      TS_ASSERT_EQUALS(wg.getInputWords().size(), 4u);
      TS_ASSERT(wg.isInputWord("cat"));
      TS_ASSERT(wg.isInputWord("fish"));
      TS_ASSERT(wg.isInputWord("hen"));
      TS_ASSERT(wg.isInputWord("potatoe"));
    }
  }

  void testAddingToShortInputWords() {
    WordGenerator<3> wg;
    // Words that have length < 3 should be ignored.

    wg.addInputWords(
        StringUtil::trimSplit("CAT FISH POTATOE OX HEN", ' '));
    auto lst = wg.getInputWords();
    TS_ASSERT_EQUALS(lst.size(), 4u);
    TS_ASSERT(wg.isInputWord("cat"));
    TS_ASSERT(wg.isInputWord("fish"));
    TS_ASSERT(wg.isInputWord("hen"));
    TS_ASSERT(wg.isInputWord("potatoe"));
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

  // Friendly
  void testConstructor()
  {
    WordGenerator<4> wg2;
    TS_ASSERT_EQUALS(wg2.N_init_, 4);

    WordGenerator<2> wg2b(3);
    TS_ASSERT_EQUALS(wg2b.N_init_, 2);

    WordGenerator<4> wg3(2);
    TS_ASSERT_EQUALS(wg3.N_init_, 2);

    WordGenerator<WordGeneratorInterface::MAX_ORDER> wg4(0);
    TS_ASSERT_EQUALS(wg4.N_init_, WordGeneratorInterface::MAX_ORDER);

    WordGenerator<WordGeneratorInterface::MAX_ORDER> wg5(5);
    TS_ASSERT_EQUALS(wg5.N_init_, 5);
  }
};

#endif  // UTIL_TESTWORDGENERATOR_H
