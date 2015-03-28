#include <util/WordGenerator.h>

#include <algorithm>

#include <boost/algorithm/string.hpp>

#include <util/MiscUtil.h>
#include <util/StringUtil.h>


template<int N>
WordGenerator<N>::WordGenerator(int N_init)
    : N_init_((N < N_init || N_init <= 0) ? N : N_init)
{
  (void)N_init;
}


template<int N>
void WordGenerator<N>::addInputWords(const std::vector<std::string>& words)
{
  // Sanitize input list
  for (auto& word : words) {
    std::string local = prepareInputWord(word);

    if (static_cast<int>(local.size()) > std::max(N - 2, 2))
      inputWords_.insert(local);
  }
}


template<int N>
void WordGenerator<N>::prepare()
{
  prepareLookupTable();
  prepareInitKeyList();
}


template<int N>
std::string WordGenerator<N>::generate() const
{
  if (initList_.empty())
    return "";

  std::string word("");
  // Get start key
  auto& init = MiscUtil::getRandomElement(initList_);

  word += init.first;
  word += init.second->key[0];

  KeyNode* currentNode = init.second;
  KeyNode* nextNode = MiscUtil::getRandomElement(init.second->children);

  while (nextNode != &END_NODE) {
    word += nextNode->key[0];
    currentNode = nextNode;
    nextNode = MiscUtil::getRandomElement(currentNode->children);
  }
  assert(nextNode == &END_NODE);
  if (N > 1)
    word.append(&currentNode->key[1], N - 1);

  StringUtil::rtrim(word);
  return word;
}


template<int N>
bool WordGenerator<N>::isInputWord(const std::string& word) const
{
  const std::string local = prepareInputWord(word);
  return inputWords_.find(local) != inputWords_.end();
}


template<int N>
std::string WordGenerator<N>::prepareInputWord(const std::string& word) const
{
  std::string local = word;
  StringUtil::trim(local);
  boost::algorithm::to_lower(local);
  return std::move(local);
}


template<int N>
const std::set<std::string>& WordGenerator<N>::getInputWords() const
{
  (void)inputWords_;
  return inputWords_;
}


template<int N>
void WordGenerator<N>::prepareLookupTable() {
  for (const auto& word : inputWords_) {
    // For each word, first, get ngram key
    KeyNode* currentNode = &getEntry(createNGram(word, 1));
    initList_.push_back(std::make_pair(word[0], currentNode));

    size_t i = 0;
    for (i = 2 ; i + N < word.size() + 1; ++i) {
      KeyNode* nextNode = &getEntry(createNGram(word, i));
      currentNode->children.push_back(nextNode);
      currentNode = nextNode;
    }
    currentNode->children.push_back(const_cast<KeyNode*>(&END_NODE));
  }
}


template<int N>
void WordGenerator<N>::prepareInitKeyList()
{
  if (N_init_ == N)
    return;

  std::vector<std::pair<NGramKey, char> > initPrefixList;
  for (const auto& w : inputWords_) {
    NGramKey key;
    key.fill(' ');
    for (int i = 1 ; i < N_init_ + 1 ; ++i)
      key[i-1] = w[i];

    initPrefixList.push_back(std::make_pair(key, w[0]));
  }

  // 2. Uniquify key prefix list
  std::sort(initPrefixList.begin(), initPrefixList.end());
  auto it = std::unique(initPrefixList.begin(), initPrefixList.end());
  initPrefixList.erase(it, initPrefixList.end());

  std::map<NGramKey, KeyNode> entries(keyNodes_.begin(), keyNodes_.end());
  std::vector<std::pair<char, KeyNode*> > localInitList;

  std::vector<NGramKey> expandedPrefixes;
  NGramKey lastPrefixKey;
  auto iEntry = entries.begin();
  for (auto& prefix : initPrefixList) {
    // 1. Check if current prefixKey == lastPrefixKey
    // 2. Copy previous result. Continue
    if (prefix.first == lastPrefixKey) {
      for (auto& expKey : expandedPrefixes)
        localInitList.push_back(
            std::make_pair(prefix.second, &getEntry(expKey)));
      continue;
    }

    // If not
    // 1. Reset previous result, update lastPrefixKey, expand prefixes
    expandedPrefixes.clear();
    lastPrefixKey = prefix.first;

    // You have prefixKey prefix.first, for each match in entries.first, add
    // entries_.first to expandedPrefixes
    while (iEntry != entries.end()) {
      // current entry key
      const NGramKey& key = iEntry->first;

      const bool isSubKey = isSubkey(prefix.first, key);

      // Check if passed
      if (!isSubKey && (key > prefix.first))
        break;

      ++iEntry;
      if (isSubkey(prefix.first, key))
        expandedPrefixes.push_back(key);
    }

    for (auto& expKey : expandedPrefixes)
      localInitList.push_back(
            std::make_pair(prefix.second, &getEntry(expKey)));
  }
  initList_ = localInitList;
}


template<int N>
typename WordGenerator<N>::KeyNode&
WordGenerator<N>::getEntry(const NGramKey& key)
{
  auto it = keyNodes_.find(key);
  if ( it == keyNodes_.end() ) {
    KeyNode& node = keyNodes_[key];
    node.key = key;
    return node;
  } else {
    return it->second;
  }
}

template<int N>
typename WordGenerator<N>::KeyNode&
WordGenerator<N>::getEntry(const std::string& word, size_t start)
{
  return getEntry(createNGram(word, start));
}


template<int N>
bool WordGenerator<N>::isSubkey(const NGramKey& sub, const NGramKey& key)
{
  for (size_t i = 0 ; i < key.size() ; ++i)
    if ((sub[i] != key[i]) && (sub[i] != ' '))
      return false;
  return true;
}


template<int N>
typename WordGenerator<N>::NGramKey
WordGenerator<N>::createNGram(const std::string& word, size_t start)
{
  NGramKey key;
  key.fill(' ');
  std::copy(word.begin() + start,
            std::min(word.begin() + start + N, word.end()),
            key.begin());
  return std::move(key);
}


/**
 * We instantiate the template for the valid range of integers.  This isn't
 * particularly pretty, but the increased executable size is negligeble. And it
 * allows a separated .cpp file with implementation.
 */
template class WordGenerator<1>;
template class WordGenerator<2>;
template class WordGenerator<3>;
template class WordGenerator<4>;
template class WordGenerator<5>;
template class WordGenerator<6>;
