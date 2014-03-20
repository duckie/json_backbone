#ifndef __NESTED_COMPILER__
#define __NESTED_COMPILER__

#include <map>
#include <vector>
#include <type_traits>
#include <sstream>
#include <utility>
#include <cassert>
#include <exception>
#include <initializer_list>
#include <tuple>
#include <typeinfo>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cfloat>
#include <limits>

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

  class exception_type : std::exception {
    const char* what() const noexcept { return "NestedContainer exception."; }
  };

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
    bool bool_;
  };

  template<typename A, typename B> using eq = std::is_same<A,B>;

  template <typename T> struct type_proxy {};
  template <bool B> struct bool_proxy {};

  template <typename Member> struct type_traits { 
    using T = typename std::remove_cv<typename std::remove_reference<Member>::type>::type;
    using pure_type = T;
    static constexpr value_type type_value() {
      return eq<bool, T>::value ? value_type::boolean 
        : eq<Int, T>::value ? value_type::integer
        : eq<UInt, T>::value ? value_type::unsigned_integer
        : eq<Float, T>::value ? value_type::floating
        : eq<String, T>::value ? value_type::string 
        : eq<Vector, T>::value ? value_type::vector
        : eq<Map, T>::value ? value_type::map
        : value_type::null;
    }

    static bool constexpr is_from_container = eq<bool, T>::value
        || eq<Int, T>::value 
        || eq<UInt, T>::value 
        || eq<Float, T>::value 
        || eq<String, T>::value 
        || eq<Vector, T>::value 
        || eq<Map, T>::value
        || eq<std::nullptr_t, T>::value
        || eq<basic_container, T>::value;

    static bool constexpr is_collection = eq<Vector, T>::value || eq<Map, T>::value;
    static bool constexpr is_lexical = eq<bool, T>::value || eq<Int, T>::value || eq<UInt, T>::value || eq<Float, T>::value || eq<String, T>::value;
    static bool constexpr is_null = eq<Null, T>::value;
    static bool constexpr is_index = eq<Key, T>::value || eq<size_t, T>::value;
    static bool constexpr is_pure = eq<T, Member>::value;

    static_assert(is_from_container, "Type must be one of container's internal types");
  };

  static bool is_lexical(value_type type) { 
    return value_type::boolean == type 
      || value_type::integer == type
      || value_type::unsigned_integer == type
      || value_type::floating == type
      || value_type::string == type;
  }
      
  value_type type_ = value_type::null;
  value value_;

  void clear() {
    switch (type_) {
      case value_type::map:
        delete value_.dict_;
        break;
      case value_type::vector:
        delete value_.vector_;
        break;
      case value_type::string:
        value_.str_.~str_type();
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
  inline void init_member(type_proxy<bool>) { value_.bool_ = false; }

  // Init with value -  trivial types
  inline void init_member(std::nullptr_t) {}
  inline void init_member(Float v) { value_.float_ = v; }
  inline void init_member(Int v) { value_.int_ = v; }
  inline void init_member(UInt v) { value_.uint_ = v; }
  inline void init_member(bool v) { value_.bool_ = v; }

  // Init with value - move
  inline void init_member(Map&& v) { value_.dict_ = new Map(v); }
  inline void init_member(Vector&& v) { value_.vector_ = new Vector(v); }
  inline void init_member(String&& v) { new (&(value_.str_)) String(v); }
  inline void init_member(basic_container&& c) {
    switch_to_type(c.type_);
    switch (type_) {
      case value_type::null:
        break; 
      case value_type::map:
        value_.dict_ = c.value_.dict_;
        c.value_.dict_ = nullptr;  // Useless in theory, just in case
        break;
      case value_type::vector:
        value_.vector_ = c.value_.vector_;
        c.value_.vector_ = nullptr;  // Useless in theory, just in case
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
      case value_type::boolean:
        value_.bool_ = c.value_.bool_;
        break;
      default:
        break;
    }

    c.type_ = value_type::null;
  }

  // Init with value - copy
  inline void init_member(Map const& v) { value_.dict_ = new Map(v); }
  inline void init_member(Vector const& v) { value_.vector_ = new Vector(v); }
  inline void init_member(String const& v) { new (&(value_.str_)) String(v); }
  inline void init_member(basic_container const& c) {
    // To replace with a switchtype with value
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
      case value_type::boolean:
        value_.bool_ = c.value_.bool_;
        break;
      default:
        break;
    }
  }

  // Runtime version
  void init_member(value_type target_type) {
    switch (target_type) {
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
      case value_type::boolean:
        init_member<bool>();
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

  template <typename T> void switch_to_type(T&& value) {
    value_type constexpr target_type = type_traits<T>::type_value();
    if (target_type == type_) {
      ref_to<T>() = std::forward<T>(value);
      return;
    }
    clear();
    init_member(std::forward<T>(value));
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
  inline bool* ptr_to(type_proxy<bool>) { return &value_.bool_; }

  template <typename T> inline T const* ptr_to() const { return ptr_to(type_proxy<T>()); }
  inline Map const* ptr_to(type_proxy<Map>) const { return value_.dict_; }
  inline Vector const* ptr_to(type_proxy<Vector>) const { return value_.vector_; }
  inline String const* ptr_to(type_proxy<String>) const { return &value_.str_; }
  inline Float const* ptr_to(type_proxy<Float>) const { return &value_.float_; }
  inline Int const* ptr_to(type_proxy<Int>) const { return &value_.int_; }
  inline UInt const* ptr_to(type_proxy<UInt>) const { return &value_.uint_; }
  inline bool const* ptr_to(type_proxy<bool>) const { return &value_.bool_; }

  template <typename T> inline T& ref_to() { return ref_to(type_proxy<T>()); }
  inline Map& ref_to(type_proxy<Map>) { return *value_.dict_; }
  inline Vector& ref_to(type_proxy<Vector>) { return *value_.vector_; }
  inline String& ref_to(type_proxy<String>) { return value_.str_; }
  inline Float& ref_to(type_proxy<Float>) { return value_.float_; }
  inline Int& ref_to(type_proxy<Int>) { return value_.int_; }
  inline UInt& ref_to(type_proxy<UInt>) { return value_.uint_; }
  inline bool& ref_to(type_proxy<bool>) { return value_.bool_; }

  template <typename T> inline T const& ref_to() const { return ref_to(type_proxy<T>()); }
  inline Map const& ref_to(type_proxy<Map>) const { return *value_.dict_; }
  inline Vector const& ref_to(type_proxy<Vector>) const { return *value_.vector_; }
  inline String const& ref_to(type_proxy<String>) const { return value_.str_; }
  inline Float const& ref_to(type_proxy<Float>) const { return value_.float_; }
  inline Int const& ref_to(type_proxy<Int>) const { return value_.int_; }
  inline UInt const& ref_to(type_proxy<UInt>) const { return value_.uint_; }
  inline bool const& ref_to(type_proxy<bool>) const { return value_.bool_; }

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
  template <typename T> T convert_to(type_proxy<T>) const { 
    static_assert(type_traits<T>::is_pure, "Type must not be a reference nor have cv-qualifiers");
    value_type target_type = type_traits<T>::type_value();
    if (target_type == type_) {
      return ref_to<T>();
    }
    else if (is_lexical(target_type) && is_lexical(type_)) {
      if (value_type::string == type_) {  // lexical cast
        return lexical_cast<T>(ref_to<String>());
      }
      else {  // static cast
        switch (type_) {
          case value_type::floating:
            return static_cast<T>(ref_to<Float>());
          case value_type::integer:
            return static_cast<T>(ref_to<Int>());
          case value_type::unsigned_integer:
            return static_cast<T>(ref_to<UInt>());
          case value_type::boolean:
            return static_cast<T>(ref_to<bool>());
          default:
            break;
        }
      }
    }
    return T();
  }

  // Handling the particular case of types not supporting casts
  str_type convert_to(type_proxy<String>) const { 
    if (value_type::string == type_) {
      return ref_to<String>();
    }
    else if (is_lexical(type_)) {
      switch (type_) {
        case value_type::floating:
          return lexical_cast<String>(ref_to<Float>());
        case value_type::integer:
          return lexical_cast<String>(ref_to<Int>());
        case value_type::unsigned_integer:
          return lexical_cast<String>(ref_to<UInt>());
        case value_type::boolean:
          return lexical_cast<String>(ref_to<bool>());
        default:
          break;
      }
    }
    return String();
  }

  Map convert_to(type_proxy<Map>) const { 
    if (value_type::map == type_) return ref_to<Map>();
    return Map();
  }

  Vector convert_to(type_proxy<Vector>) const { 
    if (value_type::vector == type_) return ref_to<Vector>();
    return Vector();
  }

 public:
  basic_container() {};
  basic_container(basic_container&& c) : basic_container(type_proxy<basic_container>(), std::forward<basic_container&&>(c)) {}
  basic_container(basic_container const& c) : basic_container(type_proxy<basic_container>(), c) {}
  // Constructor from other types
  template <typename T> basic_container(T&& arg) : basic_container(type_proxy<typename type_traits<decltype(arg)>::pure_type>(), std::forward<T>(arg)) {}
  // Handling char* case
  basic_container(typename str_type::value_type const* arg) : basic_container(type_proxy<str_type>(), str_type(arg)) {}

  // Initializer lists
  basic_container(std::initializer_list<std::pair<Key, basic_container>> list) : basic_container(type_proxy<Map>()){
    for(std::pair<Key, basic_container> const& elem : list) ref_to<Map>().emplace(elem);
  }

  static basic_container init_map(std::initializer_list<std::pair<Key, basic_container>> list) {
    basic_container container((type_proxy<Map>()));
    for(std::pair<Key, basic_container> const& elem : list) container.ref_to<Map>().emplace(elem);
    return container;
  }
  
  static basic_container init_vec(std::initializer_list<basic_container> list) {
    basic_container container((type_proxy<Vector>()));
    container.ref_to<Vector>().reserve(list.size());
    for(basic_container const& element : list) container.ref_to<Vector>().push_back(element);
    return container;
  }

  basic_container& operator= (basic_container const& c) { init_member(c); return *this; }
  basic_container& operator= (basic_container&& c) { init_member(c); return *this; }

  // Helper to initialize the container to a given type
  template <typename T> static basic_container init() { return basic_container(type_proxy<T>()); }

  ~basic_container() { clear(); }  // virtual not needed, this class is final

  // Assignement from another type
  template <typename T> basic_container& operator=(T&& arg) { 
    switch_to_type<typename type_traits<T>::pure_type>();
    init_member(std::forward<T>(arg));
    return *this;
  }
  // Handling char* case
  basic_container& operator=(typename str_type::value_type const* arg) {
    switch_to_type<str_type>();
    init_member(str_type(arg));
    return *this;
  }

  inline bool is_null() const noexcept { return value_type::null == type_; }
  inline bool is_map() const noexcept { return value_type::map == type_; }
  inline bool is_vector() const noexcept { return value_type::vector == type_; }
  inline bool is_string() const noexcept { return value_type::string == type_; }
  inline bool is_float() const noexcept { return value_type::floating == type_; }
  inline bool is_int() const noexcept { return value_type::integer == type_; }
  inline bool is_uint() const noexcept { return value_type::unsigned_integer == type_; }
  inline bool is_bool() const noexcept { return value_type::boolean == type_; }

  // By reference accessors
  template <typename T> T const& ref() const { 
    static_assert(type_traits<T>::is_pure, "Type must not be a reference nor have cv-qualifiers");
    if (type_traits<T>::type_value() != type_) throw exception_type();
    return ref_to<T>();
  }
  inline Null const& ref_null() const { return ref<Null>(); }
  inline Map const& ref_map() const { return ref<Map>(); }
  inline Vector const& ref_vector() const { return ref<Vector>(); }
  inline String const& ref_string() const { return ref<String>(); }
  inline Float const& ref_float() const { return ref<Float>(); }
  inline Int const& ref_int() const { return ref<Int>(); }
  inline UInt const& ref_uint() const { return ref<UInt>(); }
  inline bool const& ref_bool() const { return ref<bool>(); }

  template <typename T> T& ref() { 
    static_assert(type_traits<T>::is_pure, "Type must not be a reference nor have cv-qualifiers");
    if (type_traits<T>::type_value() != type_) throw exception_type();
    return ref_to<T>();
  }
  inline Null& ref_null() { return ref<Null>(); }
  inline Map& ref_map() { return ref<Map>(); }
  inline Vector& ref_vector() { return ref<Vector>(); }
  inline String& ref_string() { return ref<String>(); }
  inline Float& ref_float() { return ref<Float>(); }
  inline Int& ref_int() { return ref<Int>(); }
  inline UInt& ref_uint() { return ref<UInt>(); }
  inline bool& ref_bool() { return ref<bool>(); }

  // Forced type switch
  template <typename T> T& transform() noexcept { 
    static_assert(type_traits<T>::is_pure, "Type must not be a reference nor have cv-qualifiers");
    if (type_traits<T>::type_value() != type_) switch_to_type(convert_to(type_proxy<T>()));
    return ref_to<T>();
  }
  inline Null& transform_null() { return transform<Null>(); }
  inline Map& transform_map() { return transform<Map>(); }
  inline Vector& transform_vector() { return transform<Vector>(); }
  inline String& transform_string() { return transform<String>(); }
  inline Float& transform_float() { return transform<Float>(); }
  inline Int& transform_int() { return transform<Int>(); }
  inline UInt& transform_uint() { return transform<UInt>(); }
  inline bool& transform_bool() { return transform<bool>(); }

  // Raw accessors - use at your own risk
  template <typename T> T const& raw() const { 
    static_assert(type_traits<T>::is_from_container, "Type must be compatible with this container.");
    return ref_to<T>();
  }
  inline Null const& raw_null() const { return raw<Null>(); }
  inline Map const& raw_map() const { return raw<Map>(); }
  inline Vector const& raw_vector() const { return raw<Vector>(); }
  inline String const& raw_string() const { return raw<String>(); }
  inline Float const& raw_float() const { return raw<Float>(); }
  inline Int const& raw_int() const { return raw<Int>(); }
  inline UInt const& raw_uint() const { return raw<UInt>(); }
  inline bool const& raw_bool() const { return raw<bool>(); }

  template <typename T> T& raw() { 
    static_assert(type_traits<T>::is_from_container, "Type must be compatible with this container.");
    return ref_to<T>();
  }
  inline Null& raw_null() { return raw<Null>(); }
  inline Map& raw_map() { return raw<Map>(); }
  inline Vector& raw_vector() { return raw<Vector>(); }
  inline String& raw_string() { return raw<String>(); }
  inline Float& raw_float() { return raw<Float>(); }
  inline Int& raw_int() { return raw<Int>(); }
  inline UInt& raw_uint() { return raw<UInt>(); }
  inline bool& raw_bool() { return raw<bool>(); }

  // Lazy non throwing accessors
  template <typename T> T const* get() const noexcept { 
    return type_traits<T>::type_value() == type_ ? ptr_to<T>() : nullptr;
  }

  template <typename T> T* get() noexcept { 
    return type_traits<T>::type_value() == type_ ? ptr_to<T>() : nullptr;
  }

  template <typename T> bool get(T& output_value) const noexcept { 
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
  template <typename T> inline operator T () const { return convert_to(type_proxy<T>()); }
  template <typename T> inline T as() const { return convert_to(type_proxy<T>()); }
  inline Null as_null() const { return as<Null>(); }
  inline Map as_map() const { return as<Map>(); }
  inline Vector as_vector() const { return as<Vector>(); }
  inline String as_string() const { return as<String>(); }
  inline Float as_float() const { return as<Float>(); }
  inline Int as_int() const { return as<Int>(); }
  inline UInt as_uint() const { return as<UInt>(); }
  inline bool as_bool() const { return as<bool>(); }

  // Visiting
  struct const_visitor {
    virtual void apply(std::nullptr_t) {}
    virtual void apply(Map const&) {}
    virtual void apply(Vector const&) {}
    virtual void apply(String const&) {}
    virtual void apply(Float) {}
    virtual void apply(Int) {}
    virtual void apply(UInt) {}
    virtual void apply(bool) {}
  };

  void visit(const_visitor& v) const {
    switch (type_) {
      case value_type::null:
        v.apply(nullptr);
        break; 
      case value_type::map:
        v.apply(ref_to<Map>()); 
        break;
      case value_type::vector:
        v.apply(ref_to<Vector>()); 
        break;
      case value_type::string:
        v.apply(ref_to<String>()); 
        break;
      case value_type::floating:
        v.apply(ref_to<Float>()); 
        break;
      case value_type::integer:
        v.apply(ref_to<Int>()); 
        break;
      case value_type::unsigned_integer:
        v.apply(ref_to<UInt>()); 
        break;
      case value_type::boolean:
        v.apply(ref_to<bool>()); 
        break;
      default:
        break;
    }
  };
};

using container = basic_container<>;

namespace drivers {

template<typename container_type> class json_sstream {
  using base_visitor_type = typename container_type::const_visitor;
  using string_type = typename container_type::str_type;
  using ostream_type = std::basic_ostringstream<typename string_type::value_type, typename string_type::traits_type>;

  using key_type = typename container_type::key_type;
  using map_type = typename container_type::map_type;
  using vector_type = typename container_type::vector_type;
  using float_type = typename container_type::float_type;
  using int_type = typename container_type::int_type;
  using uint_type = typename container_type::uint_type;

  struct visitor_ostream : public base_visitor_type {
    ostream_type output_stream;
    
    virtual void apply(std::nullptr_t) override { output_stream << "null"; }
    
    virtual void apply(map_type const& v) override {
      output_stream << "{";
      bool first = true;
      for(auto const& element : v) {
        if (!first) output_stream << ",";
        first = false;
        output_stream << "\"" << element.first << "\":";
        element.second.visit(*this);
      }
      output_stream << "}";
    }
    
    virtual void apply(vector_type const& v) override {
      output_stream << "[";
      bool first = true;
      for(container_type const& element : v) {
        if (!first) output_stream << ",";
        first = false;
        element.visit(*this);
      }
      output_stream << "]";
    }
    
    virtual void apply(string_type const& v) override { output_stream << "\"" << v << "\""; }
    virtual void apply(float_type v) override { output_stream << v; }
    virtual void apply(int_type v) override { output_stream << v; }
    virtual void apply(uint_type v) override { output_stream << v; }
    virtual void apply(bool v) override { output_stream << (v?"true":"false"); }
  };

 public:
  string_type serialize(container_type const& container) const {
    visitor_ostream visitor;
    container.visit(visitor);
    return visitor.output_stream.str();
  }

  container_type deserialize(string_type const& input) {
  }
};

template <typename T> struct string_conversion_traits {
  static size_t constexpr max_size = 0u;
  static std::string const printf_code;
};
template <typename T> std::string const string_conversion_traits<T>::printf_code = "";

template <> struct string_conversion_traits<int> {
  static size_t constexpr max_size = std::numeric_limits<int>::digits + 1u;
  static std::string const printf_code;
};
std::string const string_conversion_traits<int>::printf_code = "%d";

template <> struct string_conversion_traits<unsigned int> {
  static size_t constexpr max_size = std::numeric_limits<unsigned int>::digits;
  static std::string const printf_code;
};
std::string const string_conversion_traits<unsigned int>::printf_code = "%u";

template <> struct string_conversion_traits<size_t> {
  static size_t constexpr max_size = std::numeric_limits<size_t>::digits;
  static std::string const printf_code;
};
std::string const string_conversion_traits<size_t>::printf_code = "%lu";

template <> struct string_conversion_traits<float> {
  static size_t constexpr max_size = 3 + FLT_MANT_DIG - FLT_MIN_EXP;
  static std::string const printf_code;
};
std::string const string_conversion_traits<float>::printf_code = "%f";

template<typename container_type> class json {
  using base_visitor_type = typename container_type::const_visitor;
  using string_type = typename container_type::str_type;
  using ostream_type = std::basic_ostringstream<typename string_type::value_type, typename string_type::traits_type>;

  using key_type = typename container_type::key_type;
  using map_type = typename container_type::map_type;
  using vector_type = typename container_type::vector_type;
  using float_type = typename container_type::float_type;
  using int_type = typename container_type::int_type;
  using uint_type = typename container_type::uint_type;

  struct visitor_size : public base_visitor_type {
    size_t size = 0u;
    
    virtual void apply(std::nullptr_t) override { size += 3u; }
    
    virtual void apply(map_type const& v) override {
      size += 2u;  // {}
      bool first = true;
      for(auto const& element : v) {
        if (!first) ++size;
        first = false;
        size += 3u + element.first.size();
        element.second.visit(*this);
      }
    }
    
    virtual void apply(vector_type const& v) override {
      size += 2; // []
      bool first = true;
      for(container_type const& element : v) {
        if (!first) ++size;
        first = false;
        element.visit(*this);
      }
    }
    
    virtual void apply(string_type const& v) override { size += 2u + v.size(); }
    virtual void apply(float_type v) override { size += string_conversion_traits<float_type>::max_size; }
    virtual void apply(int_type v) override { size += string_conversion_traits<int_type>::max_size; }
    virtual void apply(uint_type v) override { size += string_conversion_traits<uint_type>::max_size; }
    virtual void apply(bool v) override { size += 5; }
  };

  struct visitor_print : public base_visitor_type {
    char * buffer = nullptr;
    char * cur_buffer = nullptr;
    
    virtual void apply(std::nullptr_t) override { 
      std::strncpy(cur_buffer, "null", 5u);  // Do not forget the \0 end
      cur_buffer += 4u;
    }
    
    virtual void apply(map_type const& v) override {
      *cur_buffer = '{';
      ++cur_buffer;
      bool first = true;
      for(auto const& element : v) {
        if (!first) {
          *cur_buffer = ',';
          ++cur_buffer;
        }
        first = false;
        std::snprintf(cur_buffer, 4u + element.first.size(), "\"%s\":", element.first.c_str());
        cur_buffer += 3u + element.first.size();
        element.second.visit(*this);
      } 
      std::strncpy(cur_buffer,"}",2u);
      ++cur_buffer;
    } 
    
    virtual void apply(vector_type const& v) override {
      *cur_buffer = '[';
      ++cur_buffer;
      bool first = true;
      for(container_type const& element : v) {
        if (!first) {
          *cur_buffer = ',';
          ++cur_buffer;
        }
        first = false;
        element.visit(*this);
      } 
      std::strncpy(cur_buffer,"]",2u);
      ++cur_buffer;
    }
    
    virtual void apply(string_type const& v) override { 
      std::snprintf(cur_buffer, 3u + v.size(), "\"%s\"", v.c_str());
      cur_buffer += 2u + v.size();
    }

    virtual void apply(float_type v) override { 
      int char_written = std::snprintf(
          cur_buffer
          , string_conversion_traits<float_type>::max_size
          , string_conversion_traits<float_type>::printf_code.c_str()
          , v);
      if (0 < char_written) cur_buffer += char_written;
    }

    virtual void apply(int_type v) override { 
      int char_written = std::snprintf(
          cur_buffer
          , string_conversion_traits<int_type>::max_size
          , string_conversion_traits<int_type>::printf_code.c_str()
          , v);
      if (0 < char_written) cur_buffer += char_written;
    }

    virtual void apply(uint_type v) override { 
      int char_written = std::snprintf(
          cur_buffer
          , string_conversion_traits<uint_type>::max_size
          , string_conversion_traits<uint_type>::printf_code.c_str()
          , v);
      if (0 < char_written) cur_buffer += char_written;
    }

    virtual void apply(bool v) override { 
      if (v) {
        std::strncpy(cur_buffer, "true", 5u);
        cur_buffer += 4u;
      }
      else {
        std::strncpy(cur_buffer, "false", 5u);
        cur_buffer += 5u;
      }
    }
  }; 

 public:
  string_type serialize(container_type const& container) const {
    visitor_size size_calculator;
    container.visit(size_calculator);
    visitor_print printer;
    printer.buffer = reinterpret_cast<char*>(std::malloc(size_calculator.size));
    printer.cur_buffer = printer.buffer;
    container.visit(printer);
    std::string result(printer.buffer);
    std::free(printer.buffer);
    return result;
  }

  container_type deserialize(string_type const& input) {
  }
};

}  // namespace drivers
}  // namespace nested_container

#endif  // __NESTED_COMPILER__
