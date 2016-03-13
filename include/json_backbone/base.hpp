#ifndef JSON_BACKBONE_BASE_HEADER
#define JSON_BACKBONE_BASE_HEADER
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
#include <iostream>

namespace json_backbone {

// template <class key_type, class value_type>
// using std_object_default_allocators = std::map<key_type, value_type>;
// template <class value_type>
// using std_array_default_allocators = std::vector<value_type>;
template <class Container>
class attr_init;

template <class Container>
class array_element_init;

namespace private_ {

template <class Type, size_t Index>
struct type_info {};

template <class Indices, class... Types>
struct type_list;

template <size_t... Is, class... Types>
struct type_list<std::index_sequence<Is...>, Types...> : type_info<Types, Is>... {
  template <class Type, size_t Index>
  static constexpr std::integral_constant<size_t, Index> type_index(type_info<Type, Index> const&) {
    return {};
  }

  template <class Type>
  static constexpr std::integral_constant<size_t, sizeof...(Types)> type_index(...) {
    return {};
  }

  template <class T>
  static constexpr size_t get_index() {
    return decltype(type_index<T>(std::declval<type_list>()))::value;
  }

  template <class T>
  static constexpr bool has_type() {
    return decltype(type_index<T>(std::declval<type_list>()))::value < sizeof...(Types);
  }
};

template <class T>
std::enable_if_t<sizeof(T) <= sizeof(void*), std::function<void(void**)>> make_deleter() {
  return [](void** data) { reinterpret_cast<T*>(data)->~T(); };
}

template <class T>
std::enable_if_t<sizeof(void*) < sizeof(T), std::function<void(void**)>> make_deleter() {
  return [](void** data) { delete reinterpret_cast<T*>(*data); };
}

template <class T>
std::enable_if_t<sizeof(T) <= sizeof(void*), std::function<void(void**, void*)>> make_store_move() {
  return [](void** data, void* pval) {
    new (reinterpret_cast<T*>(data)) T(std::move(*reinterpret_cast<T*>(pval)));
  };
}

template <class T>
std::enable_if_t<sizeof(void*) < sizeof(T), std::function<void(void**, void*)>> make_store_move() {
  return [](void** data, void* pval) {
    T** ptr = reinterpret_cast<T**>(data);
    *ptr = new T(std::move(*reinterpret_cast<T*>(pval)));
  };
}

template <class T>
std::enable_if_t<sizeof(T) <= sizeof(void*), std::function<void(void**, void*)>> make_store_copy() {
  return [](void** data, void* pval) {
    new (reinterpret_cast<T*>(data)) T(*reinterpret_cast<T*>(pval));
  };
}

template <class T>
std::enable_if_t<sizeof(void*) < sizeof(T), std::function<void(void**, void*)>> make_store_copy() {
  return [](void** data, void* pval) {
    T** ptr = reinterpret_cast<T**>(data);
    *ptr = new T(*reinterpret_cast<T*>(pval));
  };
}

}  // namespace private

template <class I, size_t N>
I constexpr max_value(std::array<I, N> const& values, I current_value, size_t current_index) {
  return N <= current_index ? current_value
                            : (current_value < values.at(current_index)
                                   ? max_value(values, values.at(current_index), current_index + 1)
                                   : max_value(values, current_value, current_index + 1));
}

template <class Container>
struct container_traits {
  using char_sequence_storage_type = typename std::conditional<
      Container::type_list_type::template has_type<std::string>(), std::string,
      typename std::conditional<Container::type_list_type::template has_type<std::wstring>(),
                                std::wstring, void>::type>::type;

  // TODO: add a trait to extract first default constructible type
  using default_type = typename std::conditional<
      Container::type_list_type::template has_type<std::nullptr_t>(), std::nullptr_t,
      typename std::conditional<Container::type_list_type::template has_type<int>(), int,
                                void>::type>::type;
};

template <template <class...> class ObjectBase, template <class...> class ArrayBase, class Key,
          class... Value>
class basic_container final {
  friend attr_init<basic_container>;
  friend array_element_init<basic_container>;

