#ifndef __NESTED_COMPILER__
#define __NESTED_COMPILER__

#include <map>
#include <vector>
#include <type_traits>
#include <sstream>
#include <utility>

namespace nested_container {

// boost lexical cast, simple non-throwing version
template<typename Target, typename Source> Target lexical_cast(Source arg) {
  std::stringstream interpreter;
  Target result;

  if(!(interpreter << arg)
      || !(interpreter >> result)
      || !(interpreter >> std::ws).eof())
    return Target();

  return result;
}

template <typename key_type, typename value_type> using std_map_default_allocators = std::map<key_type, value_type>;
template <typename value_type> using std_vector_default_allocators = std::vector<value_type>;

template <
  class Key = std::string
  , class String = std::string
  , typename Int = int
  , typename UInt = unsigned int
  , typename Float = float
  , template <typename InnerKey, class This> class MapTemplate = std_map_default_allocators
  , template <typename This> class ArrayTemplate = std_vector_default_allocators
  >
class basic_container final {
 public:
  // Public type declarations
  using key_type = Key;
  using str_type = String;
  using int_type = Int;
  using uint_type = UInt;
  using float_type = Float;
  using map_type = MapTemplate<key_type, basic_container>;
  using array_type = ArrayTemplate<basic_container>;

 private:
  using Map = map_type; 
  using Array = array_type;

  enum class value_type : unsigned char  {
    null = '0'
    , dictionary
    , array
    , string
    , floating
    , integer
    , unsigned_integer
    , boolean
  };

  union value final {
    value() {}
    ~value() {}

    Map* dict_;
    Array* array_;
    String str_;
    Float float_;
    Int int_;
    UInt uint_;
  };

  template<typename A, typename B> using eq = std::is_same<A,B>;

  template <typename T> struct type_proxy {};
  template <bool Value> struct bool_proxy {};

  template <typename T> struct is_from_container {
    static bool constexpr value = eq<Int, T>::value 
        || eq<UInt, T>::value 
        || eq<Float, T>::value 
        || eq<String, T>::value 
        || eq<Array, T>::value 
        || eq<Map, T>::value
        || eq<std::nullptr_t, T>::value;
  };

  template <typename Member> struct type_traits { 
    using T = typename std::remove_cv<typename std::remove_reference<Member>::type>::type;
    using pure_type = T;
    static_assert(is_from_container<T>::value, "Type must be one of container's internal types");
    static constexpr value_type type_value() {
      return eq<Int, T>::value ? value_type::integer
        : eq<UInt, T>::value ? value_type::unsigned_integer
        : eq<Float, T>::value ? value_type::floating
        : eq<String, T>::value ? value_type::string 
        : eq<Array, T>::value ? value_type::array
        : eq<Map, T>::value ? value_type::dictionary
        : value_type::null;
    }
  };

  value_type type_ = value_type::null;
  value value_;

  void clear() {
    switch (type_) {
      case value_type::null:
        break; 
      case value_type::dictionary:
        delete value_.dict_;
        break;
      case value_type::array:
        delete value_.array_;
        break;
      case value_type::string:
        value_.str_.~str_type();
        break;
      case value_type::floating:
        break;
      case value_type::integer:
        break;
      case value_type::unsigned_integer:
        break;
      default:
        break;
    }
  }

  // Compile time initializers
  template <typename T> inline void init_member() { init_member(type_proxy<T>()); }
  inline void init_member(type_proxy<std::nullptr_t>) {}
  inline void init_member(type_proxy<Map>) { value_.dict_ = new Map; }
  inline void init_member(type_proxy<Array>) { value_.array_ = new Array; }
  inline void init_member(type_proxy<String>) { new (&value_.str_) String; }
  inline void init_member(type_proxy<Float>) { value_.float_ = 0.f; }
  inline void init_member(type_proxy<Int>) { value_.int_ = 0; }
  inline void init_member(type_proxy<UInt>) { value_.uint_ = 0u; }

  // Init with value - move
  inline void init_member(std::nullptr_t&&) {}
  inline void init_member(Map&& v) { value_.dict_ = new Map(std::move(v)); }
  inline void init_member(Array&& v) { value_.array_ = new Array(std::move(v)); }
  inline void init_member(String&& v) { new (&value_.str_) String(std::move(v)); }
  inline void init_member(Float&& v) { value_.float_ = v; }
  inline void init_member(Int&& v) { value_.int_ = v; }
  inline void init_member(UInt&& v) { value_.uint_ = v; }

