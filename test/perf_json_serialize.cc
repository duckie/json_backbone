#include <iostream>
#include <nested_container/container.hpp>
#include <nested_container/extensions/boost_spirit_json/json_forward.hpp>
#include <nested_container/test/random.hpp>
#include <vector>
#include <list>
#include <chrono>
#include <nested_container/externalize.hpp>
#include <nested_container/extensions/boost_spirit_json/externalize_json.hpp>

NESTED_CONTAINER_EXTERNALIZE(NESTED_CONTAINER_CONTAINER_SIGNATURE());
NESTED_CONTAINER_EXTERNALIZE_JSON(NESTED_CONTAINER_CONTAINER_SIGNATURE());

using nested_container::container;
using nested_container::vector_element_init;
using namespace nested_container::json;
using namespace nested_container::test::random;
template <typename C> using json_karma = serializer<C, typename C::str_type, generation_policies::visitor_partial_karma>;
template <typename C> using json_stream = serializer<C, typename C::str_type, generation_policies::visitor_ostream>;

void compare_dump_time(container const& c) {
  size_t constexpr max_iter = 1e1;
  using std::chrono::high_resolution_clock;
  using std::chrono::time_point;
  using std::chrono::duration_cast;
  using std::chrono::milliseconds;

  json_karma<container> karma_driver;
  json_stream<container> stream_driver;
  time_point<high_resolution_clock> start, end;
  // Fast
  start = high_resolution_clock::now();
  for(size_t i=0u; i < max_iter; ++i) karma_driver.serialize(c);
  end = high_resolution_clock::now();
  std::cout << "Partial karma test took " << duration_cast<milliseconds>(end-start).count() << "ms" << std::endl;

  // Generic
  start = high_resolution_clock::now();
  for(size_t i=0u; i < max_iter; ++i) stream_driver.serialize(c);
  end = high_resolution_clock::now();
  std::cout << "Sstream test took " << duration_cast<milliseconds>(end-start).count() << "ms" << std::endl;
}

int main(void) {
  {
    generator<container> random_gen;

    // Totally random
    compare_dump_time(random_gen.generate(5, 30, 60, 30, 60));

    // Only arrays of integer
    compare_dump_time(random_gen.generate(4, 0, 0, 50, 80, 0, 0, 0, 0, false, false, true, false, false, true, true, false));
    
    // Not any number
    compare_dump_time(random_gen.generate(4, 30, 50, 30, 50, 0, 12, 1, 12, true, true, true, true, false, false, false, true));
  }
  return 0;
}