 public:
  using object_type = ObjectBase<Key, basic_container>;
  using array_type = ArrayBase<basic_container>;
  using key_type = Key;
  using type_list_type = private_::type_list<std::make_index_sequence<sizeof...(Value) + 2u>,
                                             object_type, array_type, Value...>;
  using value_type_list_type =
      private_::type_list<std::make_index_sequence<sizeof...(Value)>, Value...>;

 private:
  size_t type_ = type_list_type::template get_index<
      typename container_traits<basic_container>::default_type>();
  void* data_ = nullptr;

  void clear() {
    static std::array<std::function<void(void**)>, sizeof...(Value) + 2> deleters = {
        private_::make_deleter<object_type>(), private_::make_deleter<array_type>(),
        private_::make_deleter<Value>()...};
    deleters[type_](&data_);
  }

  template <class T>
  void create(T&& value) {
    static_assert(type_list_type::template has_type<T>(),
                  "The container annot store an unsupported type.");
    static std::array<std::function<void(void**, void*)>, sizeof...(Value) + 2> ctors = {
        private_::make_store_move<object_type>(), private_::make_store_move<array_type>(),
        private_::make_store_move<Value>()...};
    type_ = type_list_type::template get_index<T>();
    ctors[type_](&data_, &value);
  }

  template <class T>
  void create(T const& value) {
    static_assert(type_list_type::template has_type<T>(),
                  "The container annot store an unsupported type.");
    static std::array<std::function<void(void**, void*)>, sizeof...(Value) + 2> ctors = {
        private_::make_store_copy<object_type>(), private_::make_store_copy<array_type>(),
        private_::make_store_copy<Value>()...};
    type_ = type_list_type::template get_index<T>();
    ctors[type_](&data_, &value);
  }

  template <class T>
  void store(T&& value) {
    clear();
    create(std::forward<T>(value));
  }

 public:
  basic_container() {
    store<typename container_traits<basic_container>::default_type>({});
  }

  template <class T> basic_container(T&& value) {
    create(std::forward<T>(value));
  }

  ~basic_container() {
    clear();
  }

