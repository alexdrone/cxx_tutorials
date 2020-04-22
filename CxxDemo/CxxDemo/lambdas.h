#include <iostream>
#include <vector>
#include <array>
#include <algorithm>
#include <functional>

void hello() {
  std::cout << "hello" << std::endl;
}

void print_value(int value) {
  std::cout << value << std::endl;
}

// A function that takes a function pointer as an argument.
void for_each_ptr(const std::vector<int>& values, void(*do_function)(int)) {
  for (const auto& value: values) {
    do_function(value);
  }
}

// Version of the function above that takes a std::function argument
// instead of a raw function ptr.
// function pointers don't work with lambdas that have a non-empty capture
// block.
void for_each(const std::vector<int>& values, std::function<void(int)> func) {
  for (const auto& value: values) {
    func(value);
  }
} 

void lambdas_demo() {
  // ** function pointers **
  
  // gets a pointer to the hello function.
  auto function = hello; // &hello would work too.
  function(); // call the function at the pointer.
  
  // explicit type.
  void(*func)() = hello; // similiar to objc block syntax.

  // typedef'd function type.
  typedef void(*HelloFunctionType)();
  HelloFunctionType func2 = hello;
  
  std::vector<int> values = {1, 2, 3, 4, 5};
  for_each_ptr(values, print_value);
  
  // ** lambdas **

  // Whenever you have a function pointer argument you can replace it
  // in the calling site with a lambda.
  // [] is the capture block.
  for_each_ptr(values, [](int value) {
    std::cout << value << std::endl;
  });
  
  // [&]: all of the vars are captured by ref.
  // [=]: all of the var are capture by value.
  // [&a, b]: a is captured by ref, b by value.
  // [this]: 'this' object is being captured.
  
  int a = 5;
  auto lambda_that_captures_a_by_value = [a](int value) {
    std::cout << value + a << std::endl;
  };
  // for_each_ptr(values, lambda_that_captures_a_by_value); [ERROR]
  // function pointers don't work with lambdas that have a non-empty capture
  // block.
  // We can replace the function pointer in the arg with std::function.
  for_each(values, lambda_that_captures_a_by_value);

  // if a lambda wants to mutate a variable that was passed in the
  // capture block, it must be marked as 'mutable'.
  int b = 2;
  auto lambda_that_captures_b_by_ref_and_modifies_it = [&b](int value) mutable {
    b = 10;
  };
  
  // example of usage in std
  
  auto iterator = std::find_if(values.begin(), values.end(), [](int value) {
    return value > 1;
  });
  auto found_value = *iterator;
}

void f(int n1, int n2, int n3, const int& n4, int n5) {
    std::cout << n1 << ' ' << n2 << ' ' << n3 << ' ' << n4 << ' ' << n5 << '\n';
}

int g(int n1) {
    return n1;
}
 
void bind_demo() {
  using namespace std::placeholders;  // for _1, _2, _3...

  // demonstrates argument reordering and pass-by-reference
  int n = 7;
  // (_1 and _2 are from std::placeholders, and represent future
  // arguments that will be passed to f1)
  auto f1 = std::bind(f, _2, 42, _1, std::cref(n), n);
  n = 10;
  
  // 1 is bound by _1, 2 is bound by _2, 1001 is unused
  //makes a call to f(2, 42, 1, n, 7)
  f1(1, 2, 1001);
  // ** output ** 2 42 1 10 7
  
  // nested bind subexpressions share the placeholders
  auto f2 = std::bind(f, _3, std::bind(g, _3), _3, 4, 5);
  f2(10, 11, 12); // makes a call to f(12, g(12), 12, 4, 5);
  // ** output ** 12 12 12 4 5
}
