#ifndef UTIL_SHAREDPOOL_H
#define UTIL_SHAREDPOOL_H

#include <memory>
#include <stack>

#include <util/Assert.h>
#include <util/Exception.h>


/**
 * SharedPool class.
 *
 * LIFO (last-in-first-out) pool of objects.
 * Objects are added using add(),
 * and aquired using acquire()
 *
 * The acquired objects exist as unique_ptrs,
 * which, upon deletion, returns the object to the pool.
 *
 * @author SwarmingLogic
 */
template <class T>
class SharedPool
{
 public:
  using ptr_type = std::unique_ptr<T, std::function<void(T*)> >;

  SharedPool(){}
  virtual ~SharedPool(){
    while (!pool_.empty())
      std::default_delete<T>()(acquire().release());
  }

  void add(std::unique_ptr<T> t) {
    pool_.push(ptr_type(t.release() ,
                        [this](T* ptr) {
                          add(std::unique_ptr<T>(ptr));
                        }));
  }

  ptr_type acquire() {
    assert(!pool_.empty());
    ptr_type tmp(std::move(pool_.top()));
    pool_.pop();
    return std::move(tmp);
  }

  bool empty() const {
    return pool_.empty();
  }

  size_t size() const {
    return pool_.size();
  }

 private:
  std::stack<ptr_type> pool_;
};

#endif  // UTIL_SHAREDPOOL_H
