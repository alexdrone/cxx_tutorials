#include <iostream>
#include <thread>
#include <future>
#include <functional>

#include "dispatch/dispatch.h"

namespace gcd {

// Implementation of std::async that uses Apple's Grand Central Dispatch.
template <typename F, typename ...Args>
auto async(F&& f, Args&&... args) -> std::future<typename std::result_of<F (Args...)>::type> {
  using result_type = typename std::result_of<F (Args...)>::type;
  using packaged_type = std::packaged_task<result_type ()>;

  auto p = new packaged_type(std::bind(std::forward<F>(f), std::forward<Args>(args)...));
  auto result = p->get_future();
  dispatch_async_f(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), p, [](void* f_) {
    packaged_type* f = static_cast<packaged_type*>(f_);
    (*f)();
    delete f;
  });

  return result;
}

template<typename It, typename F>
inline void parallel_for_each(It a, It b, F&& f) {
  size_t count=std::distance(a,b);
  using data_t=std::pair<It,F>;
  data_t helper=data_t(a,std::forward<F>(f));
  dispatch_apply_f(count, dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), &helper,
                   [](void* ctx,size_t cnt) {
    data_t* d=static_cast<data_t*>(ctx);
    auto elem_it=std::next(d->first,cnt);
    (*d).second(*(elem_it));
  });
}

}


