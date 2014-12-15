#ifndef UTIL_RAIIHELPER_H
#define UTIL_RAIIHELPER_H

#include <memory>


/**
 * RAII helper class for allowing deleter to be part of a shared_ptr type.
 */
template<class T, class D = std::default_delete<T>>
struct shared_ptr_with_deleter : public std::shared_ptr<T>
{
  explicit shared_ptr_with_deleter(T* t = nullptr)
      : std::shared_ptr<T>(t, D()) {}

  void reset(T* t = nullptr) {
    std::shared_ptr<T>::reset(t, D());
  }
};


#endif  // UTIL_RAIIHELPER_H
