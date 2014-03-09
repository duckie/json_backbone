#include <nested_container.hpp>
#include <iostream>
#include <vector>
#include <list>

std::string str1() {
  return "Roger";
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

    //std::string roger = c2;
    std::cout << c2.is_string() << std::endl;
  }

  {
    container c1 = container::init<std::string>();
    std::cout << c1.is_int() << std::endl;
    std::cout << c1.is_string() << std::endl;

    std::string const roger(std::string("Marcel"));
    container c2(roger);
  }

  {
    std::cout << "Begin test 3" << std::endl;
    int roger = 2;
    container c1(roger);

    c1["value1"] = 1;
    c1["value2"] = "123";
    c1["value3"] = 1.f;
    c1["value4"]["s_value1"] = "Hello subsection";
    c1["value4"]["s_value2"] = true;

    int value2 = c1["value2"];
    unsigned int value3 = c1["value3"];
    std::cout << value2 << std::endl;
    std::cout << value3 << std::endl;
    if (c1["value4"]["s_value2"]) {
      std::cout << c1["value4"]["s_value1"].ref_string() << std::endl;
    }

    float& test1 = c1["value2"].transform_float();
    std::cout << test1 << std::endl;
    test1 = 2.f;  // Modifies the underlying container !
    std::cout << c1["value2"].ref_float() << std::endl;
  }
  
  {
    std::cout << "Begin test 4" << std::endl;
    container c1(str1());
  }

  {
    std::cout << "Begin test 5" << std::endl;
    container c2(1);
    //std::vector<container> t = c2.as_vector();
    std::vector<container> t;
    t.push_back(c2);
    container c3 = c2;
    //container c1 = {{ container(1), container(2), container(4) }};
    //int& t1 = c2.ref_int();
    //for(container& elem : c2.as_vector()) {
      //std::cout << elem.as_int() << std::endl;
    //}
    //container c1 = {{ {"Roger",1},{"Marcel",2},{"Ole",4} }};
    //for(std::pair<int,std::string> const& elem : c1.as_map()) {
      //std::cout << elem.first << ":" << elem.second << std::endl;
    //}
  }
  return 0;
}