  // Init with value - copy
  inline void init_member(std::nullptr_t const&) {}
  inline void init_member(Map const& v) { value_.dict_ = new Map(v); }
  inline void init_member(Array const& v) { value_.array_ = new Array(v); }
  inline void init_member(String const& v) { new (&value_.str_) String(v); }
  inline void init_member(Float const& v) { value_.float_ = v; }
  inline void init_member(Int const& v) { value_.int_ = v; }
  inline void init_member(UInt const& v) { value_.uint_ = v; }

  // Runtime version
  void init_member(value_type target_type) {
    switch (type_) {
      case value_type::null:
        break; 
      case value_type::dictionary:
        init_member<Map>();
        break;
      case value_type::array:
        init_member<Array>();
        break;
      case value_type::string:
        init_member<String>();
        break;
      case value_type::floating:
        init_member<Float>();
        break;
      case value_type::integer:
        init_member<Int>();
        break;
      case value_type::unsigned_integer:
        init_member<UInt>();
        break;
      default:
        break;
    }
  }

  // When target type is known at compilation
  template <typename T> void switch_to_type() {
    value_type constexpr target_type = type_traits<T>::type_value();
    if (target_type == type_) return;
    clear();
    init_member<T>();
    type_ = target_type;
  }

  // When target type is known at runtime only
  void switch_to_type(value_type target_type) {
    if (target_type == type_) return;
    clear();
    init_member(target_type);
    type_ = target_type;
  }

  template <typename T> basic_container(type_proxy<T>) : type_(type_traits<T>::type_value()) { 
    init_member(type_proxy<T>());
  }

  template <typename T> basic_container(type_proxy<typename type_traits<T>::pure_type>, T&& arg) : type_(type_traits<T>::type_value()) { 
    init_member(std::forward<T>(arg));
  }

  // Accessors
  template <typename T> inline T* ptr_to() { return ptr_to(type_proxy<T>()); }
  inline Map* ptr_to(type_proxy<Map>) { return value_.dict_; }
  inline Array* ptr_to(type_proxy<Array>) { return value_.array_; }
  inline String* ptr_to(type_proxy<String>) { return &value_.str_; }
  inline Float* ptr_to(type_proxy<Float>) { return &value_.float_; }
  inline Int* ptr_to(type_proxy<Int>) { return &value_.int_; }
  inline UInt* ptr_to(type_proxy<UInt>) { return &value_.uint_; }

  template <typename T> inline T& ref_to() { return ref_to(type_proxy<T>()); }
  inline Map& ref_to(type_proxy<Map>) { return *value_.dict_; }
  inline Array& ref_to(type_proxy<Array>) { return *value_.array_; }
  inline String& ref_to(type_proxy<String>) { return value_.str_; }
  inline Float& ref_to(type_proxy<Float>) { return value_.float_; }
  inline Int& ref_to(type_proxy<Int>) { return value_.int_; }
  inline UInt& ref_to(type_proxy<UInt>) { return value_.uint_; }

  template <typename T> basic_container& private_at(bool_proxy<T>, Key const&);

  // Accessors for when Key != size_t
  template<> basic_container& private_at(bool_proxy<false>, typename std::enable_if<eq<Key,size_t>::value,Key const&>::type key) {
    if (value_type::dictionary != type_) switch_to_type<Map>();
    return value_.dict_->operator[](key);
  }

  // Accessors for when Key == size_t
  template <> basic_container& private_at (bool_proxy<true>, typename std::enable_if<eq<Key,size_t>::value,Key const&>::type key) {
    if (value_type::dictionary != type_ && value_type::array != type_) switch_to_type<Map>();
    if (value_type::array == type_) return value_.array_->operator[](key);
    return value_.dict_->operator[](key);
  }

