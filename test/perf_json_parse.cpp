#include <iostream>
#include <nested_container/container.hpp>
#include <nested_container/json_forward.hpp>
#include <nested_container/experimental/cjson.hpp>
#include <vector>
#include <list>
#include <chrono>

#define NESTED_COMPILER_USE_EXTERNALIZATION_MACROS
#include <nested_container/externalize.hpp>
NESTED_COMPILER_EXTERNALIZE(basic_container);

#define NESTED_COMPILER_JSON_USE_EXTERNALIZATION_MACROS
#include <nested_container/externalize_json.hpp>
NESTED_COMPILER_JSON_EXTERNALIZE(basic_container);

using nested_container::container;
template <typename C> using json = nested_container::json::serializer<C>;
template <typename C> using json_cstring = nested_container::experimental::cjson::serializer<C>;

int main(void) {
  // Performance test
  {
    std::cout << "Begin test 8" << std::endl;
    json<container> json_driver;
    std::string input = R"json(["test1","test2",1.1,2,-4])json";
    input = R"json(
    {"test1":1,
      "test2":1.1,
      "test3":["test1","test2",1.1,2,-4],
      "test4":true,  
         "test5":null
  , "test6":{}}
    )json";

    std::cout << "To deserialize: " << input << std::endl;
    //std::string input = R"json([])json";
    container c = json_driver.deserialize(input);
    std::cout << json_driver.serialize(c) << std::endl;
    container& c2 = c["test3"];

    c = c["test3"];
    
    std::cout << c[1].is_string() << "\n" << c[2].is_float() << "\n" << c[3].is_uint() << "\n" << c[4].is_int() << std::endl;
  }
  return 0;
}

