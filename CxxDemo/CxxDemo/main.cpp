#include <iostream>

#include "lvalue_rvalue.h"
#include "copying.h"
#include "constructors.h"
#include "arrays.h"
#include "templates.h"
#include "lambdas.h"
#include "smart_pointers.h"
#include "strings.h"
#include "structured_bindings.h"
#include "optionals_variants_any.h"
#include "threads_async_futures.h"

int main(int argc, const char *argv[]) {
  std::cout << "Hello, World!" << std::endl;
  
  async_demo();
}