 public:
  basic_container() {};
  basic_container(basic_container const& c) { *this = c; }
  basic_container(basic_container&& c) { c = std::move(c); }
  basic_container& operator= (basic_container const& c) {
    switch_to_type(c.type_);
    switch (type_) {
      case value_type::null:
        break; 
      case value_type::dictionary:
        *value_.dict_ = *c.value_.dict_;
        break;
      case value_type::array:
        *value_.array_ = *c.value_.array_;
        break;
      case value_type::string:
        value_.str_ = c.value_.str_;
        break;
      case value_type::floating:
        value_.float_ = c.value_.float_;
        break;
      case value_type::integer:
        value_.int_ = c.value_.int_;
        break;
      case value_type::unsigned_integer:
        value_.uint_ = c.value_.uint_;
        break;
      default:
        break;
    }
    return *this;
  }
  basic_container& operator= (basic_container&& c) {
    switch_to_type(c.type_);
    switch (type_) {
      case value_type::null:
        break; 
      case value_type::dictionary:
        *value_.dict_ = std::move(*c.value_.dict_);
        break;
      case value_type::array:
        *value_.array_ = std::move(*c.value_.array_);
        break;
      case value_type::string:
        value_.str_ = std::move(c.value_.str_);
        break;
      case value_type::floating:
        value_.float_ = c.value_.float_;
        break;
      case value_type::integer:
        value_.int_ = c.value_.int_;
        break;
      case value_type::unsigned_integer:
        value_.uint_ = c.value_.uint_;
        break;
      default:
        break;
    }
    return *this;
  }

  // Specifying those without a template is mandatory
  // All have the same code, only signature changes;
  basic_container(std::nullptr_t&& arg)       : basic_container(type_proxy<typename type_traits<decltype(arg)>::pure_type>(), arg) {}
  basic_container(Map&& arg)                  : basic_container(type_proxy<typename type_traits<decltype(arg)>::pure_type>(), arg) {}
  basic_container(Array&& arg)                : basic_container(type_proxy<typename type_traits<decltype(arg)>::pure_type>(), arg) {}
  basic_container(String&& arg)               : basic_container(type_proxy<typename type_traits<decltype(arg)>::pure_type>(), arg) {}
  basic_container(Float&& arg)                : basic_container(type_proxy<typename type_traits<decltype(arg)>::pure_type>(), arg) {}
  basic_container(Int&& arg)                  : basic_container(type_proxy<typename type_traits<decltype(arg)>::pure_type>(), arg) {}
  basic_container(UInt&& arg)                 : basic_container(type_proxy<typename type_traits<decltype(arg)>::pure_type>(), arg) {}
  basic_container(std::nullptr_t const& arg)  : basic_container(type_proxy<typename type_traits<decltype(arg)>::pure_type>(), arg) {}
  basic_container(Map const& arg)             : basic_container(type_proxy<typename type_traits<decltype(arg)>::pure_type>(), arg) {}
  basic_container(Array const& arg)           : basic_container(type_proxy<typename type_traits<decltype(arg)>::pure_type>(), arg) {}
  basic_container(String const& arg)          : basic_container(type_proxy<typename type_traits<decltype(arg)>::pure_type>(), arg) {}
  basic_container(Float const& arg)           : basic_container(type_proxy<typename type_traits<decltype(arg)>::pure_type>(), arg) {}
  basic_container(Int const& arg)             : basic_container(type_proxy<typename type_traits<decltype(arg)>::pure_type>(), arg) {}
  basic_container(UInt const& arg)            : basic_container(type_proxy<typename type_traits<decltype(arg)>::pure_type>(), arg) {}

  ~basic_container() { clear(); }  // virtual not needed, this class is final

  // Helper to initialize the container to a given type
  template <typename T> static basic_container init() { 
    return basic_container(type_proxy<T>());
  }

  template <typename T> T as() { 
    static_assert(is_from_container<T>::value, "as() must be used with a type of the container.");
  }

  inline bool is_null() const { return value_type::null == type_; }
  inline bool is_dictionary() const { return value_type::dictionary == type_; }
  inline bool is_array() const { return value_type::array == type_; }
  inline bool is_string() const { return value_type::string == type_; }
  inline bool is_float() const { return value_type::floating == type_; }
  inline bool is_int() const { return value_type::integer == type_; }
  inline bool is_uint() const { return value_type::unsigned_integer == type_; }

  template <typename T> T const* get() const { 
    return type_traits<T>::type_value() == type_ ? ptr_to<T>() : nullptr;
  }

  basic_container& operator[] (Key const& key) {
    return private_at(bool_proxy<eq<Key,size_t>::value>(), key);
  }
  
  basic_container& operator[] (typename std::enable_if<!eq<Key,size_t>::value, size_t>::type index) {
    if (value_type::array != type_) switch_to_type<Array>();
    return value_.array_->operator[](index);
  }
};

using container = basic_container<>;
};  // namespace nested_container

#endif  // __NESTED_COMPILER__
