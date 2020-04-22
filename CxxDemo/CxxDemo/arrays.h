#include <iostream>
#include <vector>
#include <array>

struct Vertex {
  float x, y, z;
};

std::ostream& operator<<(std::ostream& stream, const Vertex& vertex) {
  stream << vertex.x << "," << vertex.y << "," << vertex.z;
  return stream;
}

// When using std::array we neet to have a template around functions that
// need to know the array size.
template<std::size_t SIZE>
void multiply_array(const std::array<int, SIZE>& arr, const int multiplier) {
  for(auto& e : arr) {
    e *= multiplier;
  }
}

void arrays_demo() {
  // Store Vertex object (that would result in a inline memory layout).
  // Memory is contiguous / same cache line.
  // ** Cons ** When the array is resized all of the object are going to be
  // copied to the new allocated buffer.
  // Moving instead of copying largely solves this particular issue bu there are
  // still some copying which is not ideal.
  std::vector<Vertex> vs;
  // Memory is not contiguous.
  // Useful for large collections
  std::vector<Vertex*> vector_of_ptrs;
  
  vs.push_back({1, 2, 3});
  vs.push_back({2, 3, 4});

  // Iterate the vector.
  for (size_t i = 0; i < vs.size(); i++) {
    std::cout << vs[i] << std::endl;
  }
  //.. or range iteration
  for (const auto& /* or const Vertex& */ v : vs) {
    std::cout << v << std::endl;
  }
  // ** note **
  for (auto /* or Vertex */ v: vs) {
    std::cout << v << std::endl;
  }
  // will result in each object being copied while iterating.
  
  vs.erase(vs.begin() + 1); // Remove the item at index 1.
  vs.clear(); // Remove all of the elements.

  // ** optimization 1 **
  std::vector<Vertex> vs2;
  vs2.reserve(3); // Reserve memory for the vector ahead.
  
  // ** emplace_back: Prevents the object from being copied from the
  // caller stack to the vector contiguos mem by allocating it right away
  // in the vector buffer.
  vs2.emplace_back(Vertex{1, 2, 3}); // like push_back but no copies.
  vs2.emplace_back(Vertex{1, 2, 3});

  // ** static arrays. **
  // std::array is for arrays that don't grow.
  std::array<Vertex, 5> array;
  array[0] = {1,2,3};
  array[1] = {2,4,5};
}
