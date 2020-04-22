#include <iostream>
#include <vector>
#include <memory>


class Obj {
public:
  Obj() {
    std::cout << "created" << std::endl;
  }
  Obj(int value): value_{value} {
    std::cout << "created" << std::endl;
  }
  ~Obj() {
    std::cout << "destructed" << std::endl;
  }
  void print() const {
    std::cout << "hello" << std::endl;
  }
private:
  int value_;
};
    
void do_something_with_an_obj(const Obj& object) {
  object.print();
}

void smart_pointers_demo() {
  // Enforce RAII.
  
  std::cout << "unique_ptr" << std::endl;
  // ** unique_ptr **
  {
    // A scoped pointer that cannot be copied.
    // You can construct a unique pointer by calling make_unique.
    std::unique_ptr<Obj> obj = std::make_unique<Obj>(10);
    // The arrow operator is overriden so that you can automatically derefence
    // the pointee.
    obj->print();
    do_something_with_an_obj(*obj);
    //std::unique_ptr<Obj> obj2 = obj; [ERROR] You cannot copy a unique_ptr.
  } // the unique pointer is released here and so is its wrapped object.
  
  
  std::cout << "shared_ptr" << std::endl;
  // ** shared_ptr **
  {
    // Scoped pointer that can be shared and implements reference counting.
    std::shared_ptr<Obj> obj = std::make_shared<Obj>(10); // ref-count: 1;
    std::shared_ptr<Obj> obj2 = obj; // ref-count: 2;
    {
      std::shared_ptr<Obj> obj3 = obj; // ref-count: 3
      do_something_with_an_obj(*obj3);
    }
    //ref-count: 2;
  }
  //ref-count: 0; *Obj is destructed.*
  
  std::cout << "weak_ptr" << std::endl;
  // ** weak_ptr **
  std::weak_ptr<Obj> weakObj;
  {
    std::shared_ptr<Obj> obj = std::make_shared<Obj>(10); // ref-count: 1;
    weakObj = obj; // ref-count: 1 ** weak_ptr does not increase the retain count.
    
    //A separate stored pointer is necessary to ensure that converting a shared_ptr to weak_ptr
    // and then back works correctly, even for aliased shared_ptrs. It is not possible to access
    //the stored pointer in a weak_ptr without locking it into a shared_ptr.
    if (auto shared_ptr = weakObj.lock()) {
      do_something_with_an_obj(*shared_ptr);
    }
  }
  //ref-count: 0; *Obj is destructed.*
  
  std::cout << weakObj.expired() << std::endl;
  // with 'expired' we can check if the pointee is still alive.
}
