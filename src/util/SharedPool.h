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
 private:
  struct External_Deleter {
    explicit External_Deleter(std::weak_ptr<SharedPool<T>* > pool)
        : pool_(pool) {}

    void operator()(T* ptr) {
      if (auto pool_ptr = pool_.lock()) {
        try {
          (*pool_ptr.get())->add(std::unique_ptr<T>{ptr});
          return;
        } catch(...) {}
      }
      std::default_delete<T>{}(ptr);
    }
   private:
    std::weak_ptr<SharedPool<T>* > pool_;
  };

 public:
  using ptr_type = std::unique_ptr<T, External_Deleter >;

  SharedPool() : this_ptr_(new SharedPool<T>*(this)) {}
  virtual ~SharedPool(){}

  void add(std::unique_ptr<T> t) {
    pool_.push(std::move(t));
  }

  ptr_type acquire() {
    assert(!pool_.empty());
    ptr_type tmp(pool_.top().release(),
                 External_Deleter{std::weak_ptr<SharedPool<T>*>{this_ptr_}});
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
  std::shared_ptr<SharedPool<T>* > this_ptr_;
  std::stack<std::unique_ptr<T> > pool_;
};

#endif  // UTIL_SHAREDPOOL_H
