#ifndef UTIL_WORDGENERATOR_H
#define UTIL_WORDGENERATOR_H

#include <array>
#include <map>
#include <string>
#include <utility>
#include <vector>

#include <util/UnitTestUtil.h>
#include <util/WordGeneratorInterface.h>


/**
 * WordGenerator class.
 *
 * Markov Chain Random Word Generation tool.
 *
 * Does not words containing spaces
 *
 * Takes a list of words as input, creates a likelihood table with
 * "what is the prability that the letter is X, given that previous N letters
 * were [n_{i-1}, n_{i-2}, ..., n_{i-N}]
 *
 * N is the depth (aka. order or level) of the markov chain word generator
 * The nodes are character N-gram.
 *
 * To allow more variability, a shallower depth can be used for the
 * initalization. This is called N_init, and follows 1 <= N_init <= N
 *
 * Valid N in range [1, .., WordGeneratorInterface::MAX_ORDER]
 *
 * @author SwarmingLogic
 */
template<int N>
class WordGenerator : public WordGeneratorInterface
{
  FRIENDLY_UNIT_TEST(WordGenerator)
  FRIENDLY_UNIT_TEST(WordGeneratorFactory)
 public:
  /**
   * Template N can only be in range [1, MAX_ORDER], and will not link
   * otherwise (e.g. a form of static assertion).
   *
   * If N < N_init || N_init <= 0
   *   then N_init := N
   */
  explicit WordGenerator(int N_init = 0);
  virtual ~WordGenerator() {}

  /**
   * Add words as initializer list.
   */
  void addInputWords(const std::string& text, char dlim = '\n') override;
  void addInputWords(const std::vector<std::string>& words) override;

  /**
   * Calling this function creates internal tables that are
   * necessary for when generating words
   */
  void prepare() override;

  /**
   * Generates a word. It has no guarantee of being unique
   */
  std::string generate() const override;

  /**
   * Checks if the word parameter already exists as a input word.
   */
  bool isInputWord(const std::string& word) const override;

  /**
   * Properties of WordGenerator
   */
  int getN() const override;
  int getN_init() const override;

  // Disable copy
  WordGenerator(const WordGenerator& c) = delete;
  WordGenerator& operator=(const WordGenerator& c) = delete;


 private:
  using ListKey = std::array<char, N>;
  using ListValue = std::vector<char>;

  void prepareLookupTable();
  void prepareInitKeyList();
  void insertWordIntoLookupTable(const std::string& word);
  ListValue& getEntry(const ListKey& key);
  static bool isSubkey(const ListKey& sub, const ListKey& key);
  const std::vector<std::string>& getInputWords() const;

  static const char END_VALUE;
  std::vector<std::string> inputWords_;
  std::map<ListKey, ListValue> entries_;
  std::vector<std::pair<char, ListKey>> initList_;
  const int N_init_;
};

template<int N>
const char WordGenerator<N>::END_VALUE = '#';

#endif  // UTIL_WORDGENERATOR_H
