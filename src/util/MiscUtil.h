#ifndef UTIL_MISCUTIL_H
#define UTIL_MISCUTIL_H

#include <iostream>

#include <math/Random.h>
#include <util/Assert.h>


/**
 * MiscUtil functions
 *
 * Various refactored utility functions that end up here,
 * before being sorted and put elsewhere.
 *
 * @author SwarmingLogic
 */
class MiscUtil
{
 public:

  template<typename T>
  static typename T::value_type& getRandomElement(T& container) {
    return const_cast<typename T::value_type&>(
        getRandomElement(const_cast<const T&>(container)));
  }

  template<typename T>
  static const typename T::value_type& getRandomElement(const T& container) {
    assert(!container.empty());
    const int randomIndex =
        Random::getInt(0, static_cast<int>(container.size()) - 1);
    return container[randomIndex];
  }



  ~MiscUtil();
 private:
  MiscUtil();
};


#endif  // MISCUTIL_H
