#include <iostream>

// ** copying **

// By default, objects can be copied. This is true for objects of user-defined
// types as well as for builtin types. The default meaning of copy is memberwise
// copy: copy each member.

// copy semantics of structs and classes is identical.
struct Vec2 {
  float x, y;
};

// Primitive bare-metal String class.
class Str {
public:
  Str(const char *string) {
    size_ = strlen(string);
    buffer_ = new char[size_+1];
    memcpy(buffer_, string, size_);
    // null-terminated string.
    buffer_[size_] = 0;
  }
  
  // ** prevent copy **
  // Str(const Str& other) = delete;
  
  // ** shallow copy constructor (default implementation) **
  // Str(const Str& other): buffer_{other.buffer_}, size_{other.size_} { }
  
  // An alternative implementation would be:
  // Str(const Str& other) { memcpy(this, &other, sizeof(Str)) }
  
  // **deep copy constructor **
  Str(const Str& other): size_{other.size_} {
    buffer_ = new char[size_+1];
    // copies the other object buffer into this one.
    memcpy(buffer_, other.buffer_, size_+1);
  }
  
  virtual ~Str() {
    delete[] buffer_;
  }
  
  virtual char& operator[](unsigned int index) {
    return buffer_[index];
  }
  
private:
  char *buffer_;
  size_t size_;

  // note: By marking a function as 'friend' it can access its private
  // members.
  friend std::ostream& operator<<(std::ostream& stream, const Str& string);
};

// Makes Str usable with cout <<...
std::ostream& operator<<(std::ostream& stream, const Str& string) {
  stream << string.buffer_;
  return stream;
}

// Every time the function is called the copying constructor is called.
void print_that_performs_a_deep_copy_of_the_arg(Str string) {
  std::cout << string << std::endl;
}

// A readonly reference is passed to the function.
void print_that_does_not_copy(const Str& string) {
  std::cout << string << std::endl;
}

void copying_demo() {
  // ** stack allocation **
  Vec2 a = {2, 3};
  Vec2 b = a; // b is a copy of a.
  b.x = 5; // This does not change the value of a.
  
  // ** heap allocation **
  Vec2 *ha = new Vec2{2, 3};
  Vec2 *hb = ha; // Only the pointer is copied.
                 // (ha and hb points to the same storage).
  hb->x = 5; // This changes the value of ha.x as well.
  
  // ** objects **
  Str s1 = "John";
  Str s2 = s1; // copies s1 to s2.
  // ** If there's no copy constructor it performs a shallow copy of members.
  //    buffer_ is still shared among the two classes.
  // ** If Str(const Str& other) is defined it performs a deep copy.
  //    buffer_ is not shared.
  
  // ** If there's no copy constructor:
  std::cout << s1 << std::endl; // Prints "John"
  std::cout << s2 << std::endl; // Prints "John"
  // ** [CRASHES] We try to delete the same buffer_ twice.
}
