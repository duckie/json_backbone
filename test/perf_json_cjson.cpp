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
  {
    std::cout << "Begin test 7" << std::endl;
    container c = container::init_vec({});
    size_t nb_elements = 1e6;
    for(size_t i=0u; i < nb_elements; ++i) {
      c.raw_vector().push_back(static_cast<unsigned int>(i));
    }
    
    json<container> json_sstream_driver;
    json_cstring<container> json_driver;

    size_t constexpr max_iter = 1e1;
    using std::chrono::high_resolution_clock;
    using std::chrono::time_point;
    using std::chrono::duration_cast;
    using std::chrono::milliseconds;

    time_point<high_resolution_clock> start, end;

    // Sstream
    start = high_resolution_clock::now();
    for(size_t i=0u; i < max_iter; ++i) json_sstream_driver.serialize(c);
    end = high_resolution_clock::now();
    std::cout << "Sstream test took " << duration_cast<milliseconds>(end-start).count() << "ms" << std::endl;

    // Fast
    start = high_resolution_clock::now();
    for(size_t i=0u; i < max_iter; ++i) json_driver.serialize(c);
    end = high_resolution_clock::now();
    std::cout << "Printf test took " << duration_cast<milliseconds>(end-start).count() << "ms" << std::endl;
  }
  return 0;
}

