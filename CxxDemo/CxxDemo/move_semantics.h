#pragma once

#include <iostream>
#include <utility>
#include <vector>
#include <string>

namespace move_demo {

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
    memcpy(data_, other.data_, size_);
  }
  String(String&& other) { // move-constructor (takes r-value ref).
    std::cout << "moved." << std::endl;
    size_ = other.size_;
    data_ = other.data_; // We can simply steal the data_ ptr.
    
    // We need to take care of the original object otherwise when deallocated
    // data is going to be deallocated to as per destructor.
    // Hollow object.
    other.size_ = 0;
    other.data_ = nullptr;
  }
  String& operator=(String&& other) noexcept { // move-assignment
    std::cout << "move assignment." << std::endl;
    if (this != &other) {
      delete[] data_;
      data_ = other.data_;
      size_ = other.size_;
    }
    return *this;
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

class Entity {
public:
  Entity(const String& name): name_{name} { // copy-constructor.
  }
  Entity(String&& name): name_{std::move(name)} {
    // Entity also needs an overload with a temporary.
    // * It's important that we cast the designated initializer to (String&&) otherwise it will
    // perform a copy. *
    // (String&&) is equal to call std::move.
    
    // In particular, std::move produces an xvalue expression that identifies its argument t.
    // It is exactly equivalent to a static_cast to an rvalue reference type.
    
    // One exception is when the type of the function parameter is rvalue reference to type
    // template parameter ("forwarding reference" or "universal reference"),
    // in which case std::forward is used instead.
  }
  ~Entity() {
  }
  void print() const {
    name_.print();
  }
private:
  String name_;
};


}

void move_semantics_demo() {
  // move semantics are useful when we want to transfer ownership without performing a copy.
  using namespace move_demo;
  Entity(String("Foo"));
  
  // [without String(const String&& other) and Entity(String&& name)]
  // This will result in the program printing:
  // created.
  // copied.
  // destroyed.
  // ... We want to get rid of the useless copy.
  
  // [with String(const String&& other) and Entity(String&& name)]
  // This will result in the program printing:
  // created.
  // moved.
  // destroyed.
}

void move_assignment_demo() {
  using namespace move_demo;
  String src = "Hello";
  String src_2 = "Hello2";
  String copy_dest = src; // This is a copy.
  
  String move_dest_1((String&&)src); // To move it we need to cast src to a temporary (r-value).
  String move_dest_2(std::move(move_dest_1)); // More elegant way to transform src into a temporary.
  // ** note ** This can be written as an assignment but IT IS NOT an assignment.
  // (Just syntactic sugar for the constructor).
  // The assignment operator is called ONLY when the lhs is an already existing value.
  String move_dest_3 = std::move(move_dest_2);
  
  // move_dest_3 = std::move(src) This WON'T work without the move assignment operator override.
  // String& operator=(String&& other) noexcept
  move_dest_3 = std::move(src_2);
}

void move_semantics_push_back() {
   std::string str = "Hello";
   std::vector<std::string> v;

   // uses the push_back(const T&) overload, which means
   // we'll incur the cost of copying str
   v.push_back(str);
   std::cout << "After copy, str is \"" << str << "\"\n";

   // uses the rvalue reference push_back(T&&) overload,
   // which means no strings will be copied; instead, the contents
   // of str will be moved into the vector.  This is less
   // expensive, but also means str might now be empty.
   v.push_back(std::move(str));
   std::cout << "After move, str is \"" << str << "\"\n";
}
