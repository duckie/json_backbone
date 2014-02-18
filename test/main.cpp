#include <nested_container.hpp>
#include <iostream>
#include <vector>
#include <list>

namespace nested_container {
extern template class basic_container<>;
}

int main(void) {
  using nested_container::container;
  {
    container c1;
    std::cout << c1.is_null() << std::endl;
    container c2("Roger");
    container c3(3.f);
    container c4(4);
    container c5(4u);
  }
  
  return 0;
}

