#ifndef __NESTED_COMPILER__
#define __NESTED_COMPILER__

#include <map>
#include <vector>
#include <type_traits>
#include <sstream>
#include <utility>
#include <cassert>
#include <iostream>

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
  typename Key = std::string
  , class String = std::string
  , typename Int = int
  , typename UInt = unsigned int
  , typename Float = float
  , template <typename InnerKey, class This> class MapTemplate = std_map_default_allocators
  , template <typename This> class VectorTemplate = std_vector_default_allocators
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
  using vector_type = VectorTemplate<basic_container>;

 private:
  using Map = map_type; 
  using Vector = vector_type;
  using Null = std::nullptr_t;

  enum class value_type : unsigned char  {
    null = '0'
    , map 
    , vector 
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
    Vector* vector_;
    String str_;
    Float float_;
    Int int_;
    UInt uint_;
  };

  template<typename A, typename B> using eq = std::is_same<A,B>;

  template <typename T> struct type_proxy {};
  template <bool B> struct bool_proxy {};

  template <typename Member> struct type_traits { 
    using T = typename std::remove_cv<typename std::remove_reference<Member>::type>::type;
    using pure_type = T;
    static constexpr value_type type_value() {
      return eq<Int, T>::value ? value_type::integer
        : eq<UInt, T>::value ? value_type::unsigned_integer
        : eq<Float, T>::value ? value_type::floating
        : eq<String, T>::value ? value_type::string 
        : eq<Vector, T>::value ? value_type::vector
        : eq<Map, T>::value ? value_type::map
        : value_type::null;
    }

    static bool constexpr is_from_container = eq<Int, T>::value 
        || eq<UInt, T>::value 
        || eq<Float, T>::value 
        || eq<String, T>::value 
        || eq<Vector, T>::value 
        || eq<Map, T>::value
        || eq<std::nullptr_t, T>::value;

    static bool constexpr is_collection = eq<Vector, T>::value || eq<Map, T>::value;
    static bool constexpr is_lexical = eq<Int, T>::value || eq<UInt, T>::value || eq<Float, T>::value || eq<String, T>::value;
    static bool constexpr is_null = eq<Int, T>::value || eq<UInt, T>::value || eq<Float, T>::value || eq<String, T>::value;
    static bool constexpr is_index = eq<Key, T>::value || eq<size_t, T>::value;

    static_assert(is_from_container, "Type must be one of container's internal types");
  };

  static bool is_lexical(value_type type) { 
    return value_type::integer == type
      || value_type::unsigned_integer == type
      || value_type::floating == type
      || value_type::string == type;
  }
      

  value_type type_ = value_type::null;
  value value_;

  void clear() {
    switch (type_) {
      case value_type::null:
        break; 
      case value_type::map:
        delete value_.dict_;
        break;
      case value_type::vector:
        delete value_.vector_;
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
  inline void init_member(type_proxy<Vector>) { value_.vector_ = new Vector; }
  inline void init_member(type_proxy<String>) { new (&(value_.str_)) String; }
  inline void init_member(type_proxy<Float>) { value_.float_ = 0.f; }
  inline void init_member(type_proxy<Int>) { value_.int_ = 0; }
  inline void init_member(type_proxy<UInt>) { value_.uint_ = 0u; }

  // Init with value - move
  inline void init_member(std::nullptr_t&&) {}
  inline void init_member(Map&& v) { value_.dict_ = new Map(v); }
  inline void init_member(Vector&& v) { value_.vector_ = new Vector(v); }
  inline void init_member(String&& v) { new (&(value_.str_)) String(v); }
  inline void init_member(Float&& v) { value_.float_ = v; }
  inline void init_member(Int&& v) { value_.int_ = v; }
  inline void init_member(UInt&& v) { value_.uint_ = v; }

  // Init with value - copy
  inline void init_member(std::nullptr_t const&) {}
  inline void init_member(Map const& v) { value_.dict_ = new Map(v); }
  inline void init_member(Vector const& v) { value_.vector_ = new Vector(v); }
  inline void init_member(String const& v) { new (&(value_.str_)) String(v); }
  inline void init_member(Float const& v) { value_.float_ = v; }
  inline void init_member(Int const& v) { value_.int_ = v; }
  inline void init_member(UInt const& v) { value_.uint_ = v; }

  // Runtime version
  void init_member(value_type target_type) {
    switch (target_type) {
      case value_type::null:
        break; 
      case value_type::map:
        init_member<Map>();
        break;
      case value_type::vector:
        init_member<Vector>();
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

  // Accessors, private, not protected against bad behavior, checks must be done before
  template <typename T> inline T* ptr_to() { return ptr_to(type_proxy<T>()); }
  inline Map* ptr_to(type_proxy<Map>) { return value_.dict_; }
  inline Vector* ptr_to(type_proxy<Vector>) { return value_.vector_; }
  inline String* ptr_to(type_proxy<String>) { return &value_.str_; }
  inline Float* ptr_to(type_proxy<Float>) { return &value_.float_; }
  inline Int* ptr_to(type_proxy<Int>) { return &value_.int_; }
  inline UInt* ptr_to(type_proxy<UInt>) { return &value_.uint_; }

  template <typename T> inline T const* ptr_to() const { return ptr_to(type_proxy<T>()); }
  inline Map const* ptr_to(type_proxy<Map>) const { return value_.dict_; }
  inline Vector const* ptr_to(type_proxy<Vector>) const { return value_.vector_; }
  inline String const* ptr_to(type_proxy<String>) const { return &value_.str_; }
  inline Float const* ptr_to(type_proxy<Float>) const { return &value_.float_; }
  inline Int const* ptr_to(type_proxy<Int>) const { return &value_.int_; }
  inline UInt const* ptr_to(type_proxy<UInt>) const { return &value_.uint_; }

  template <typename T> inline T& ref_to() { return ref_to(type_proxy<T>()); }
  inline Map& ref_to(type_proxy<Map>) { return *value_.dict_; }
  inline Vector& ref_to(type_proxy<Vector>) { return *value_.vector_; }
  inline String& ref_to(type_proxy<String>) { return value_.str_; }
  inline Float& ref_to(type_proxy<Float>) { return value_.float_; }
  inline Int& ref_to(type_proxy<Int>) { return value_.int_; }
  inline UInt& ref_to(type_proxy<UInt>) { return value_.uint_; }

  template <typename T> inline T const& ref_to() const { return ref_to(type_proxy<T>()); }
  inline Map const& ref_to(type_proxy<Map>) const { return *value_.dict_; }
  inline Vector const& ref_to(type_proxy<Vector>) const { return *value_.vector_; }
  inline String const& ref_to(type_proxy<String>) const { return value_.str_; }
  inline Float const& ref_to(type_proxy<Float>) const { return value_.float_; }
  inline Int const& ref_to(type_proxy<Int>) const { return value_.int_; }
  inline UInt const& ref_to(type_proxy<UInt>) const { return value_.uint_; }

  // [] accessors, Key != size_type version
  basic_container const& access_collection(bool_proxy<false>, Key const& index) const {
    if (!is_map()) return init<Null>();
    return ref_to<Map>()[index];
  }

  basic_container const& access_collection(bool_proxy<false>, size_t const& index) const {
    if (!is_vector()) return init<Null>();
    return ref_to<Vector>()[index];
  }

  basic_container& access_collection(bool_proxy<false>, Key const& index) {
    if (!is_map()) switch_to_type<Map>();
    return ref_to<Map>()[index];
  }

  basic_container& access_collection(bool_proxy<false>, size_t const& index) {
    if (!is_vector()) switch_to_type<Vector>();
    return ref_to<Vector>()[index];
  }

  // [] accessors, Key == size_type version
  basic_container& access_collection(bool_proxy<true>, size_t const& index) const {
    if (!is_map() && !is_vector()) return init<Null>();
    if (is_map()) return ref_to<Map>()[index];
    else return ref_to<Vector>()[index];
  }

  basic_container& access_collection(bool_proxy<true>, size_t const& index) {
    if (!is_map() && !is_vector()) switch_to_type<Map>();
    if (is_map()) return ref_to<Map>()[index];
    else return ref_to<Vector>()[index];
  }

  //template <typename T> convert_to

 public:
  basic_container() {};
  basic_container(basic_container const& c) { *this = c; }
  basic_container(basic_container&& c) { c = std::move(c); }
  basic_container& operator= (basic_container const& c) {
    switch_to_type(c.type_);
    switch (type_) {
      case value_type::null:
        break; 
      case value_type::map:
        *value_.dict_ = *c.value_.dict_;
        break;
      case value_type::vector:
        *value_.vector_ = *c.value_.vector_;
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
      case value_type::map:
        *value_.dict_ = std::move(*c.value_.dict_);
        break;
      case value_type::vector:
        *value_.vector_ = std::move(*c.value_.vector_);
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

  // Constructor from other types
  template <typename T> basic_container(T arg) : basic_container(type_proxy<typename type_traits<decltype(arg)>::pure_type>(), arg) {}
  
  // Handling char* case
  basic_container(typename str_type::value_type const* arg) : basic_container(type_proxy<str_type>(), str_type(arg)) {}

  ~basic_container() { clear(); }  // virtual not needed, this class is final

  // Helper to initialize the container to a given type
  template <typename T> static basic_container init() { 
    return basic_container(type_proxy<T>());
  }

  template <typename T> T as() { static_assert(type_traits<T>::is_from_container,""); }

  inline bool is_null() const { return value_type::null == type_; }
  inline bool is_map() const { return value_type::map == type_; }
  inline bool is_vector() const { return value_type::vector == type_; }
  inline bool is_string() const { return value_type::string == type_; }
  inline bool is_float() const { return value_type::floating == type_; }
  inline bool is_int() const { return value_type::integer == type_; }
  inline bool is_uint() const { return value_type::unsigned_integer == type_; }

  template <typename T> T const* get() const { 
    return type_traits<T>::type_value() == type_ ? ptr_to<T>() : nullptr;
  }

  template <typename T> bool get(T& output_value) const { 
    if (type_traits<T>::type_value() == type_) {
      output_value = ref_to<T>();
      return true;
    }
    return false;
  }

  template <typename T> basic_container const& operator[] (T const& index) const {
    return access_collection(bool_proxy<eq<Key, size_t>::value>(), index);
  }

  template <typename T> basic_container& operator[] (T const& index) {
    return access_collection(bool_proxy<eq<Key, size_t>::value>(), index);
  }

  // Conversion.
  template <typename T> operator T () const {
    static_assert(type_traits<T>::is_from_container,"");
    value_type target_type = type_traits<T>::type_value();
    if (target_type == type_) {
      return ref_to<T>();
    }
    else if (is_lexical(target_type) && is_lexical(type_)) {
      if (value_type::string == type_ || value_type::string == target_type) {  // lexical cast
        assert(value_type::map != type_);
        switch (type_) {
          case value_type::string:
            return lexical_cast<T>(ref_to<String>());
          case value_type::floating:
            return lexical_cast<T>(ref_to<Float>());
          case value_type::integer:
            return lexical_cast<T>(ref_to<Int>());
          case value_type::unsigned_integer:
            return lexical_cast<T>(ref_to<UInt>());
          default:
            break;
        }
      }
      else {  // static cast
        switch (type_) {
          case value_type::floating:
            return static_cast<T>(ref_to<Float>());
          case value_type::integer:
            return static_cast<T>(ref_to<Int>());
          case value_type::unsigned_integer:
            return static_cast<T>(ref_to<UInt>());
          default:
            break;
        }
      }
    }
    return T();
  }
};


using container = basic_container<>;
};  // namespace nested_container

#endif  // __NESTED_COMPILER__