  /*
    // Public type declarations
    using key_type = Key;
    using str_type = String;
    using int_type = Int;
    using uint_type = UInt;
    using float_type = Float;
    using object_type = ObjectTemplate<key_type, basic_container>;
    using array_type = ArrayTemplate<basic_container>;
    using array_size_type = typename array_type::size_type;

    class exception_type : std::exception {
      const char* what() const noexcept { return "NestedContainer exception.";
   }
    };

   private:
    using Object = object_type;
    using Array = array_type;
    using ArraySizeType = typename array_type::size_type;
    using Null = std::nullptr_t;

    enum class value_type : unsigned char {
      null = '0',
      object,
      array,
      string,
      floating,
      integer,
      unsigned_integer,
      boolean
    };

    union value final {
      value() {}
      ~value() {}

      Object* object_;
      Array* array_;
      String str_;
      Float float_;
      Int int_;
      UInt uint_;
      bool bool_;
    };

    template <class A, class B> using eq = std::is_same<A, B>;
    template <class A, class B>
    using ifeq = typename std::enable_if<eq<A, B>::value, int>::type;
    template <class A, class B>
    using ifneq = typename std::enable_if<!eq<A, B>::value, int>::type;

    template <class Member> struct type_traits {
      using T = typename std::remove_cv<
          typename std::remove_reference<Member>::type>::type;
      using pure_type = T;
      static constexpr value_type type_value() {
        return eq<bool, T>::value
                   ? value_type::boolean
                   : eq<Int, T>::value
                         ? value_type::integer
                         : eq<UInt, T>::value
                               ? value_type::unsigned_integer
                               : eq<Float, T>::value
                                     ? value_type::floating
                                     : eq<String, T>::value
                                           ? value_type::string
                                           : eq<Array, T>::value
                                                 ? value_type::array
                                                 : eq<Object, T>::value
                                                       ? value_type::object
                                                       : value_type::null;
      }

      static bool constexpr is_from_container =
          eq<bool, T>::value || eq<Int, T>::value || eq<UInt, T>::value ||
          eq<Float, T>::value || eq<String, T>::value || eq<Array, T>::value
   ||
          eq<Object, T>::value || eq<std::nullptr_t, T>::value ||
          eq<basic_container, T>::value;

      static bool constexpr is_collection =
          eq<Array, T>::value || eq<Object, T>::value;
      static bool constexpr is_lexical =
          eq<bool, T>::value || eq<Int, T>::value || eq<UInt, T>::value ||
          eq<Float, T>::value || eq<String, T>::value;
      static bool constexpr is_null = eq<Null, T>::value;
      static bool constexpr is_index =
          eq<Key, T>::value || eq<array_size_type, T>::value;
      static bool constexpr is_pure = eq<T, Member>::value;
      static bool constexpr is_self = eq<basic_container, T>::value;

      // static_assert(is_from_container, "Type must be one of container's
      // internal types");
    };

    // boost lexical cast, simple non-throwing version
    // Putting the lexical cast here as a member is ugly but needed to avoid
    // externalization conflicts when two different containers shares a same
   type
    // which is the case for instance with container_double and
   container_longint_double
    template <class Target, class Source>
    static typename std::enable_if<std::is_fundamental<Source>::value,
                                   Target>::type
    lexical_cast(Source arg) {
      std::stringstream interpreter;
      Target result;

      if (!(interpreter << arg) || !(interpreter >> result) ||
          !(interpreter >> std::ws).eof())
        return Target();

      return result;
    }

    template <class Target, class Source>
    static typename std::enable_if<!std::is_fundamental<Source>::value,
                                   Target>::type
    lexical_cast(Source const& arg) {
      std::stringstream interpreter;
      Target result;

      if (!(interpreter << arg) || !(interpreter >> result) ||
          !(interpreter >> std::ws).eof())
        return Target();

      return result;
    }

    static bool is_lexical(value_type type) {
      return value_type::boolean == type || value_type::integer == type ||
             value_type::unsigned_integer == type ||
             value_type::floating == type || value_type::string == type;
    }

    static bool is_collection(value_type type) {
      return value_type::object == type || value_type::array == type;
    }

    // Static instance is used while in constant context
    // and using operator[]
    static basic_container const& get_static_const_default() {
      static basic_container const default_container {};
      return default_container;
    }

    value value_;
    value_type type_ = value_type::null;

    void clear() {
      switch (type_) {
      case value_type::object:
        delete value_.object_;
        break;
      case value_type::array:
        delete value_.array_;
        break;
      case value_type::string:
        value_.str_.~str_type();
        break;
      default:
        break;
      }
    }

    // Compile time initializers
    template <class T, ifeq<Null, T> = 0> inline void init_member() {}
    template <class T, ifeq<Object, T> = 0> inline void init_member() {
      value_.object_ = new Object;
    }
    template <class T, ifeq<Array, T> = 0> inline void init_member() {
      value_.array_ = new Array;
    }
    template <class T, ifeq<String, T> = 0> inline void init_member() {
      new (&(value_.str_)) String;
    }
    template <class T, ifeq<Float, T> = 0> inline void init_member() {
      value_.float_ = 0.f;
    }
    template <class T, ifeq<Int, T> = 0> inline void init_member() {
      value_.int_ = 0;
    }
    template <class T, ifeq<UInt, T> = 0> inline void init_member() {
      value_.uint_ = 0u;
    }
    template <class T, ifeq<bool, T> = 0> inline void init_member() {
      value_.bool_ = false;
    }

    // Init with value -  trivial types
    inline void init_member(std::nullptr_t) {}
    inline void init_member(Float v) { value_.float_ = v; }
    inline void init_member(Int v) { value_.int_ = v; }
    inline void init_member(UInt v) { value_.uint_ = v; }
    inline void init_member(bool v) { value_.bool_ = v; }

    // Init with value - move
    inline void init_member(Object&& v) { value_.object_ = new
   Object(std::move(v)); }
    inline void init_member(Array&& v) {
      value_.array_ = new Array(std::move(v));
    }
    inline void init_member(String&& v) {
      new (&(value_.str_)) String(std::move(v));
    }
    inline void init_member(basic_container&& c) {
      bool same_type = (c.type_ == type_);
      switch (c.type_) {
      case value_type::null:
        same_type ? void() : clear();
        break;
      case value_type::object:
        clear();
        value_.object_ = c.value_.object_;
        c.value_.object_ = nullptr; // Useless in theory, just in case
        break;
      case value_type::array:
        clear();
        value_.array_ = c.value_.array_;
        c.value_.array_ = nullptr; // Useless in theory, just in case
        break;
      case value_type::string:
        same_type ? void() : switch_to_type(value_type::string);
        std::swap(value_.str_, c.value_.str_);
        break;
      case value_type::floating:
        same_type ? void() : clear();
        value_.float_ = c.value_.float_;
        break;
      case value_type::integer:
        same_type ? void() : clear();
        value_.int_ = c.value_.int_;
        break;
      case value_type::unsigned_integer:
        same_type ? void() : clear();
        value_.uint_ = c.value_.uint_;
        break;
      case value_type::boolean:
        same_type ? void() : clear();
        value_.bool_ = c.value_.bool_;
        break;
      default:
        break;
      }

      type_ = c.type_;
      c.type_ = value_type::null;
    }

    // Init with value - copy
    inline void init_member(Object const& v) { value_.object_ = new Object(v);
   }
    inline void init_member(Array const& v) { value_.array_ = new Array(v); }
    inline void init_member(String const& v) { new (&(value_.str_)) String(v);
   }
    inline void init_member(basic_container const& c) {
      value_type previous_type = type_;
      if (type_ != c.type_ && is_collection(type_) && is_collection(c.type_))
   {
        // We have to handle the special case of an affectation from a
   container
        // sub element to itself : deletion must be defered not to read
        // a deleted memory area
        if (value_type::object == type_) {
          Object* defered_object = nullptr;
          init_member(*c.value_.array_);
          delete defered_object;
        } else {
          Array* defered_array = nullptr;
          init_member(*c.value_.object_);
          delete defered_array;
        }
        type_ = c.type_;
      } else {
        switch_to_type(c.type_);
        switch (type_) {
        case value_type::null:
          break;
        case value_type::object:
          *value_.object_ = *c.value_.object_;
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
        case value_type::boolean:
          value_.bool_ = c.value_.bool_;
          break;
        default:
          break;
        }
      }
    }

    // Runtime version
    void init_member(value_type target_type) {
      switch (target_type) {
      case value_type::object:
        init_member<Object>();
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
      case value_type::boolean:
        init_member<bool>();
        break;
      default:
        break;
      }
    }

    // When target type is known at compilation
    template <class T> void switch_to_type() {
      value_type constexpr target_type = type_traits<T>::type_value();
      if (target_type == type_)
        return;
      clear();
      init_member<T>();
      type_ = target_type;
    }

    template <class T> void switch_to_type(T&& value) {
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
      if (target_type == type_)
        return;
      clear();
      init_member(target_type);
      type_ = target_type;
    }

    // For default construction with a type at compile type
    template <class T> struct type_proxy {};
    template <class T>
    explicit basic_container(type_proxy<T>)
        : type_(type_traits<T>::type_value()) {
      init_member<T>();
    }

    //// Accessors, private, not protected against bad behavior, checks must
   be done
    //// before
    template <class T, ifeq<Object, T> = 0> inline Object& ref_to() {
      return *value_.object_;
    }
    template <class T, ifeq<Array, T> = 0> inline Array& ref_to() {
      return *value_.array_;
    }
    template <class T, ifeq<String, T> = 0> inline String& ref_to() {
      return value_.str_;
    }
    template <class T, ifeq<Float, T> = 0> inline Float& ref_to() {
      return value_.float_;
    }
    template <class T, ifeq<Int, T> = 0> inline Int& ref_to() {
      return value_.int_;
    }
    template <class T, ifeq<UInt, T> = 0> inline UInt& ref_to() {
      return value_.uint_;
    }
    template <class T, ifeq<bool, T> = 0> inline bool& ref_to() {
      return value_.bool_;
    }

    template <class T, ifeq<Object, T> = 0> inline Object const& ref_to()
   const {
      return *value_.object_;
    }
    template <class T, ifeq<Array, T> = 0>
    inline Array const& ref_to() const {
      return *value_.array_;
    }
    template <class T, ifeq<String, T> = 0>
    inline String const& ref_to() const {
      return value_.str_;
    }
    template <class T, ifeq<Float, T> = 0> inline Float const& ref_to() const
   {
      return value_.float_;
    }
    template <class T, ifeq<Int, T> = 0> inline Int const& ref_to() const {
      return value_.int_;
    }
    template <class T, ifeq<UInt, T> = 0> inline UInt const& ref_to() const {
      return value_.uint_;
    }
    template <class T, ifeq<bool, T> = 0> inline bool const& ref_to() const {
      return value_.bool_;
    }

    // [] accessors, Key != size_type version
    template <ifneq<Key, ArraySizeType> = 0>
    basic_container const& access_collection(Key const& index) const noexcept
   {
      if (!is_object())
        return get_static_const_default();
      auto value_iterator = ref_to<Object>().find(index);
      if (end(ref_to<Object>()) == value_iterator)
        return get_static_const_default();
      return value_iterator->second;
    }

    template <ifneq<Key, ArraySizeType> = 0>
    basic_container const& access_collection(array_size_type index) const
   noexcept {
      if (!is_array() || ref_to<Array>().size() <= index)
        return get_static_const_default();
      return ref_to<Array>()[index];
    }

    template <ifneq<Key, ArraySizeType> = 0>
    basic_container& access_collection(Key const& index) noexcept {
      if (!is_object())
        switch_to_type<Object>();
      return ref_to<Object>()[index];
    }

    template <ifneq<Key, ArraySizeType> = 0>
    basic_container& access_collection(Key&& index) noexcept {
      if (!is_object())
        switch_to_type<Object>();
      return ref_to<Object>()[std::move(index)];
    }

    template <ifneq<Key, ArraySizeType> = 0>
    basic_container& access_collection(array_size_type index) noexcept {
      if (!is_array())
        switch_to_type<Array>();
      if (ref_to<Array>().size() <= index)
        ref_to<Array>().resize(index + 1);
      return ref_to<Array>()[index];
    }

    // template <typename T> convert_to
    template <class T, typename std::enable_if<
                              !eq<T, Null>::value && !eq<T, String>::value &&
                                  !eq<T, Object>::value && !eq<T,
   Array>::value,
                              int>::type = 0>
    T convert_to() const {
      static_assert(type_traits<T>::is_pure,
                    "Type must not be a reference nor have cv-qualifiers");
      value_type target_type = type_traits<T>::type_value();
      if (target_type == type_) {
        return ref_to<T>();
      } else if (is_lexical(target_type) && is_lexical(type_)) {
        if (value_type::string == type_) { // lexical cast
          return lexical_cast<T>(ref_to<String>());
        } else { // static cast
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
    template <class T, ifeq<Null, T> = 0> T convert_to() const {
      return nullptr;
    }

    template <class T, ifeq<String, T> = 0> T convert_to() const {
      if (value_type::string == type_) {
        return ref_to<String>();
      } else if (is_lexical(type_)) {
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

    template <class T, ifeq<Object, T> = 0> T convert_to() const {
      if (value_type::object == type_)
        return ref_to<Object>();
      return Object();
    }

    template <class T, ifeq<Array, T> = 0> T convert_to() const {
      if (value_type::array == type_)
        return ref_to<Array>();
      return Array();
    }

   public:
    basic_container(){};
    basic_container(basic_container&& c) { init_member(std::move(c)); }
    basic_container(basic_container const& c) { init_member(c); }
    basic_container(basic_container& c) { init_member(c); }
    // Constructor from other types

    // Handling char array case
    template <std::size_t Size>
    basic_container(char const (&arg)[Size])
        : basic_container(str_type(arg, Size-1)) {}
    // Handling char* case
    template <class T, ifeq<typename String::value_type const*, T> = 0>
    basic_container(T arg)
        : basic_container(str_type(arg)) {}
    template <class T, ifeq<typename String::value_type*, T> = 0>
    basic_container(T arg)
        : basic_container(str_type(arg)) {}
    // Other types
    template <class T, typename std::enable_if<
                              type_traits<T>::is_from_container, int>::type =
   0>
    basic_container(T&& arg)
        : type_(type_traits<T>::type_value()) {
      init_member(std::forward<T>(arg));
    }

    // Initializer lists
    basic_container(std::initializer_list<attr_init<basic_container>> list)
        : type_(value_type::object) {
      init_member<Object>();
      for (auto const& elem : list)
        ref_to<Object>().emplace(elem.key(), elem.value());
    }
    basic_container(
        std::initializer_list<array_element_init<basic_container>> list)
        : type_(value_type::array) {
      init_member<Array>();
      ref_to<Array>().reserve(list.size());
      for (auto const& element : list)
        ref_to<Array>().emplace_back(element.value());
    }

    static basic_container
    init_object(std::initializer_list<std::pair<Key, basic_container>> list) {
      basic_container container((type_proxy<Object>()));
      for (std::pair<Key, basic_container> const& elem : list)
        container.ref_to<Object>().emplace(elem);
      return container;
    }

    static basic_container init_vec(std::initializer_list<basic_container>
   list) {
      basic_container container((type_proxy<Array>()));
      container.ref_to<Array>().reserve(list.size());
      for (basic_container const& element : list)
        container.ref_to<Array>().emplace_back(element);
      return container;
    }

    // Helper to initialize the container to a given type
    template <class T> static basic_container init() {
      return basic_container(type_proxy<T>());
    }

    ~basic_container() { clear(); } // virtual not needed, this class is final

    // Assignement from another type
    basic_container& operator=(basic_container const& c) {
      init_member(c);
      return *this;
    }
    basic_container& operator=(basic_container&& c) {
      init_member(std::move(c));
      return *this;
    }
    basic_container& operator=(basic_container& c) {
      init_member(c);
      return *this;
    }
    template <size_t Length>
    basic_container&
    operator=(typename str_type::value_type const (&arg)[Length]) {
      switch_to_type<str_type>();
      init_member(str_type(arg, Length));
      return *this;
    }
    // Handling char* case
    basic_container& operator=(typename str_type::value_type const* arg) {
      switch_to_type<str_type>();
      init_member(str_type(arg));
      return *this;
    }
    template <typename T> basic_container& operator=(T&& arg) {
      switch_to_type<typename type_traits<T>::pure_type>();
      init_member(std::forward<T>(arg));
      return *this;
    }

    inline bool is_null() const noexcept { return value_type::null == type_; }
    inline bool is_object() const noexcept { return value_type::object ==
   type_; }
    inline bool is_array() const noexcept { return value_type::array == type_;
   }
    inline bool is_string() const noexcept { return value_type::string ==
   type_; }
    inline bool is_float() const noexcept {
      return value_type::floating == type_;
    }
    inline bool is_int() const noexcept { return value_type::integer == type_;
   }
    inline bool is_uint() const noexcept {
      return value_type::unsigned_integer == type_;
    }
    inline bool is_bool() const noexcept { return value_type::boolean ==
   type_; }

    // By reference accessors
    template <class T> T const& ref() const {
      static_assert(type_traits<T>::is_pure,
                    "Type must not be a reference nor have cv-qualifiers");
      if (type_traits<T>::type_value() != type_)
        throw exception_type();
      return ref_to<T>();
    }
    inline Null ref_null() const { return nullptr; }
    inline Object const& ref_object() const { return ref<Object>(); }
    inline Array const& ref_array() const { return ref<Array>(); }
    inline String const& ref_string() const { return ref<String>(); }
    inline Float const& ref_float() const { return ref<Float>(); }
    inline Int const& ref_int() const { return ref<Int>(); }
    inline UInt const& ref_uint() const { return ref<UInt>(); }
    inline bool const& ref_bool() const { return ref<bool>(); }

    template <class T> T& ref() {
      static_assert(type_traits<T>::is_pure,
                    "Type must not be a reference nor have cv-qualifiers");
      if (type_traits<T>::type_value() != type_)
        throw exception_type();
      return ref_to<T>();
    }
    inline Object& ref_object() { return ref<Object>(); }
    inline Array& ref_array() { return ref<Array>(); }
    inline String& ref_string() { return ref<String>(); }
    inline Float& ref_float() { return ref<Float>(); }
    inline Int& ref_int() { return ref<Int>(); }
    inline UInt& ref_uint() { return ref<UInt>(); }
    inline bool& ref_bool() { return ref<bool>(); }

    // Forced type switch
    template <class T> T& transform() noexcept {
      static_assert(type_traits<T>::is_pure,
                    "Type must not be a reference nor have cv-qualifiers");
      if (type_traits<T>::type_value() != type_)
        switch_to_type(convert_to<T>());
      return ref_to<T>();
    }
    inline Null transform_null() {
      switch_to_type<Null>();
      return nullptr;
    }
    inline Object& transform_object() { return transform<Object>(); }
    inline Array& transform_array() { return transform<Array>(); }
    inline String& transform_string() { return transform<String>(); }
    inline Float& transform_float() { return transform<Float>(); }
    inline Int& transform_int() { return transform<Int>(); }
    inline UInt& transform_uint() { return transform<UInt>(); }
    inline bool& transform_bool() { return transform<bool>(); }

    // Raw accessors - use at your own risk
    template <class T> T const& raw() const {
      static_assert(type_traits<T>::is_from_container,
                    "Type must be compatible with this container.");
      return ref_to<T>();
    }
    inline Null raw_null() const { return nullptr; }
    inline Object const& raw_object() const { return raw<Object>(); }
    inline Array const& raw_array() const { return raw<Array>(); }
    inline String const& raw_string() const { return raw<String>(); }
    inline Float const& raw_float() const { return raw<Float>(); }
    inline Int const& raw_int() const { return raw<Int>(); }
    inline UInt const& raw_uint() const { return raw<UInt>(); }
    inline bool const& raw_bool() const { return raw<bool>(); }

    template <class T> T& raw() {
      static_assert(type_traits<T>::is_from_container,
                    "Type must be compatible with this container.");
      return ref_to<T>();
    }
    inline Object& raw_object() { return raw<Object>(); }
    inline Array& raw_array() { return raw<Array>(); }
    inline String& raw_string() { return raw<String>(); }
    inline Float& raw_float() { return raw<Float>(); }
    inline Int& raw_int() { return raw<Int>(); }
    inline UInt& raw_uint() { return raw<UInt>(); }
    inline bool& raw_bool() { return raw<bool>(); }

    // Lazy non throwing accessors
    template <class T> T const* get() const noexcept {
      return type_traits<T>::type_value() == type_ ? &raw<T>() : nullptr;
    }
    inline Object const* get_object() const { return get<Object>(); }
    inline Array const* get_array() const { return get<Array>(); }
    inline String const* get_string() const { return get<String>(); }
    inline Float const* get_float() const { return get<Float>(); }
    inline Int const* get_int() const { return get<Int>(); }
    inline UInt const* get_uint() const { return get<UInt>(); }
    inline bool const* get_bool() const { return get<bool>(); }

    template <class T> T* get() noexcept {
      return type_traits<T>::type_value() == type_ ? &raw<T>() : nullptr;
    }
    inline Object* get_object() noexcept { return get<Object>(); }
    inline Array* get_array() noexcept { return get<Array>(); }
    inline String* get_string() noexcept { return get<String>(); }
    inline Float* get_float() noexcept { return get<Float>(); }
    inline Int* get_int() noexcept { return get<Int>(); }
    inline UInt* get_uint() noexcept { return get<UInt>(); }
    inline bool* get_bool() noexcept { return get<bool>(); }

    template <class T> bool get(T& output_value) const noexcept {
      if (type_traits<T>::type_value() == type_) {
        output_value = ref_to<T>();
        return true;
      }
      return false;
    }
    inline bool get_object(Object& v) noexcept { return get<Object>(v); }
    inline bool get_array(Array& v) noexcept { return get<Array>(v); }
    inline bool get_string(String& v) noexcept { return get<String>(v); }
    inline bool get_float(Float& v) noexcept { return get<Float>(v); }
    inline bool get_int(Int& v) noexcept { return get<Int>(v); }
    inline bool get_uint(UInt& v) noexcept { return get<UInt>(v); }
    inline bool get_bool(bool& v) noexcept { return get<bool>(v); }

    template <ifneq<Key, ArraySizeType> = 0>
    basic_container const&
    operator[](typename key_type::value_type const* index) const {
      return access_collection(index);
    }
    template <ifneq<Key, ArraySizeType> = 0>
    basic_container&
    operator[](typename key_type::value_type const* index) noexcept {
      return access_collection(index);
    }
    template <class T>
    basic_container const& operator[](T const& index) const {
      return access_collection(index);
    }
    template <class T> basic_container& operator[](T&& index) noexcept {
      return access_collection(std::forward<T&&>(index));
    }

    std::type_info const& get_typeid() const {
      switch (type_) {
      case value_type::null:
        return typeid(std::nullptr_t);
        break;
      case value_type::object:
        return typeid(Object);
        break;
      case value_type::array:
        return typeid(Array);
        break;
      case value_type::string:
        return typeid(String);
        break;
      case value_type::floating:
        return typeid(Float);
        break;
      case value_type::integer:
        return typeid(Int);
        break;
      case value_type::unsigned_integer:
        return typeid(UInt);
        break;
      case value_type::boolean:
        return typeid(bool);
        break;
      default:
        break;
      }
    };

    // Conversion.
    template <class T> inline operator T() const { return convert_to<T>(); }
    template <class T> inline T as() const { return convert_to<T>(); }
    inline Null as_null() const { return as<Null>(); }
    inline Object as_object() const { return as<Object>(); }
    inline Array as_array() const { return as<Array>(); }
    inline String as_string() const { return as<String>(); }
    inline Float as_float() const { return as<Float>(); }
    inline Int as_int() const { return as<Int>(); }
    inline UInt as_uint() const { return as<UInt>(); }
    inline bool as_bool() const { return as<bool>(); }

    template <class T> void const_visit(T& v) const {
      switch (type_) {
      case value_type::null:
        v.apply(nullptr);
        break;
      case value_type::object:
        v.apply(ref_to<Object>());
        break;
      case value_type::array:
        v.apply(ref_to<Array>());
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
    */
};

/*
template <class Container> class attr_init final {
  using key_type = typename Container::str_type;

  mutable key_type key_;
  mutable Container value_;

 public:
  explicit attr_init(key_type const& key)
      : key_(key) {}
  explicit attr_init(key_type&& key)
      : key_(std::move(key)) {}
  explicit attr_init(key_type const& key, Container const& value)
      : key_(key)
      , value_(value) {}
  explicit attr_init(key_type&& key, Container const& value)
      : key_(std::move(key))
      , value_(value) {}
  explicit attr_init(key_type const& key, Container&& value)
      : key_(key)
      , value_(std::move(value)) {}
  explicit attr_init(key_type&& key, Container&& value)
      : key_(std::move(key))
      , value_(std::move(value)) {}

  attr_init& operator=(Container const& value) {
    value_ = value;
    return *this;
  }
  attr_init& operator=(Container&& value) {
    value_ = std::move(value);
    return *this;
  }
  attr_init& operator()(Container const& value) {
    value_ = value;
    return *this;
  }
  attr_init& operator()(Container&& value) {
    value_ = std::move(value);
    return *this;
  }

  // Always consumed with move
  inline key_type key() const { return std::move(key_); }
  inline Container value() const { return std::move(value_); }
};

template <class Container> class array_element_init final {
  // static_assert(std::is_same< basic_

  mutable Container value_;

 public:
  array_element_init(Container const& value)
      : value_(value) {}
  array_element_init(Container&& value)
      : value_(std::move(value)) {}
  template <class T>
  array_element_init(T&& value)
      : value_(std::forward<T>(value)) {}

  array_element_init& operator=(Container const& value) {
    value_ = value;
    return *this;
  }
  array_element_init& operator=(Container&& value) {
    value_ = std::move(value);
    return *this;
  }
  array_element_init& operator()(Container const& value) {
    value_ = value;
    return *this;
  }
  array_element_init& operator()(Container&& value) {
    value_ = std::move(value);
    return *this;
  }

  // Always consumed with move
  inline Container value() const { return std::move(value_); }
};

// Visiting
template <class Container> struct const_visitor_adapter {
  virtual void apply(std::nullptr_t) {}
  virtual void apply(typename Container::object_type const&) {}
  virtual void apply(typename Container::array_type const&) {}
  virtual void apply(typename Container::str_type const&) {}
  virtual void apply(typename Container::float_type) {}
  virtual void apply(typename Container::int_type) {}
  virtual void apply(typename Container::uint_type) {}
  virtual void apply(bool) {}
};
*/

}  // namespace json_backbone

#endif  // NESTED_COMPILER_BASE_HEADER
