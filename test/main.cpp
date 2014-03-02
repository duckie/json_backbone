#include <nested_container.hpp>
#include <iostream>
#include <vector>
#include <list>

int main(void) {
  using nested_container::container;
  {
    container c1;
    std::cout << c1.is_null() << std::endl;
    container c2(std::string("Roger"));
    container c3(3.f);
    container c4(4);
    container c5(4u);
  }

  {
    container c1 = container::init<std::string>();
    std::cout << c1.is_int() << std::endl;
    std::cout << c1.is_string() << std::endl;

    std::string const roger(std::string("Marcel"));
    container c2(roger);
  }

  {
    int roger = 2;
    container c1(roger);
  }
  
  return 0;
}

