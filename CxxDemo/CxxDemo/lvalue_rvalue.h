#include <iostream>

int get_rvalue() {
  return 10;
}

int& get_lvalue_ref() {
  static int __lvalue_ref = 10;
  return __lvalue_ref;
}
 
// can be called with lvalue or rvalue args.
void set_value(int value) { }

// can be called with lvalue only.
void set_value_with_lvalue_ref_arg(int& value) { }

// accepts [const lvalue_refs], hence lvalues and rvalues.
void set_value_with_const_lvalue_ref(const int& value) { }

// accepts lvalues only.
void print_name_with_lvalue_ref(std::string& name) {
  std::cout << name << std::endl;
}

// accepts [const lvalue_refs], hence lvalues and rvalues.
void print_name_with_const_lvalue_ref(const std::string& name) {
  std::cout << name << std::endl;
}

// accepts [rvalue_refs] only.
void print_name_with_rvalue_ref(std::string&& name) {
  std::cout << name << std::endl;
}

// Overrides.

// accepts [const lvalue_refs], hence lvalues and rvalues.
void print_name(const std::string& name) {
  std::cout << "[lvalue]" << name << std::endl;
}

// accepts [rvalue_refs] only.
// specialized override for temporary values only.
// useful with move semantics because the ref can be stolen.
void print_name(std::string&& name) {
  std::cout << "[rvalue]" <<  name << std::endl;
}

void lvalue_rvalue_demo() {
  // lvalue have a location, storage (usually left part of the expr.)
  // rvalue are temporary values (right side of the expr.)
  
  int i = 10; // i [lvalue] = 0 [rvalue].
  // 10 = i; [ERROR] You cannot store anything in a [rvalue]
  int a = i; // a [lvalue] = i [lvalue].
  
  // [rvalue]s can be returned by a func call a well.
  int i2 = get_rvalue(); // i [lvalue] = get_rvalue [rvalue].
  // get_rvalue() = 5; [ERROR] You cannot store anything in a [rvalue].
  // (Expression must be a modifiable lvalue).
  
  get_lvalue_ref() = 42; // get_lvalue_ref [lvalue_ref] = 42 [rvalue]. Works fine.
  
  // void set_value(int value) can be called with lvalue or rvalue args.
  set_value(5);
  set_value(i);
  
  // ** You cannot take an [lvalue_ref] from an [rvalue].
  set_value_with_lvalue_ref_arg(i);
  // set_value_with_lvalue_ref_arg(5); [ERROR] Initial value of ref
  //to non-const must be an lvalue.
  
  // ** const **
  // While you cannot take an [lvalue_ref] from an [rvalue] (e.g. int& a = 10)
  // you CAN take an [const lvalue_ref] from an [rvalue].
  // int& b = 10; [ERROR].
  const int& b = 10; // b [lvalue_ref] = 10 [rvalue].
  // (The compiler create a temp hidden storage for the rvalue to assign its ref)
  
  // set_value_all(const int& value) accepts [const lvalue_refs],
  // hence lvalues and rvalues.
  // ** This is the preferred signature for refs. **
  set_value_with_const_lvalue_ref(5);
  set_value_with_const_lvalue_ref(i);
  set_value_with_const_lvalue_ref(b);
  
  std::string firstname = "John"; // firstname [lvalue] = "John" [rvalue]
  std::string lastname = "Appleseed"; // lastname [lvalue] = "Appleseed" [rvalue]
  std::string fullname = firstname + lastname; // fullname [lvalue] = (firstname + lastname) [rvalue]
  
  print_name_with_lvalue_ref(fullname);
  //print_name_with_lvalue_ref(firstname + lastname); // [ERROR].
  
  print_name_with_const_lvalue_ref(fullname);
  print_name_with_const_lvalue_ref(firstname + lastname);
  
  // ** rvalue_refs **
  // You can pass rvalue_refs using the && operator.
  // e.g. void print_name(std::string&& name)
  
  print_name_with_rvalue_ref(firstname + lastname); // Works, arg is a rvalue.
  // print_name_with_rvalue_ref(fullname); // [ERROR], arg is a lvalue.
  
  // ** overrides **
  print_name(fullname); // Prints: "[lvalue] John Appleseed".
  // accepts [rvalue_refs] only.
  // specialized override for temporary values only.
  // useful with move semantics because the ref can be stolen since we know
  // that the resource is not owned by anybody.
  print_name(firstname + lastname); // Prints: "[rvalue] John Appleseed".
}

