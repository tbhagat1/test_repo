#include <iostream>
#include <vector>
#include <sp.hpp>

struct foo {
  foo() {
    std::cout << "constructor" << std::endl;
  }
  ~foo() {
    std::cout << "destructor" << std::endl;
  }
};
typedef std::vector<con::shared_ptr<foo>> fools;

int main() {

  con::shared_ptr<int>  sp1;
  con::shared_ptr<int>  sp2;
  sp1 = sp2;

  fools fs;
  fs.reserve(10);

  for (size_t i = 0; i < 10; ++i) {
    // fs.push_back(con::make_shared<foo>());
    fs.push_back(con::shared_ptr<foo>(new foo()));
  }
}
