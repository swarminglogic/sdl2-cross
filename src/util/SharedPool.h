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

  SharedPool()
      : is_valid_(new bool(true)),
        external_count_(new size_t(0))
  {}
  virtual ~SharedPool(){
    *is_valid_ = false;
    if (*external_count_ == 0) {
      delete is_valid_;
      is_valid_ = nullptr;

      delete external_count_;
      external_count_ = nullptr;
    }
  }

  void add(std::unique_ptr<T> t) {
    pool_.push(std::move(t));
  }

  ptr_type acquire() {
    assert(!pool_.empty());
    const auto is_valid_tmp_ptr = is_valid_;
    const auto external_count_tmp_ptr = external_count_;
    ptr_type tmp(pool_.top().release(),
                 [this, is_valid_tmp_ptr, external_count_tmp_ptr](T* ptr) {
                   if (*is_valid_tmp_ptr) {
                     this->re_add(std::unique_ptr<T>(ptr));
                   }
                   else {
                     std::default_delete<T>()(ptr);
                     --*external_count_tmp_ptr;
                     if (*external_count_tmp_ptr == 0) {
                       delete external_count_tmp_ptr;
                       delete is_valid_tmp_ptr;
                     }
                   }
                 });
    ++*external_count_;
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
  void re_add(std::unique_ptr<T> t) {
    pool_.push(std::move(t));
    --*external_count_;
  }

  std::stack<std::unique_ptr<T> > pool_;
  bool* is_valid_;
  size_t* external_count_;
};

#endif  // UTIL_SHAREDPOOL_H
