
// Logger public declaration part
// This file must be included from C++


#ifndef LOGGER_SINGLETON_HEADER
#define LOGGER_SINGLETON_HEADER

#include "logger_config.h"
#include "logger_interfaces.h"

namespace logging {

/// Singleton template implementation
template <typename _TIf, typename _TImpl = _TIf>
class singleton : public logger_singleton_interface<_TIf> {
 public:
  singleton(void (*on_create_fn)(_TIf*) = NULL,
            int (_TIf::*ref_fn)() = NULL, 
            int (_TIf::*deref_fn)() = NULL,
            int (_TIf::*ref_cnt_fn)() const = NULL, 
            _TIf* ptr = NULL, 
            bool need_delete = false)
      : ptr_(NULL),
        need_delete_(false),
        on_create_fn_(on_create_fn),
        ref_fn_(ref_fn),
        deref_fn_(deref_fn),
        ref_cnt_fn_(ref_cnt_fn) {
    if (ptr) reset(ptr, need_delete);
  }

  ~singleton() { release(); }

  LOG_INLINE _TIf* operator->() { return get(); }

  static void delete_fn(_TIf* obj, void* user_param) {
    delete obj;
  }

  _TIf* get() {
    if (!ptr_) {
      ptr_ = new _TImpl(&delete_fn, this);
      need_delete_ = true;

      if (ref_fn_) (ptr_->*ref_fn_)();

      if (on_create_fn_)
        on_create_fn_(ptr_);
    }

    return ptr_;
  }

  void reset(_TIf* ptr, bool need_delete = true) {
    release();

    need_delete_ = need_delete;
    ptr_ = ptr;

    if (ref_fn_) (ptr_->*ref_fn_)();
  }

  void release() {
    _TIf* ptr = ptr_;
    bool need_delete = need_delete_;

    ptr_ = NULL;
    need_delete_ = false;

    int cnt = 0;

    if (deref_fn_ && ptr) 
      cnt = (ptr->*deref_fn_)();

    if (cnt == 0 && !need_delete_) {
      ptr = NULL;
    }

    if (ptr && need_delete_) delete ptr;
  }

 private:
  _TIf* ptr_;
  bool need_delete_;

  void(*on_create_fn_)(_TIf*);
  int (_TIf::*ref_fn_)();
  int (_TIf::*deref_fn_)();
  int (_TIf::*ref_cnt_fn_)() const;
};

}//namespace logging

#endif /*LOGGER_SINGLETON_HEADER*/
