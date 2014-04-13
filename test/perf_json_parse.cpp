#include <iostream>
#include <nested_container/container.hpp>
#include <nested_container/json_forward.hpp>
#include <nested_container/test/random.hpp>
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
using namespace nested_container::json;
using namespace nested_container::test::random;

template <typename C> using json = nested_container::json::serializer<C>;
template <typename C> using json_full_spirit = serializer<C, typename C::str_type, generation_policies::visitor_ostream, parsing_policies::full_spirit>;
template <typename C> using json_partial_spirit = serializer<C, typename C::str_type, generation_policies::visitor_ostream, parsing_policies::partial_spirit>;

void compare_parse_time(container const& c) {
  size_t constexpr max_iter = 1e1;
  using std::chrono::high_resolution_clock;
  using std::chrono::time_point;
  using std::chrono::duration_cast;
  using std::chrono::milliseconds;

  json<container> serialize_driver;
  std::string to_parse(serialize_driver.serialize(c));
  //std::cout << to_parse << std::endl;
  //std::cout << to_parse.size() << std::endl;

  json_full_spirit<container> full_driver;
  json_partial_spirit<container> partial_driver;
  time_point<high_resolution_clock> start, end;
  // Fast
  start = high_resolution_clock::now();
  for(size_t i=0u; i < max_iter; ++i) full_driver.deserialize(to_parse);
  //container result_full = full_driver.deserialize(to_parse);
  //std::string result_full_str(serialize_driver.serialize(result_full));
  //std::cout << result_full_str.size() << std::endl;
  //std::cout << result_full_str << std::endl;
  end = high_resolution_clock::now();
  std::cout << "Full spirit test took " << duration_cast<milliseconds>(end-start).count() << "ms" << std::endl;

  // Generic
  start = high_resolution_clock::now();
  for(size_t i=0u; i < max_iter; ++i) partial_driver.deserialize(to_parse);
  end = high_resolution_clock::now();
  std::cout << "Partial spirit took " << duration_cast<milliseconds>(end-start).count() << "ms" << std::endl;
}

int main(void) {
  // Performance test
  //{
    //std::cout << "Begin test 8" << std::endl;
    //json<container> json_driver;
    //std::string input = R"json(["test1","test2",1.1,2,-4])json";
    //input = R"json(
    //{"test1":1,
      //"test2":1.1,
      //"test3":["test1","test2",1.1,2,-4],
      //"test4":true,  
         //"test5":null
  //, "test6":{}}
    //)json";
//
    //std::cout << "To deserialize: " << input << std::endl;
    ////std::string input = R"json([])json";
    //container c = json_driver.deserialize(input);
    //std::cout << json_driver.serialize(c) << std::endl;
    //container& c2 = c["test3"];
    //c = c["test3"];
    //
    //std::cout << c[1].is_string() << "\n" << c[2].is_float() << "\n" << c[3].is_uint() << "\n" << c[4].is_int() << std::endl;
  //}
//
  //return 0;

  generator<container> random_gen;

  // Totally random
  compare_parse_time(random_gen.generate(4, 30, 60, 30, 60));

  // Only arrays of integer
  //compare_parse_time(random_gen.generate(4, 0, 0, 50, 80, 0, 0, 0, 0, false, false, true, false, false, true, true, false));

  // Not any number
  //compare_parse_time(random_gen.generate(1, 30, 50, 30, 50, 0, 12, 1, 12, true, true, true, true, false, false, false, true));

  return 0;
}

