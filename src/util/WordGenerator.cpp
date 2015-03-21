#include <util/WordGenerator.h>

#include <algorithm>

#include <boost/algorithm/clamp.hpp>
#include <boost/algorithm/string.hpp>

#include <math/MathUtil.h>
#include <math/Random.h>
#include <util/StringUtil.h>


template<int N>
WordGenerator<N>::WordGenerator(int N_init)
    : N_init_((N < N_init || N_init <= 0) ? N : N_init)
{
  (void)N_init;
}


template<int N>
void WordGenerator<N>::addInputWords(const std::string& text, char dlim)
{
  addInputWords(StringUtil::split(StringUtil::trimc(text), dlim));
}


template<int N>
void WordGenerator<N>::addInputWords(const std::vector<std::string>& words)
{
  // Sanitize input list
  for (auto& word : words) {
    std::string local = word;
    StringUtil::trim(local);
    boost::algorithm::to_lower(local);
    if (local.size() >= N)
      inputWords_.push_back(local);
  }
  // Sort for speed
  std::sort(inputWords_.begin(), inputWords_.end());
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
  auto init = initList_[Random::getInt(0,
                                       static_cast<int>(initList_.size()) - 1)];
  // Adds the first randomly chosen init char and key,
  // together these form the initial word.
  word += init.first;
  word.append(&init.second[0], init.second.size());

  ListKey currentKey = init.second;
  const ListValue* possibleNextValues = &entries_.at(currentKey);
  char nextValue =
      (*possibleNextValues)[Random::getInt(
          0, static_cast<int>(possibleNextValues->size()) - 1)];

  while (nextValue != END_VALUE) {
    // insert value
    word += nextValue;

    // update key & value
    // Shift key one value to left
    std::rotate(currentKey.begin(),
                currentKey.begin() + 1,
                currentKey.end());
    currentKey[N - 1] = nextValue;
    possibleNextValues = &entries_.at(currentKey);
    nextValue =
        (*possibleNextValues)[Random::getInt(
            0, static_cast<int>(possibleNextValues->size()) - 1)];
  }

  if ( *--word.end()  == END_VALUE )
    word.pop_back();

  return word;
}

template<int N>
bool WordGenerator<N>::isInputWord(const std::string& word) const
{
  std::string local = word;
  StringUtil::trim(local);
  boost::algorithm::to_lower(local);

  return std::any_of(inputWords_.cbegin(), inputWords_.cend(),
                     [&local](const std::string& w){
                       return w == local;
                     });
}



template<int N>
const std::vector<std::string>& WordGenerator<N>::getInputWords() const
{
  (void)inputWords_;
  return inputWords_;
}

template<int N>
void WordGenerator<N>::prepareLookupTable() {
  for (auto& word : inputWords_)
    insertWordIntoLookupTable(word + END_VALUE);
}


template<int N>
void WordGenerator<N>::prepareInitKeyList()
{
  // N_init = 1
  // ... this should be general for 1 <= N_init <= N

  std::vector<std::pair<ListKey, char> > initPrefixList;

  // 1. Create initial key prefix list,
  for (auto& w : inputWords_) {
    ListKey key;
    key.fill(' ');
    for (int i = 1 ; i < N_init_ + 1 ; ++i)
      key[i-1] = w[i];

    if ( N_init_ == N )
      initList_.push_back(std::make_pair(w[0], key));
    else
      initPrefixList.push_back(std::make_pair(key, w[0]));
  }

  // If N_init_ == N we are done now
  if ( N_init_ < N ) {
    // 2. Uniquify key prefix list
    std::sort(initPrefixList.begin(), initPrefixList.end());
    auto it = std::unique(initPrefixList.begin(), initPrefixList.end());
    initPrefixList.erase(it, initPrefixList.end());

    // 3. Expand key prefix to matching keys
    // Since both lists are sorted, this perfoms in
    // O(#INPUTWORDS), as a result of only needing a linear traversal
    // of prefix keys and keys.

    ListKey lastPrefixKey { END_VALUE };
    std::vector<ListKey> expandedPrefixes;
    auto iEntry = entries_.begin();
    for (auto& prefix : initPrefixList) {
      // 1. Check if current prefixKey == lastPrefixKey
      // 2. Copy previous result. Continue
      if (prefix.first == lastPrefixKey) {
        for (auto& expKey : expandedPrefixes)
          initList_.push_back(std::make_pair(prefix.second, expKey));
        continue;
      }

      // If not
      // 1. Reset previous result, update lastPrefixKey, expand prefixes
      expandedPrefixes.clear();
      lastPrefixKey = prefix.first;

      // You have prefixKey prefix.first, for each match in entries_.first, add
      // entries_.first to expandedPrefixes
      while (iEntry != entries_.end()) {
        // current entry key
        const ListKey& key = iEntry->first;

        const bool isSubKey = isSubkey(prefix.first, key);

        // Check if passed
        if (!isSubKey && (key > prefix.first))
          break;

        ++iEntry;
        if (isSubkey(prefix.first, key))
          expandedPrefixes.push_back(key);
      }

      for (auto& expKey : expandedPrefixes)
        initList_.push_back(std::make_pair(prefix.second, expKey));
    }
  }
}


template<int N>
int WordGenerator<N>::getN() const { return N; }

template<int N>
int WordGenerator<N>::getN_init() const { return N_init_ ; }


template<int N>
void WordGenerator<N>::insertWordIntoLookupTable(const std::string& word)
{
  std::string local = StringUtil::suffix(word, -1);
  std::string keystr;
  do  {
    keystr = StringUtil::prefix(local, N + 1);
    local = StringUtil::suffix(local, -1);

    // Prepare key
    ListKey key { ' ' };
    for (int i = 0 ; i < N ; ++i)
      key[i] = keystr[i];

    // Get entry for key
    getEntry(key).push_back(*--keystr.end());

  } while (*--keystr.end() != END_VALUE);
}


template<int N>
typename WordGenerator<N>::ListValue&
WordGenerator<N>::getEntry(const ListKey& key)
{
  auto it = entries_.find(key);
  if ( it == entries_.end() )
    return entries_[key];
  else
    return it->second;
}


template<int N>
bool WordGenerator<N>::isSubkey(const ListKey& sub, const ListKey& key)
{
  for (size_t i = 0 ; i < key.size() ; ++i)
    if ((sub[i] != key[i]) && (sub[i] != ' '))
      return false;
  return true;
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

