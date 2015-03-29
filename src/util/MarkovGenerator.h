#ifndef UTIL_MARKOVGENERATOR_H
#define UTIL_MARKOVGENERATOR_H

#include <array>
#include <map>
#include <string>
#include <utility>
#include <vector>

#include <boost/functional/hash.hpp>

#include <unordered_map>
#include <unordered_set>
#include <util/UnitTestUtil.h>
#include <util/MarkovGeneratorInterface.h>


/**
 * MarkovGenerator class.
 *
 * Markov Chain Random Data Generation tool.
 *
 * For simplicity, data will be considered 'words', which is comprised of a
 * sequence of 'letters'. Though, it might as well be "sentenctes and words",
 * instead of "words and letters". It might even be "words and syllables".
 *
 * Takes a list of words as input, creates a likelihood table with
 * "what is the prability that the letter is X, given that previous N letters
 * were [n_{i-1}, n_{i-2}, ..., n_{i-N}]
 *
 * N is the depth (aka. order or level) of the markov chain data generator
 * The nodes are N-grams, that point to a list of other N-grams.
 *
 * To allow more variability, a shallower depth can be used for the
 * initalization. This is called N_init.  1 <= N_init <= N
 *
 * Valid N in range [1, .., MarkovGeneratorInterface::MAX_ORDER]
 *
 * Complexity:
 * Creating the markov chain is O(K lg K), where K is the number of input words.
 * Generating a new word is O(1) (the average input word length is considered to
 * be constant).
 *
 * @author SwarmingLogic
 */
template<int N, typename T>
class MarkovGenerator : public MarkovGeneratorInterface<T>
{
  FRIENDLY_UNIT_TEST(MarkovGenerator)
  FRIENDLY_UNIT_TEST(WordGenerator)
  FRIENDLY_UNIT_TEST(WordGeneratorFactory)
 public:
  /**
   * If N_init is not in the range [1, N] then it is assigned N
   */
  explicit MarkovGenerator(int N_init = 0);
  virtual ~MarkovGenerator() {}

  /**
   * Add words as initializer list.
   */
  void addInputData(const std::vector<std::vector<T> >& data) override;

  /**
   * Calling this function creates internal tables that are
   * necessary for when generating data
   */
  void prepare() override;

  /**
   * Generates a data element. It has no guarantee of being unique.
   *
   * Choses a random starting node, and keeps selecting a random child,
   * (building up the data), until it encounters END_NODE
   */
  std::vector<T> generate() const override;

  /**
   * Checks if the data parameter already exists as a input word.
   */
  bool isInputData(const std::vector<T>& data) const override;

  // Disable copy
  MarkovGenerator(const MarkovGenerator& c) = delete;
  MarkovGenerator& operator=(const MarkovGenerator& c) = delete;


 private:
  // The NGRAM in the markov chain nodes
  using NGramKey = std::array<T, N>;

  // A graph node, representing the markov chain model
  struct KeyNode {
    NGramKey key;
    std::vector<KeyNode*> children;
  };

  /**
   * Processes all input data, and generates the list of NGramKeys
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
  KeyNode& getEntry(const std::vector<T>& word, size_t start);


  /**
   * Helper function to extract N characters from a string,
   * at specified start index, and return this as an NGram.
   */
  static NGramKey createNGram(const std::vector<T>& word, size_t start);

  /**
   * Helper function to determine if an NGram is a prefix subkey of another
   * NGram. Spaces are used for the un-set values in the subkey.
   *
   * E.g. {'foo   '} is a subkey of {'foobar'}, while {'fooqux'} is not.
   */
  static bool isSubkey(const NGramKey& sub, const NGramKey& key);


  /**
   * Pops values from end until a value != T{} is found.
   */
  static void rtrim(std::vector<T>& word);

  std::set<std::vector<T> > inputData_;

  // Unrolled markov chain entries.
  // This serves as the memory management container for the KeyNodes
  std::unordered_map<NGramKey, KeyNode,
                     boost::hash<NGramKey>> keyNodes_;

  // Entry point for the word generator.
  std::vector<std::pair<T, KeyNode*> > initList_;

  static const KeyNode END_NODE;
  const int N_init_;
};

// Defines a special end-node, that can be asigned to a KeyNode child,
// which, if selected upon word-generation, signifies the end of the word.
template<int N, typename T>
const typename MarkovGenerator<N, T>::KeyNode
MarkovGenerator<N, T>::END_NODE {{{}}, {}};


// Templetized type needs definition in header
#include <util/MarkovGenerator.tpp>

#endif  // UTIL_MARKOVGENERATOR_H
