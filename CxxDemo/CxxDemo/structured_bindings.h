#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include <tuple>

std::tuple<std::string, int> create_person_tuple() {
  return {"John Appleseed", 42};
}

void structured_bindings_demo() {
  auto person = create_person_tuple();
  std::string& name0 = std::get<0>(person);
  int age0 = std::get<1>(person);
  // or..
  std::string name1;
  int age1;
  std::tie(name1, age1) = create_person_tuple();

  // C++17 (structured bundings).
  auto[name, string] = create_person_tuple();
}
