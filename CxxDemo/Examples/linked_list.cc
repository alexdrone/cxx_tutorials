#include "linked_list.h"

class String {
public:
  String() = default;
  String(const char * string) {
    std::cout << "created." << std::endl;
    size_ = strlen(string);
    data_ = new char[size_];
    memcpy(data_, string, size_);
  }
  String(const String& other) noexcept { // copy-constructor.
    std::cout << "copied." << std::endl;
    size_ = other.size_;
    data_ = new char[size_];
    memcpy(data_, other.data_, size_);
  }
  String(String&& other) { // move-constructor (takes r-value ref).
    std::cout << "moved." << std::endl;
    size_ = other.size_;
    data_ = other.data_; // We can simply steal the data_ ptr.
    other.size_ = 0;
    other.data_ = nullptr;
  }
  ~String() {
    std::cout << "destroyed." << std::endl;
    delete data_;
  }
  void print() const {
    for (size_t i = 0; i < size_; i++)
      printf("%c", data_[i]);
    printf("\n");
  }
private:
  char *data_;
  size_t size_;
};

void LinkedListTestSuite() {
  std::cout << "LinkedList Tests." << std::endl;

  auto *node = new Node<String>{"a"};
  node->Append("b");
  node->Append("c");
  node->Append("d");

  for (auto i = node->begin(); i != node->end(); i++) {
    (*i).print();
  }
  
}
