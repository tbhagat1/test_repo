#include <config.hpp>
#include <iostream>

int main() {
  support::config& c = support::config::instance();
  c.insert_or_assign<bool>("foo", true);
  bool b = c.find<bool>("foo");
  std::string s = c.as_string<bool>("foo");
  std::cout << s << std::endl;
}
