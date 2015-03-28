#ifndef UTIL_WORDGENERATOR_H
#define UTIL_WORDGENERATOR_H

#include <array>
#include <map>
#include <string>
#include <utility>
#include <vector>

#include <boost/functional/hash.hpp>

#include <unordered_map>
#include <unordered_set>
#include <util/UnitTestUtil.h>
#include <util/WordGeneratorInterface.h>


/**
 * WordGenerator class.
 *
 * Markov Chain Random Word Generation tool.
 *
 * Takes a list of words as input, creates a likelihood table with
 * "what is the prability that the letter is X, given that previous N letters
 * were [n_{i-1}, n_{i-2}, ..., n_{i-N}]
 *
 * N is the depth (aka. order or level) of the markov chain word generator
 * The nodes are character N-gram.
 *
 * To allow more variability, a shallower depth can be used for the
 * initalization. This is called N_init.  1 <= N_init <= N
 *
 * Valid N in range [1, .., WordGeneratorInterface::MAX_ORDER]
 *
 * Complexity:
 * Creating the markov chain is O(k lg k), where k is the number of input words.
 * Generating a new word is considered O(1) (the average input
 * word length is considered to be constant).
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
  void addInputWords(const std::vector<std::string>& words) override;

  /**
   * Calling this function creates internal tables that are
   * necessary for when generating words
   */
  void prepare() override;

  /**
   * Generates a word. It has no guarantee of being unique
   * Choses a random starting node, and keeps selecting a random child,
   * (building up the word), until it encounters END_NODE
   */
  std::string generate() const override;

  /**
   * Checks if the word parameter already exists as a input word.
   */
  bool isInputWord(const std::string& word) const override;

  // Disable copy
  WordGenerator(const WordGenerator& c) = delete;
  WordGenerator& operator=(const WordGenerator& c) = delete;


 private:
  // The NGRAM in the markov chain nodes
  using NGramKey = std::array<char, N>;

  // A graph node, representing the markov chain model
  struct KeyNode {
    NGramKey key;
    std::vector<KeyNode*> children;
  };

  /**
   * Processes all input words, and generates the list of NGramKeys
   */
  void prepareLookupTable();

  /**
   * For the case of N_init < N, this function creates the initial starting
   * array (all possible starting points), based on a M-gram, where M < N, which
   * are then expanded to all existing, and matching N-grams.
   */
  void prepareInitKeyList();


  /**
   * A type of factory creation method. It checks to see if a NGram has
   * an existing node. If so, it returns this node, otherwise creates and
   * initializes a new one, before returning it.
   */
  KeyNode& getEntry(const NGramKey& key);
  KeyNode& getEntry(const std::string& word, size_t start);


  /**
   * Helper function to extract N characters from a string,
   * at specified start index, and return this as an NGram.
   */
  static NGramKey createNGram(const std::string& word, size_t start);

  /**
   * Helper function to determine if an NGram is a prefix subkey of another
   * NGram. Spaces are used for the un-set values in the subkey.
   *
   * E.g. {'foo   '} is a subkey of {'foobar'}, while {'fooqux'} is not.
   */
  static bool isSubkey(const NGramKey& sub, const NGramKey& key);

  const std::set<std::string>& getInputWords() const;
  std::string prepareInputWord(const std::string& word) const;

  // Sanitized copy of input values
  std::set<std::string> inputWords_;

  // Unrolled markov chain entries.
  // This serves as the memory management container for the KeyNodes
  std::unordered_map<NGramKey, KeyNode,
                     boost::hash<NGramKey>> keyNodes_;

  // Entry point for the word generator.
  std::vector<std::pair<char, KeyNode*> > initList_;

  static const KeyNode END_NODE;
  const int N_init_;
};

// Defines a special end-node, that can be asigned to a KeyNode child,
// which, if selected upon word-generation, signifies the end of the word.
template<int N>
const typename WordGenerator<N>::KeyNode
WordGenerator<N>::END_NODE {{{}}, {}};

#endif  // UTIL_WORDGENERATOR_H
