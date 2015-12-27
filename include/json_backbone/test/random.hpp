#ifndef HEADER_GUARD_NESTED_COMPILER_TEST_RANDOM
#define HEADER_GUARD_NESTED_COMPILER_TEST_RANDOM

#include <random>
#include <list>
#include <functional>
#include <sstream>
#include <cassert>
#include <set>
#include <utility>

// This header is a tool to generate randomized containers

namespace json_backbone {
namespace test {
namespace random {

template <typename Container> class generator {
  using str_type      = typename Container::str_type;
  using ostream_type  = std::basic_ostringstream<typename str_type::value_type, typename str_type::traits_type>;
  using key_type      = typename Container::key_type;
  using object_type      = typename Container::object_type;
  using array_type   = typename Container::array_type;
  using float_type    = typename Container::float_type;
  using int_type      = typename Container::int_type;
  using uint_type     = typename Container::uint_type;

  static int constexpr to_null = 0;
  static int constexpr to_map = 1;
  static int constexpr to_vec = 2;
  static int constexpr to_str = 3;
  static int constexpr to_float = 4;
  static int constexpr to_int = 5;
  static int constexpr to_uint = 6;
  static int constexpr to_bool  = 7;

  std::string const alnums = "abcdefghijklmnopqrstuvwxyz0123456789";
  size_t const alnums_size = 36u;
  std::random_device rd_;
  std::mt19937 gen_;
  std::discrete_distribution<> bool_chooser_ = {1, 1};
  std::uniform_int_distribution<unsigned int> char_chooser_ = std::uniform_int_distribution<unsigned int>(0u, alnums_size - 1u);
  std::uniform_int_distribution<int_type> int_chooser_ = std::uniform_int_distribution<int_type>(std::numeric_limits<int_type>::lowest(), std::numeric_limits<int_type>::max());
  std::uniform_int_distribution<uint_type> uint_chooser_ = std::uniform_int_distribution<uint_type>(std::numeric_limits<uint_type>::lowest(), std::numeric_limits<uint_type>::max());
  std::uniform_real_distribution<float_type> float_chooser_ = std::uniform_real_distribution<float_type>(
      std::numeric_limits<float_type>::lowest()/2
      , std::numeric_limits<float_type>::max()/2);

  str_type generate_string(size_t size) {
    ostream_type output;
    for(size_t index = 0u; index < size; ++index) output << alnums.at(char_chooser_(gen_));
    return output.str();
  }

  Container generate_container_once(int type, std::uniform_int_distribution<>& str_gen) {
    if (to_map == type) return Container::template init<typename Container::object_type>();
    else if (to_vec == type) return Container::template init<typename Container::array_type>();
    else if (to_str == type) return Container(generate_string(str_gen(gen_)));
    else if (to_str == type) return Container("Allloo");
    else if (to_float == type) return float_chooser_(gen_);
    else if (to_int == type) return int_chooser_(gen_);
    else if (to_uint == type) return uint_chooser_(gen_);
    else if (to_bool == type) return (bool_chooser_(gen_) ? true : false);  // Important to state true or false here, else the container will get an int
    return Container();
  }

 public:
  generator() {
    gen_ = std::mt19937(rd_());
  }

  Container generate(
    size_t max_depth = 0u
    , size_t min_map_size = 1u
    , size_t max_map_size = 100u
    , size_t min_array_size = 1u
    , size_t max_array_size = 100u
    , size_t min_string_size = 0u
    , size_t max_string_size = 12u
    , size_t min_key_size = 1u
    , size_t max_key_size = 12u
    , bool include_null = true
    , bool include_map = true
    , bool include_vector = true
    , bool include_string = true
    , bool include_float = true
    , bool include_int = true
    , bool include_uint = true
    , bool include_bool = true

    ){

    std::discrete_distribution<> type_chooser = {
      include_null ? 1.: 0.
        , include_map ? 1. : 0.
        , include_vector ? 1. : 0.
        , include_string ? 1. : 0.
        , include_float ? 1. : 0.
        , include_int ? 1. : 0.
        , include_uint ? 1. : 0.
        , include_bool ? 1. : 0.
    };
    std::discrete_distribution<> root_chooser = {(include_map ? 1. : 0.), (include_vector ? 1. : 0.)};
    Container root(root_chooser(gen_) ? Container::template init<typename Container::array_type>() : Container::template init<typename Container::object_type>());

    if(max_depth) {
      std::uniform_int_distribution<> map_size_gen(min_map_size, max_map_size);
      std::uniform_int_distribution<> vec_size_gen(min_array_size, max_array_size);
      std::uniform_int_distribution<> str_size_gen(min_string_size, max_string_size);
      std::uniform_int_distribution<> key_size_gen(min_key_size, max_key_size);


      std::list< std::reference_wrapper<Container> > to_process;
      std::list< std::reference_wrapper<Container> > next_to_process;
    
      to_process.push_back(root);

      while(max_depth) {

        while(to_process.size()) {
          Container & current = to_process.front().get();
          if(current.is_map()) {
            std::set<typename container::key_type> used_keys;
            size_t size = map_size_gen(gen_);
            for (size_t index = 0u; index < size; ++index) {
              typename container::key_type new_key(generate_string(key_size_gen(gen_)));
              auto insertion_result = used_keys.insert(new_key);
              if (!insertion_result.second) {  // Key already used
                --index;
                continue;
              }

              auto container_insert_result = current.raw_map().insert(std::make_pair(new_key, generate_container_once(type_chooser(gen_), str_size_gen)));
              assert(container_insert_result.second);
              Container& inserted = container_insert_result.first->second;
              if (inserted.is_map() || inserted.is_vector()) next_to_process.emplace_back(inserted);
            }
          }
          else if (current.is_vector()) {
            size_t size = vec_size_gen(gen_);
            current.ref_vector().reserve(size);
            for (size_t index = 0u; index < size; ++index) {
              current.ref_vector().push_back(generate_container_once(type_chooser(gen_), str_size_gen));
              Container& inserted = current.raw_vector().back();
              if (inserted.is_map() || inserted.is_vector()) next_to_process.emplace_back(inserted);
            }
          }
          else {
            assert(false);  // Should never happen
          }
          to_process.pop_front();
        }
        
        std::swap(to_process, next_to_process);
        --max_depth;
      }
    }

    return root;
  }
};



}  // namespace json_backbone::test::random
}  // namespace json_backbone::test
}  // namespace json_backbone

#endif  // HEADER_GUARD_NESTED_COMPILER_TEST_RANDOM
