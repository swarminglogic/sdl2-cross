#ifndef UTIL_TESTMARKOVGENERATOR_H
#define UTIL_TESTMARKOVGENERATOR_H

#include <util/MarkovGenerator.h>

#include <cxxtest/TestSuite.h>


/**
 * Test for the MarkovGenerator class.
 *
 * @author SwarmingLogic
 */
class TestMarkovGenerator : public CxxTest::TestSuite
{
 public:
  void testTestHelpFunctions()
  {
    // s2c
    const std::string str = "Hello World!";
    std::vector<char> m = s2c(str);
    TS_ASSERT_EQUALS(m[0], 'H');
    TS_ASSERT_EQUALS(*--m.end(), '!');

    // c2s
    std::string m2 = c2s(m);
    TS_ASSERT_EQUALS(m2, str);
  }


  // FRIENDLY
  void testBasicWordInput() {
    MarkovGenerator<4, char> wg;
    wg.addInputData(ss2cc({"fantastic"}));
    wg.prepare();

    linearKeyTesting(wg, 'f', 5u,
                     ss2cc({"anta", "ntas", "tast", "asti", "stic"}));

    std::string word = c2s(wg.generate());
    TS_ASSERT_EQUALS(word, "fantastic");
  }

  void testBasicSentence() {
    MarkovGenerator<4, std::string> wg;
    using Word = std::string;
    using Sentence = std::vector<Word>;
    using SentenceList = std::vector<Sentence>;

    SentenceList input = {{{"You"}, {"have"}, {"a"}, {"nice"}, {"day"}}};
    TS_ASSERT_EQUALS(input.size(), 1u);
    wg.addInputData(input);
    wg.prepare();

    Sentence output = wg.generate();
    TS_ASSERT_EQUALS(input[0], output);
  }


 private:
  // FRIENDLY
  template <int N, typename T>
  void linearKeyTesting(const MarkovGenerator<N, T>& wg,
                        T first,
                        size_t nNodes,
                        const std::vector<std::vector<T> >& keys) {

    using MarkovType = MarkovGenerator<N, T>;
    TS_ASSERT_EQUALS(wg.initList_.size(), 1u);
    TS_ASSERT_EQUALS(wg.keyNodes_.size(), nNodes);
    TS_ASSERT_EQUALS(wg.initList_[0].first, first);
    TS_ASSERT_EQUALS(wg.initList_[0].second->key,
                     MarkovType::createNGram(keys[0], 0));
    auto currentChildren = wg.initList_[0].second->children;
    for (size_t i = 1 ; i < keys.size() ; ++i) {
      TS_ASSERT_EQUALS(currentChildren.size(), 1u);
      TS_ASSERT_EQUALS(currentChildren[0]->key,
                       MarkovType::createNGram(keys[i], 0));
      currentChildren = currentChildren[0]->children;
    }
    TS_ASSERT_EQUALS(currentChildren[0], &MarkovType::END_NODE);
  }

  std::vector<char> s2c(const std::string& s) {
    return std::vector<char>(s.begin(), s.end());
  }


  std::vector<std::vector<char> > ss2cc(const std::vector<std::string>& s) {
    std::vector<std::vector<char> > ret;
    for (auto& str : s) {
      ret.push_back(std::vector<char>(str.begin(), str.end()));
    }
    return ret;
  }

  std::string c2s(const std::vector<char>& c) {
    return std::string(c.begin(), c.end());
  }
};

#endif  // UTIL_TESTMARKOVGENERATOR_H
