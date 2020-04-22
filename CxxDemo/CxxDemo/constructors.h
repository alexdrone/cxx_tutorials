#include <iostream>

class Container {
public:
  // ** Abstract class / No constructor **
  
  // Adding 0 at the end makes it a pure virtual function.
  // A class with a pure virtual function is abstract.
  virtual size_t size() = 0;
  // Virtual destructor.
  virtual ~Container() {};
};

class Vec: public Container {
public:
  // Usage:
  // const auto v1 = new Vec{10};
  // const auto v2 = new Vec(10);
  Vec(size_t size): elements_{new double[size]}, size_{size} {}

  // Construct from an inline list of args.
  // Container *c = new Vec {10,20,30};
  Vec(std::initializer_list<double> list):
    // initialize the members first.
    elements_{new double[list.size()]},
    size_{list.size()} {
      std::copy(list.begin(), list.end(), elements_);
  }
  
  virtual size_t size() override {
    return size_;
  }

  // Destructor.
  ~Vec() {
    delete[] elements_;
  }

  // const at the end of function declaration means non-mutating.
  virtual void foo() const {
    // size_ = 10; [ERROR] You cannot mutate the member in a const method.
    foo_ = 42; // This is allowed because the member is mutable.
    std::cout << "Foo in Vec" << std::endl;
  }

  private:
    double* elements_;
    size_t size_;
    mutable int foo_;
};


//// is kind of:
//// if (const auto& v = dynamic_cast<SubVec *>(o)) { ... }
class SubVec: public Vec {
public:
  // Inherit super constructor.
  // subclasses don't automatically inherit constructors from their
  // superclasses.
  SubVec(std::initializer_list<double> list): Vec(list) { };
  
  // override must be explicit in C++.
  virtual void foo() const override {
    std::cout << "Foo in SubVec" << std::endl;
  }

  void bar() {
    std::cout << "bar:" << bar_ << std::endl;
  }

private:
  int bar_;
};

// Another example.
class ScopedVecPtr {
public:
  ScopedVecPtr(Vec *vec): vec_{vec} { }
  
  ~ScopedVecPtr() {
    delete vec_;
  }
  
  // Overrides the -> operator so that dereferencing a ScopedVecPtr
  // returns the Vec pointer.
  Vec *operator->() {
    return vec_;
  }
  // const version of it.
  const Vec *operator->() const {
    return vec_;
  }
  
private:
  Vec *vec_;
};


struct SomeData {
  int foo;
  int bar;
};

void static_dynamic_cast_demo() {
  // Cast and polymorphism.
  Vec *vo = new Vec{10};
  Vec *v2o = new SubVec{4};
  vo->foo();
  v2o->foo();
  if (const auto vxx = dynamic_cast<SubVec *>(vo)) {
    // Fails cast.
    vxx->foo();
  } else {
    std::cout << "dynamic_cast: vo is not a Vector2" << std::endl;
  }
  if (const auto vxx = dynamic_cast<SubVec *>(v2o)) {
    // Calls SubVec impl.
    vxx->foo();
  } else {
    std::cout << "dynamic_cast: v2o is not a Vector2" << std::endl;
  }
  if (const auto vxx = dynamic_cast<Vec *>(v2o)) {
    // Still calling the right SubVec 2 impl.
    vxx->foo();
  } else {
    std::cout << "dynamic_cast: v2o is not a Vector" << std::endl;
  }
  if (const auto vxx = static_cast<SubVec *>(vo)) {
    // Casts Vec to its subclass because and works
    // would call SubVec impl even if the istance is a Vec.
    // static_cast bypass the v-table.
    vxx->foo();
    // This would work too but it would access to some garbage data for
    // _bar.
    vxx->bar();
  }
  
  // stack allocated objects can be assigned to their constructor argument
  // straight away.
  // ** note: The reason why you can directly assign a Vec& to a ScopedVecPtr
  // is becuse in C++ there's an implicit conversion.
  // This behaviour can be removed by using the keyword 'explicit' in the constructor.
  ScopedVecPtr scoped = new Vec { 10 };
  // Can use scoped as a Vec reference.
  scoped->foo();
  
  // ** Used arrow operator to get mem offset for a type **
  int64_t offset1 = (int64_t)&((SomeData*)nullptr)->foo; // prints 0.
  int64_t offset2 = (int64_t)&((SomeData*)nullptr)->bar; // prints 4.
  // useful to binary serialize data.
}


class Person {
public:
  Person(const std::string& name): name_{new std::string{name}}, age_{18} {}
  Person(int age): name_{new std::string{"N/A"}}, age_{age} {}
  explicit Person(double height): height_{height}, name_{new std::string{"N/A"}}, age_{18} { }
  
  const std::string& name() const {
    return *name_;
  }
  
private:
  const std::string *name_;
  int age_;
  double height_;
};

void print_person(const Person& person) {
  std::cout << person.name() << std::endl;
}

void implicit_conversion_demo() {
  // You can instantiate a new person like so.
  Person a = Person { "John" };
  // ..but also by calling the constructor directly.
  Person b("John");
  // Implicit conversions are performed whenever an expression of some type T1 is used in context
  // that does not accept that type, but accepts some other type T2.
  Person c = std::string { "John" }; // ** we cannot do = "John" directly because that would be
                                     // 2 implicit conversions (char * > std:: string > Person)
                                     // and C++ does just one.
  Person d = 12;
  
  // This applies also to function arguments.
  print_person(a);
  print_person(12); // This gets converted into Person(12).
  
  // ** You can disable this behaviour by using the explicit keyword
  // Person e = 185.0; [ERROR] constructor with double is explicit.
}
