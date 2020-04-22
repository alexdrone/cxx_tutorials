#include <iostream>
#include <vector>
#include <memory>
#include <thread>
#include <future>
#include <mutex>
#include <functional>

#include "async_gcd.h"

static bool __finished = false;
static std::mutex __stdout_mutex;

void do_work() {
  using namespace std::literals::chrono_literals;
  
  std::cout << "Started thread id=" << std::this_thread::get_id() << std::endl;
  while (!__finished) {
    std::cout << "working..." << std::endl;
    // std::this_thread is used to have a ref to the current thread.
    std::this_thread::sleep_for(1s);
  }
}

void threads_demo() {
  std::thread worker(do_work);
  
  std::cin.get();
  __finished = true;
  std::cout << "finished." << std::endl;

  worker.join(); // 'join' block the current thread until the worker is done.
}

// * async and futures *

class AsyncDemo {
public:
  AsyncDemo(): inputs_{{1, 2, 3, 4}} {}
  void demo();

private:
  std::vector<int> inputs_;
  std::vector<int> results_;
  // Used to synchronize the access to result_
  std::mutex mutex_;
  std::vector<std::future<void>> futures_;
  
  int compute_heavy(int arg);
};


int AsyncDemo::compute_heavy(int arg) {
  { // logging.
    std::lock_guard<std::mutex> lock(__stdout_mutex);
    const auto thread_id = std::this_thread::get_id();
    std::cout << "compute_heavy arg=" << arg << " thread_id=" << thread_id << std::endl;
  }
  // A dumb long-running function.
  int result = arg;
  for (int i = 0; i < 100000000; i++) {
    result += i % 2 ? rand() : -arg;
  }
  { // logging.
    std::lock_guard<std::mutex> lock(__stdout_mutex);
    std::cout << "✔ compute_heavy arg=" << arg  << std::endl;
  }
  return result;
}

void AsyncDemo::demo() {
  for (int idx : inputs_) {
    futures_.emplace_back(std::async(std::launch::async, [=] {
      int result = this->compute_heavy(idx);
      // ** if we make this concurrent, this is not thread-safe. **
      // We'd have to use a mutex to access the resource.
      std::lock_guard<std::mutex> lock(this->mutex_);
      this->results_.push_back(result);
      //..unlock is done automatically in the lock_guard destructor.
    }));
  }
}

void async_demo() {
  AsyncDemo demo;
  demo.demo();
  
  std::cin.get();
}
