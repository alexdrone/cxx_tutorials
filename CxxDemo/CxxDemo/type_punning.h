#include <iostream>

struct Foo {
  int x, y;
}

void type_punning_demo() {
  // Given an integer.
  int a = 10;
  double b = a; // b = 10 (implicit conversion).
  
  // We could interpret it (NOT CAST IT) as a double for example.
  // Take the very same memory and treat it as a different type.
  
  // raw type punning an int into a double.
  double& c = *(double*)&a; // b = -9.2e+61
  
  // Given a struct (or a class).
  Foo e = Foo{5, 8};
  // we can interpret it as a int array.
  int *pos = (int*)6e;
  // pos[0] is 5.
  // pos[1] is 8.
  
  // ** reinterpret_cast conversion
  // Converts between types by reinterpreting the underlying bit pattern.
  int* pos2 = reinterpret_cast<int*>(&e)
}
