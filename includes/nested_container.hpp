#ifndef __NESTED_COMPILER__
#define __NESTED_COMPILER__

#include <map>
#include <vector>

namespace nested_container {

template <typename key_type, typename value_type> using std_map_default_allocators = std::map<key_type, value_type>;
template <typename value_type> using std_vector_default_allocators = std::vector<value_type>;

template <
  class key_type = std::string
  , class str_type = std::string
  , typename int_type = int
  , typename uint_type = unsigned int
  , typename float_type = float
  , template <typename inner_key_type, class this_type> class map_type_template = std_map_default_allocators
  , template <typename this_type> class array_type_template = std_vector_default_allocators
  >
class basic_container final {
  using map_type = map_type_template<key_type, basic_container>;
  using array_type = array_type_template<basic_container>;

  enum class value_type : unsigned char  {
    null = '0'
    , dictionary
    , array
    , string
    , floating
    , integer
    , unsigned_integer
  };

  union value {
    map_type dict_;
    array_type array_;
    str_type str_;
    float_type float_;
    int_type int_;
    uint_type uint_;
  };

  value_type type_ = value_type::null;
  value value_;

 public:
  basic_container() = default;
  basic_container(basic_container const&) = default;
  basic_container(basic_container&&) = default;

  //basic_container(str_type const& 


  inline bool is_null() const { return value_type::null == type_; }
  inline bool is_dictionary() const { return value_type::dictionary == type_; }
  inline bool is_array() const { return value_type::array == type_; }
  inline bool is_string() const { return value_type::string == type_; }
  inline bool is_float() const { return value_type::floating == type_; }
  inline bool is_int() const { return value_type::integer == type_; }
  inline bool is_uint() const { return value_type::unsigned_integer == type_; }
};

using container = basic_container<>;
};  // namespace nested_container

#endif  // __NESTED_COMPILER__
