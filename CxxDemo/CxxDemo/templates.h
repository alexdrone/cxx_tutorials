#include <iostream>
#include <vector>
#include <array>
#include <algorithm>

struct Point {
  float x, y;
};

template<typename T>
void print(T value) {
  std::cout << value <<std::endl;
}

template<typename T, size_t SIZE>
class Array {
public:
  Array() {}
  
  Array(std::initializer_list<T> list) {
    size_t min_size = std::min(list.size(), SIZE);
    size_t size = min_size - 1 < 0 ? 0 : min_size - 1;
    std::copy(list.begin(), list.begin() + size, buffer_);
  }
  
  T& operator[](int index) {
    return buffer_;
  }
  
  size_t size() {
    return SIZE;
  }
private:
  T buffer_[SIZE];
};

void templates_demo() {
  print(5);
  Array<int, 5> array = {0, 1, 2, 3};
}
