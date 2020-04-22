#include <iostream>
#include <vector>
#include <memory>
#include <string>

void print_string(const std::string& string) {
  std::cout << string << std::endl;
}

void strings_demo() {
  // char is 1 byte. [ASCII encoding].
  // Other encodings takes more by per char. (ut16 for example).
  // Normal strings would just do latin encoding (based on 1-byte char).
  
  // C-style string.
  // This is not heap allocated.
  const char *c_style_string = "Hi"; // or u8"Hi" utf8;
  
  // All c-strings have a null-terminated char.
  // If you define a string manually you should add the null-termination char yourself.
  const char c[3] = {'H', 'i', 0};

  // std::string is based on a const char array.
  std::string str = "Hello";
  str += "World"; // append the string.
  str.size(); // Returns the string size.
  const auto position = str.find("Wo"); // Returns std::string:npos if there's no match.
  
  // wide chars.
  const wchar_t *c_style_wide_char_string = L"Hi";
  const char16_t *c_style_wide_char16_string = u"Hi"; // 2bytes utf16.
  const char32_t *c_style_wide_char35_string = U"Hi"; // 4bytes utf32.
  
  // string literals.
  using namespace std::string_literals;
  
  std::string format = "Hello"s + "World";
  std::string multiline = R"(
  Line1
  Line2
  )";
  
}
