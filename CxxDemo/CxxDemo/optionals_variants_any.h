#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include <tuple>
#include <fstream>
#include <optional>
#include <variant>
#include <any>

std::string read_file_as_string(const std::string& filepath) {
  std::ifstream stream(filepath);
  if (stream) {
    std::string result = "some";
    // read file...
    stream.close();
    return result;
  };
  return std::string(); // The user won't know if the file is not there of the content is really "".
}

// C++17 Optionals
std::optional<std::string> read_file_as_string_optional(const std::string& filepath) {
  std::ifstream stream(filepath);
  if (stream) {
    std::string result = "some";
    // read file...
    stream.close();
    return result;
  };
  return {}; // The user won't know if the file is not there of the content is really "".
}

void optionals_demo() {
  std::string example1 = read_file_as_string("data.txt");
  const auto example2 = read_file_as_string_optional("data.txt");
  if (example2.has_value()) { // if (example2) { ... } would work as well because std::optional
                              // ovverides the bool operator.
    std::string data = *example2; // or example2.data()
  }
  
  // ** another convenient way to unwrap the optional if we have a default is value_or **
  const auto data = example2.value_or("N/A");
}

enum class ErrorCode {
  Unknown = 0,
  FileDoesNotExist = 1
};

// C++17 Variants
std::variant<std::string, ErrorCode> read_file_as_string_variant(const std::string& filepath) {
  std::ifstream stream(filepath);
  if (stream) {
    std::string result = "some";
    // read file...
    stream.close();
    return result;
  };
  return ErrorCode::FileDoesNotExist;
}

void variant_demo() {
  // variant is a new C++17 constructs that allows a variable to store 2 possible types.
  std::variant<std::string, int> data;
  data = "John Appleseed"; // Data is a string.
  data = 42; // Now data is an int.
  
  // accessing it.
  int string_data = std::get<int>(data);
  // if you try to access with a wrong type std::bad_variant_access is thrown.
  
  int index = data.index(); // returns 0 if data is a string, 1 otherwise.
  
  if (const auto value = std::get_if<std::string>(&data)) {
    // value is a string.
  }
  
  // variants are not implemented through unions, they contains both members.
}

// std::any is a modern C++17 void* alternative.

void any_demo() {
  
  std::any data;
  data = 2;
  data = "A C string";
  data = std::string("A string object");
  
  std::string string = std::any_cast<std::string>(data); // Will throw an exception if the type 
                                                         // is not the correct one.
}
