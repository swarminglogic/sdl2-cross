#include <util/MarkovGenerator.h>

#include <algorithm>

#include <boost/algorithm/string.hpp>

#include <util/MiscUtil.h>
#include <util/StringUtil.h>


template<int N, typename T>
MarkovGenerator<N, T>::MarkovGenerator(int N_init)
    : N_init_((N < N_init || N_init <= 0) ? N : N_init)
{
  (void)N_init;
}


template<int N, typename T>
void MarkovGenerator<N, T>::addInputData(
    const std::vector<std::vector<T> >& words)
{
  // Sanitize input list
  for (const auto& word : words) {
    if (static_cast<int>(word.size()) > std::max(N - 2, 2))
      inputData_.insert(word);
  }
}


template<int N, typename T>
void MarkovGenerator<N, T>::prepare()
{
  prepareLookupTable();
  prepareInitKeyList();
}


template<int N, typename T>
std::vector<T> MarkovGenerator<N, T>::generate() const
{
  if (initList_.empty())
    return std::vector<T>{};

  std::vector<T> word;
  // Get start key
  auto& init = MiscUtil::getRandomElement(initList_);

  word.push_back(init.first);
  word.push_back(init.second->key[0]);

  KeyNode* currentNode = init.second;
  KeyNode* nextNode = MiscUtil::getRandomElement(init.second->children);

  while (nextNode != &END_NODE) {
    word.push_back(nextNode->key[0]);
    currentNode = nextNode;
    nextNode = MiscUtil::getRandomElement(currentNode->children);
  }
  assert(nextNode == &END_NODE);
  if (N > 1) {
    word.insert(word.end(),
                currentNode->key.begin() + 1,
                currentNode->key.begin() + N);
  }

  rtrim(word);
  return word;
}


template<int N, typename T>
bool MarkovGenerator<N, T>::isInputData(const std::vector<T>& word) const
{
  return inputData_.find(word) != inputData_.end();
}


template<int N, typename T>
void MarkovGenerator<N, T>::prepareLookupTable() {
  for (const auto& word : inputData_) {
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


template<int N, typename T>
void MarkovGenerator<N, T>::prepareInitKeyList()
{
  if (N_init_ == N)
    return;

  std::vector<std::pair<NGramKey, T> > initPrefixList;
  for (const auto& w : inputData_) {
    NGramKey key;
    key.fill(T{});
    for (int i = 1 ; i < N_init_ + 1 ; ++i)
      key[i-1] = w[i];

    initPrefixList.push_back(std::make_pair(key, w[0]));
  }

  // 2. Uniquify key prefix list
  std::sort(initPrefixList.begin(), initPrefixList.end());
  auto it = std::unique(initPrefixList.begin(), initPrefixList.end());
  initPrefixList.erase(it, initPrefixList.end());

  std::map<NGramKey, KeyNode> entries(keyNodes_.begin(), keyNodes_.end());
  std::vector<std::pair<T, KeyNode*> > localInitList;

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


template<int N, typename T>
typename MarkovGenerator<N, T>::KeyNode&
MarkovGenerator<N, T>::getEntry(const NGramKey& key)
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

template<int N, typename T>
typename MarkovGenerator<N, T>::KeyNode&
MarkovGenerator<N, T>::getEntry(const std::vector<T>& word, size_t start)
{
  return getEntry(createNGram(word, start));
}


template<int N, typename T>
bool MarkovGenerator<N, T>::isSubkey(const NGramKey& sub, const NGramKey& key)
{
  for (size_t i = 0 ; i < key.size() ; ++i)
    if ((sub[i] != key[i]) && (sub[i] != T{}))
      return false;
  return true;
}


template<int N, typename T>
void MarkovGenerator<N, T>::rtrim(std::vector<T>& word) {
  while (!word.empty() && *--word.cend() == T{})
    word.pop_back();
}


template<int N, typename T>
typename MarkovGenerator<N, T>::NGramKey
MarkovGenerator<N, T>::createNGram(const std::vector<T>& word,
                                   size_t start)
{
  NGramKey key;
  key.fill(T{});
  std::copy(word.begin() + start,
            std::min(word.begin() + start + N, word.end()),
            key.begin());
  return std::move(key);
}
