#ifndef UTIL_OBJECTPOOL_H
#define UTIL_OBJECTPOOL_H

#include <memory>
#include <stack>

#include <util/Assert.h>
#include <util/Exception.h>


/**
 * ObjectPool class.
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
template <class T, class Deleter = std::default_delete<T> >
class ObjectPool
{
 private:
  struct External_Deleter {
    explicit External_Deleter(std::weak_ptr<ObjectPool<T, Deleter>* > pool)
        : pool_(pool) {}

    void operator()(T* ptr) {
      if (auto pool_ptr = pool_.lock()) {
        try {
          (*pool_ptr.get())->add(std::unique_ptr<T, Deleter>{ptr});
          return;
        } catch(...) {}
      }
      Deleter{}(ptr);
    }
   private:
    std::weak_ptr<ObjectPool<T, Deleter>* > pool_;
  };

 public:
  using ptr_type = std::unique_ptr<T, External_Deleter >;

  ObjectPool() : this_ptr_(new ObjectPool<T, Deleter>*(this)) {}
  virtual ~ObjectPool(){}

  void add(std::unique_ptr<T, Deleter> t) {
    pool_.push(std::move(t));
  }

  ptr_type acquire() {
    assert(!pool_.empty());
    ptr_type tmp(pool_.top().release(),
                 External_Deleter(
                     std::weak_ptr<ObjectPool<T, Deleter>*>{this_ptr_}));
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
  std::shared_ptr<ObjectPool<T, Deleter>* > this_ptr_;
  std::stack<std::unique_ptr<T, Deleter> > pool_;
};

#endif  // UTIL_OBJECTPOOL_H
