#ifndef JSON_BACKBONE_BASE_HEADER
#define JSON_BACKBONE_BASE_HEADER
#include <type_traits>
#include <utility>
#include <functional>
#include <exception>
#include <limits>
#include <initializer_list>

namespace json_backbone {

// template <class key_type, class value_type>
// using std_object_default_allocators = std::map<key_type, value_type>;
// template <class value_type>
// using std_array_default_allocators = std::vector<value_type>;
template <class Container>
class element_init;

template <class Container>
class array_element_init;

//
// bounded_type_traits says if a type is small or is complete or not
//
// User can specialize this traits to force a type to be considered small
// even if it would not in the first place.
// Example : double in 32bits platforms will not be considered small
// but maybe you would like it to do so.
// is_small_type is written in old SFINAE style to support incomplete type
// thus enabling automatic recrusion without a recursive_wrapper
//
template <typename T>
struct bounded_type_traits {
  template <typename U>
  static auto test_is_small(U*) -> std::integral_constant<bool, sizeof(U) == sizeof(U)>;
  static auto test_is_small(...) -> std::false_type;
  static constexpr bool is_small_type = decltype(test_is_small((T*)0))::value;

  template <typename U>
  static auto resolve_size(U*) -> std::integral_constant<std::size_t, sizeof(U)>;
  static auto resolve_size(...) -> std::integral_constant<std::size_t, 0>;
  static constexpr std::size_t resolution_size = decltype(resolve_size((T*)0))::value;
  //static constexpr size_t resolution_size = decltype(test((T*)0))::value;
};

template <typename T>
struct is_small_type : std::integral_constant<bool, bounded_type_traits<T>::is_small_type> {
};

//
// memory_footprint_t represents minimum size needed to store the type
//
// This is the minimum size to be allocated in a container to hold
// this type wether it is a small type or not. Big types needs to
// be pointed to whiole small ones are directly stored
//
template <class T>
using memory_footprint_t =
    std::conditional_t<is_small_type<T>::value, std::integral_constant<std::size_t, bounded_type_traits<T>::resolution_size>,
                       std::integral_constant<std::size_t, sizeof(T*)>>;

//
// enable_if_small_t is enable_if_t when the type fits in the given size
//
template <class T, class Return, std::size_t MemSize>
using enable_if_small_t = std::enable_if_t<(0 < bounded_type_traits<T>::resolution_size && bounded_type_traits<T>::resolution_size <= MemSize), Return>;

//
// enable_if_big_t is enable_if_t when the type doesnt fit in the given size
//
template <class T, class Return, std::size_t MemSize>
using enable_if_big_t = std::enable_if_t<(0 == bounded_type_traits<T>::resolution_size || MemSize < bounded_type_traits<T>::resolution_size), Return>;

//
// arithmetics provides compiles times arithmetics over arrays
//
namespace arithmetics {

// max_value computes the higher value of an array at compile time
template <class I, std::size_t N>
I constexpr max_value(std::array<I, N> const& values,
                      I current_value = std::numeric_limits<I>::lowest(),
                      std::size_t current_index = 0) {
  return N <= current_index ? current_value
                            : (current_value < values.at(current_index)
                                   ? max_value(values, values.at(current_index), current_index + 1)
                                   : max_value(values, current_value, current_index + 1));
}

template <class I, std::size_t N>
std::size_t constexpr find_first(std::array<I, N> const& values, I value,
                                 std::size_t current_index = 0) {
  return N <= current_index ? N : value == values.at(current_index)
                                      ? current_index
                                      : find_first(values, value, current_index + 1);
}

template <class I, std::size_t N>
std::size_t constexpr find_last(std::array<I, N> const& values, I value,
                                std::size_t current_index = 0) {
  return N <= current_index ? N : value == values.at(N - current_index - 1)
                                      ? (N - current_index - 1)
                                      : find_last(values, value, current_index + 1);
}
};

//
// Utilities for computation on type lists
//
namespace type_list_traits {

// type_list forward declaration
template <class Indices, class... Types>
struct type_list;

// Utility type
template <class T>
struct type_holder {
  using type = T;
};

// Utility type
template <class Type, std::size_t Index>
struct type_info {};

//
// type_list offers compile time services on a type_list
//
// type_list is implemented with function overloads rather than than recursive
// templates to speed up compilation.
//
template <std::size_t... Is, class... Types>
struct type_list<std::index_sequence<Is...>, Types...> : type_info<Types, Is>... {
  //
  // Returns the index as integral constant of the given type in the list
  //
  // Not supposed to be called, implementation is just there to avoid
  // warnings
  //
  template <class Type, std::size_t Index>
  static constexpr std::integral_constant<std::size_t, Index> type_index(
      type_info<Type, Index> const&) {
    return {};
  }

  //
  // Sink member function to return a value for unsupported types
  //
  // Not supposed to be called, implementation is just there to avoid
  // warnings
  //
  template <class Type>
  static constexpr std::integral_constant<std::size_t, sizeof...(Types)> type_index(...) {
    return {};
  }

  //
  // Returns the type at a given index
  //
  // Not supposed to be called, implementation is just there to avoid
  // warnings
  //
  template <std::size_t Index, class Type>
  static constexpr type_holder<Type> get_type_at(type_info<Type, Index> const&) {
    return {};
  }

  //
  // Void sink for unsupported types
  //
  // Not supposed to be called, implementation is just there to avoid
  // warnings
  //
  template <std::size_t Index>
  static constexpr type_holder<void> get_type_at(...) {
    return {};
  }

  //
  // Returns index of type T in the list
  //
  // Returns size of the list if T not in the list
  //
  template <class T>
  static constexpr std::size_t get_index() {
    return decltype(type_index<T>(std::declval<type_list>()))::value;
  }

  //
  // Returns the type at a given index
  //
  // Not supposed to be called, implementation is just there to avoid
  // warnings
  //
  template <std::size_t Index>
  struct type_at {
    using type = typename decltype(get_type_at<Index>(std::declval<type_list>()))::type;
  };

  // Returns true if type T is in the list
  template <class T>
  static constexpr bool has_type() {
    return decltype(type_index<T>(std::declval<type_list>()))::value < sizeof...(Types);
  }

  //
  // Returns a type chosen for construction following those rules
  // - If there is a single argument either bool or nullptr_t, select the last
  //   type of the list constructible with it.
  // - If there is a single argument which is integral, select the first
  //   integral type of the list constructible with it
  // - If it fails, get the first type constructible in the list
  //
  // Constructibility covers convertibility, so is used for the sake
  // of implicit conversion too
  //
  template <std::size_t MemSize, class Arg, class... Args>
  struct select_constructible {
    static constexpr std::size_t index_first_same_value =
        arithmetics::find_first<bool, sizeof...(Types)>(
            {(std::is_same<std::decay_t<Arg>, Types>() &&
              std::is_constructible<Types, Arg, Args...>::value)...},
            true);
    static constexpr std::size_t index_first_integral_ssig_value =
        arithmetics::find_first<bool, sizeof...(Types)>(
            {(std::is_integral<Types>() && sizeof(Arg) <= sizeof(Types) &&
              std::is_signed<Types>() == std::is_signed<Arg>() &&
              std::is_constructible<Types, Arg, Args...>::value)...},
            true);
    static constexpr std::size_t index_first_integral_value =
        arithmetics::find_first<bool, sizeof...(Types)>(
            {(std::is_integral<Types>() && sizeof(Arg) <= sizeof(Types) &&
              std::is_constructible<Types, Arg, Args...>::value)...},
            true);
    static constexpr std::size_t index_first_arithmetic_value =
        arithmetics::find_first<bool, sizeof...(Types)>(
            {(std::is_arithmetic<Types>() && !std::is_integral<Types>() &&
              sizeof(Arg) <= sizeof(Types) &&
              std::is_constructible<Types, Arg, Args...>::value)...},
            true);
    static constexpr std::size_t index_first_ptrwise_value =
        arithmetics::find_first<bool, sizeof...(Types)>(
            {(!(std::is_integral<Types>() && std::is_pointer<std::decay_t<Arg>>() &&
                0 == sizeof...(Args)) &&
              std::is_constructible<Types, Arg, Args...>::value)...},
            true);
    static constexpr std::size_t index_first_value =
        arithmetics::find_first<bool, sizeof...(Types)>(
            {(std::is_constructible<Types, Arg, Args...>::value)...}, true);

    static constexpr std::size_t index_value =
        (index_first_same_value < sizeof...(Types) && 0 == sizeof...(Args))
            ? index_first_same_value
            : (std::is_integral<Arg>() && 0 == sizeof...(Args) &&
               index_first_integral_ssig_value < sizeof...(Types))
                  ? index_first_integral_ssig_value
                  : (std::is_integral<Arg>() && 0 == sizeof...(Args) &&
                     index_first_integral_value < sizeof...(Types))
                        ? index_first_integral_value
                        : (std::is_arithmetic<Arg>() && 0 == sizeof...(Args) &&
                           index_first_arithmetic_value < sizeof...(Types))
                              ? index_first_arithmetic_value
                              : (index_first_ptrwise_value < sizeof...(Types))
                                    ? index_first_ptrwise_value
                                    : index_first_value;
    using type = typename decltype(get_type_at<index_value>(std::declval<type_list>()))::type;
  };

  //
  // Returns a type chosen for default construction
  //
  // Rules:
  // - Select first default constructible type
  //
  template <std::size_t MemSize>
  struct select_default {
    static constexpr std::size_t index_first_value =
        arithmetics::find_first<bool, sizeof...(Types)>({std::is_default_constructible<Types>()...},
                                                        true);
    static constexpr std::size_t index_value = index_first_value;
    using type = typename decltype(get_type_at<index_value>(std::declval<type_list>()))::type;
  };
};
}  // namespace type_list_traits

//
// helpers creates functions to be used by the variant
//
namespace helpers {

// deleter_fp is a function that deletes a type - smalYl type version
template <class T, std::size_t MemSize>
enable_if_small_t<T, void, MemSize> deleter_fp(void** data) {
  reinterpret_cast<T*>(data)->~T();
}

// deleter_fp is a function that deletes a type - big type version
template <class T, std::size_t MemSize>
enable_if_big_t<T, void, MemSize> deleter_fp(void** data) {
  delete reinterpret_cast<T*>(*data);
}

}  // namespace helpers

// bad_type is thrown at runtime when accessing a container with the wrong type
template <class Container>
struct bad_type : public std::exception {
  char const* what() const noexcept override {
    return "Requested type does not match current type of the variant.";
  }
};

// non_existing_element is thrown when you try to get and element through [] but is does not exist
template <class Container>
struct non_existing_element : public std::exception {
  char const* what() const noexcept override {
    return "Requested element does not exist and the object is const.";
  }
};

// Forward declare friends

//
// variant is a discriminated union optimized for small types
//
template <class... Value>
class variant {
  // Compute minimum size required by types. Default 8
  static constexpr std::size_t min_memory_size =
      arithmetics::max_value<std::size_t, sizeof...(Value)>({memory_footprint_t<Value>::value...});

 public:
  // Compute memory size of an array of void* wide enough to hold min_memory_size
  static constexpr std::size_t memory_size =
      sizeof(void*) * (min_memory_size / sizeof(void*) + (min_memory_size % sizeof(void*) ? 1 : 0));

 private:
  std::size_t type_ = sizeof...(Value);
  void* data_[memory_size / sizeof(void*)] = {};

 public:
  using type_list_type =
      type_list_traits::type_list<std::make_index_sequence<sizeof...(Value)>, Value...>;

 private:
  //
  // Destroys currently held object and deallocates heap if needed
  //
  void clear() {
    static std::array<void (*)(void**), sizeof...(Value)> deleters = {
        helpers::deleter_fp<Value, memory_size>...};
    if (type_ < sizeof...(Value))  // Should only be the case in some ctors
      deleters[type_](&data_[0]);
  }

  // Fails to compile if type is not supported
  template <class T>
  inline void assert_has_type() const {
    static_assert(type_list_type::template has_type<std::decay_t<T>>(),
                  "Type T not supported by this container.");
  }

  template <class T, class Arg, class... Args,
            class Enabler = enable_if_small_t<T, void, memory_size>>
  void allocate(Arg&& arg, Args&&... args) {
    new (reinterpret_cast<T*>(&data_[0])) T(std::forward<Arg>(arg), std::forward<Args>(args)...);
  }

  template <class T, class Arg, class... Args,
            class Enabler = enable_if_big_t<T, void, memory_size>>
  void allocate(Arg&& arg, Args&&... args, void* shim = nullptr) {
    T** ptr = reinterpret_cast<T**>(&data_[0]);
    *ptr = new T(std::forward<Arg>(arg), std::forward<Args>(args)...);
  }

  template <class Arg, class... Args>
  void create(Arg&& arg, Args&&... args) {
    using target_type =
        typename type_list_type::template select_constructible<memory_size, Arg, Args...>::type;
    assert_has_type<target_type>();
    type_ = type_list_type::template get_index<std::decay_t<target_type>>();
    allocate<target_type>(std::forward<Arg>(arg), std::forward<Args>(args)...);
  }

  template <class T>
  static void copy_ctor_fp(variant& self, variant const& other) {
    self.create(other.template raw<T>());
  }

  template <class T>
  static void move_ctor_fp(variant& self, variant&& other) {
    self.create(std::move(other.template raw<T>()));
  }

  template <class T>
  static void copy_assign_fp(variant& self, variant const& other) {
    self.template get<T>() = other.template raw<T>();
  }

  template <class T>
  static void move_assign_fp(variant& self, variant&& other) {
    self.template get<T>() = std::move(other.template raw<T>());
  }

 public:
  template <bool HasDefault = (type_list_type::template select_default<memory_size>::index_value <
                               sizeof...(Value)),
            class Enabler = std::enable_if_t<HasDefault, void>>
  variant() {
    create(typename type_list_type::template select_default<memory_size>::type{});
  }

  variant(variant const& other) {
    static std::array<void (*)(variant&, variant const&), sizeof...(Value)> ctors = {
        copy_ctor_fp<Value>...};
    ctors[other.type_](*this, other);
  }

  variant(variant&& other) {
    static std::array<void (*)(variant&, variant&&), sizeof...(Value)> ctors = {
        move_ctor_fp<Value>...};
    ctors[other.type_](*this, std::move(other));
  }

  // Construction with a compatible constructor from a bounded type
  template <
      class Arg, class... Args,
      class Enabler = std::enable_if_t<
          !(std::is_base_of<variant, std::decay_t<Arg>>::value && 0 == sizeof...(Args)), void>>
  variant(Arg&& arg, Args&&... args) {
    static_assert(
        type_list_type::template select_constructible<memory_size, Arg, Args...>::index_value <
            sizeof...(Value),
        "Construction not supported by the variant.");
    create(typename type_list_type::template select_constructible<memory_size, Arg, Args...>::type{
        std::forward<Arg>(arg), std::forward<Args>(args)...});
  }

  // Non-virtual destructor to spare a useless virtual table
  ~variant() { clear(); }

  // Assign from other variant
  variant& operator=(variant const& other) {
    static std::array<void (*)(variant&, variant const&), sizeof...(Value)> stores = {
        copy_assign_fp<Value>...};

    static std::array<void (*)(variant&, variant const&), sizeof...(Value)> ctors = {
        copy_ctor_fp<Value>...};

    if (type_ == other.type_) {
      stores[type_](*this, other);
    } else {
      clear();
      ctors[other.type_](*this, other);
    }
    return *this;
  }

  variant& operator=(variant&& other) {
    static std::array<void (*)(variant&, variant&&), sizeof...(Value)> stores = {
        move_assign_fp<Value>...};

    static std::array<void (*)(variant&, variant&&), sizeof...(Value)> ctors = {
        move_ctor_fp<Value>...};

    if (type_ == other.type_) {
      stores[type_](*this, std::move(other));
    } else {
      clear();
      ctors[other.type_](*this, std::move(other));
    }
    return *this;
  }

  inline size_t type_index() const { return type_; }

  // Assignation from a bounded type
  template <class T, class Enabler = std::enable_if_t<
                         type_list_type::template has_type<std::decay_t<T>>(), void>>
  variant& assign(T&& value) {
    assert_has_type<T>();
    if (type_ == type_list_type::template get_index<std::decay_t<T>>()) {
      this->template raw<T>() = std::forward<T>(value);
    } else {
      clear();
      create<T>(std::forward<T>(value));
    }
    return *this;
  }

  // Assignation from a type convertible to one of the bounded types
  template <class T, class Enabler =
                         std::enable_if_t<!(type_list_type::template has_type<std::decay_t<T>>() ||
                                            std::is_base_of<variant, std::decay_t<T>>::value),
                                          void>>
  variant& assign(T&& value, void* shim = nullptr) {
    static_assert(type_list_type::template select_constructible<memory_size, T>::index_value <
                      sizeof...(Value),
                  "Assignation not supported by the variant.");
    return assign(typename type_list_type::template select_constructible<memory_size, T>::type{
        std::forward<T>(value)});
  }

  // Assignation operators dispatches call on the matching assign.
  template <class T, class Enabler =
                         std::enable_if_t<!std::is_base_of<variant, std::decay_t<T>>::value, void>>
  inline variant& operator=(T&& value) {
    return assign(std::forward<T>(value));
  }

  template <class T>
  inline bool is() const noexcept {
    assert_has_type<T>();
    return type_ == type_list_type::template get_index<std::decay_t<T>>();
  }

  // get checks the type is correct and returns it
  template <class T>
  enable_if_small_t<T, T&, memory_size> get() & {
    assert_has_type<T>();
    if (is<T>()) return *(reinterpret_cast<T*>(&data_[0]));
    throw bad_type<variant>{};
  }

  // get checks the type is correct and returns it
  template <class T>
  enable_if_big_t<T, T&, memory_size> get() & {
    assert_has_type<T>();
    if (is<T>()) return *(reinterpret_cast<T*>(data_[0]));
    throw bad_type<variant>{};
  }

  // get checks the type is correct and returns it
  template <class T>
  enable_if_small_t<T, T const&, memory_size> get() const & {
    assert_has_type<T>();
    if (is<T>()) return *(reinterpret_cast<T const*>(&data_[0]));
    throw bad_type<variant>{};
  }

  // get checks the type is correct and returns it
  template <class T>
  enable_if_big_t<T, T const&, memory_size> get() const & {
    assert_has_type<T>();
    if (is<T>()) return *(reinterpret_cast<T const*>(data_[0]));
    throw bad_type<variant>{};
  }

  // get checks the type is correct and returns it
  template <class T>
  enable_if_small_t<T, T, memory_size> get() && {
    assert_has_type<T>();
    if (is<T>()) return std::move(*(reinterpret_cast<T*>(&data_[0])));
    throw bad_type<variant>{};
  }

  // get checks the type is correct and returns it
  template <class T>
  enable_if_big_t<T, T, memory_size> get() && {
    assert_has_type<T>();
    if (is<T>()) return std::move(*(reinterpret_cast<T*>(data_[0])));
    throw bad_type<variant>{};
  }

  // raw returns directly without any check
  template <class T>
      inline enable_if_small_t<T, T&, memory_size> raw() & noexcept {
    assert_has_type<T>();
    return *(reinterpret_cast<T*>(&data_[0]));
  }

  // raw returns directly without any check
  template <class T>
      inline enable_if_big_t<T, T&, memory_size> raw() & noexcept {
    assert_has_type<T>();
    return *(reinterpret_cast<T*>(data_[0]));
  }

  // raw returns directly without any check
  template <class T>
  inline enable_if_small_t<T, T const&, memory_size> raw() const& noexcept {
    assert_has_type<T>();
    return *(reinterpret_cast<T const*>(&data_[0]));
  }

  // raw returns directly without any check
  template <class T>
  inline enable_if_big_t<T, T const&, memory_size> raw() const& noexcept {
    assert_has_type<T>();
    return *(reinterpret_cast<T const*>(data_[0]));
  }

  // raw returns directly without any check
  template <class T>
      inline enable_if_small_t<T, T, memory_size> raw() && noexcept {
    assert_has_type<T>();
    return std::move(*(reinterpret_cast<T*>(&data_[0])));
  }

  // raw returns directly without any check
  template <class T>
      inline enable_if_big_t<T, T, memory_size> raw() && noexcept {
    assert_has_type<T>();
    return std::move(*(reinterpret_cast<T*>(data_[0])));
  }

  // Conversion operator
  template <class T, class Enabler = std::enable_if_t<
                         type_list_type::template has_type<std::decay_t<T>>(), void>>
  operator T&() & {
    return this->get<T>();
  }

  template <class T, class Enabler = std::enable_if_t<
                         type_list_type::template has_type<std::decay_t<T>>(), void>>
  operator T const&() const & {
    return this->get<T>();
  }

  template <class T, class Enabler = std::enable_if_t<
                         type_list_type::template has_type<std::decay_t<T>>(), void>>
  operator T() && {
    return std::move(this->get<T>());
  }
};

// Functional version of is
template <class T, class... Value>
bool is(variant<Value...> const& value) {
  return value.template is<T>();
}

// Functional version of get
template <class T, class... Value>
T get(variant<Value...>& value) {
  return value.template get<T>();
}

// Functional version of get
template <class T, class... Value>
T const& get(variant<Value...> const& value) {
  return value.template get<T>();
}

// Functional version of get
template <class T, class... Value>
T get(variant<Value...>&& value) {
  return std::move(value.template get<T>());
}

// Functional version of raw
template <class T, class... Value>
inline decltype(auto) raw(variant<Value...>& value) noexcept {
  return value.template raw<T>();
}

// Functional version of raw
template <class T, class... Value>
inline decltype(auto) raw(variant<Value...> const& value) noexcept {
  return value.template raw<T>();
}

// Functional version of raw
template <class T, class... Value>
inline decltype(auto) raw(variant<Value...>&& value) noexcept {
  return std::move(value.template raw<T>());
}

//
// base_container is an variant extended with an associative container and a random access container
//
template <template <class...> class ObjectBase, template <class...> class ArrayBase, class Key,
          class... Value>
class container
    : public variant<Value..., ArrayBase<container<ObjectBase, ArrayBase, Key, Value...>>,
                     ObjectBase<Key, container<ObjectBase, ArrayBase, Key, Value...>>> {
 public:
  using variant_type = variant<Value..., ArrayBase<container<ObjectBase, ArrayBase, Key, Value...>>,
                               ObjectBase<Key, container<ObjectBase, ArrayBase, Key, Value...>>>;
  using object_type = ObjectBase<Key, container>;
  using array_type = ArrayBase<container>;
  using key_type = Key;
  using value_type_list_type =
      type_list_traits::type_list<std::make_index_sequence<sizeof...(Value)>, Value...>;

  container() : variant_type{} {}

  container(container const& value) : variant_type(value) {}

  container(container&& value) : variant_type(std::move(value)) {}

  container& operator=(container const& value) {
    variant_type::operator=(value);
    return *this;
  }

  container& operator=(container&& value) {
    variant_type::operator=(std::move(value));
    return *this;
  }

  template <class Arg, class... Args>
  container(Arg&& arg, Args&&... args)
      : variant_type(std::forward<Arg>(arg), std::forward<Args>(args)...) {}

  inline object_type& get_object() & { return this->template get<object_type>(); }

  inline object_type const& get_object() const & { return this->template get<object_type>(); }

  inline object_type get_object() && { return std::move(this->template get<object_type>()); }

  inline array_type& get_array() & { return this->template get<array_type>(); }

  inline array_type const& get_array() const & { return this->template get<array_type>(); }

  inline array_type get_array() && { return std::move(this->template get<array_type>()); }

  container& operator[](size_t value) & { return this->template get<array_type>()[value]; }

  container const& operator[](size_t value) const & {
    return this->template get<array_type>()[value];
  }

  container operator[](size_t value) && { return this->template get<array_type>()[value]; }

  template <class T, class Enabler = std::enable_if_t<!std::is_integral<std::decay_t<T>>(), void>>
  container& operator[](T&& value) & {
    return this->template get<object_type>()[std::forward<T>(value)];
  }

  template <class T, class Enabler = std::enable_if_t<!std::is_integral<std::decay_t<T>>(), void>>
  container const& operator[](T&& value) const & {
    auto it = this->template get<object_type>().find(std::forward<T>(value));
    if (it != this->template get<object_type>().end()) { return it->second; }
    throw non_existing_element<container>{};
  }

  template <class T, class Enabler = std::enable_if_t<!std::is_integral<std::decay_t<T>>(), void>>
  container operator[](T&& value) && {
    return std::move(this->template get<object_type>()[std::forward<T>(value)]);
  }
};

template <class Container>
class element_init {
  typename Container::object_type::key_type key_;
  Container value_;

 public:
  using container_type = Container;
  element_init(typename Container::object_type::key_type const& key) : key_{key} {}
  element_init(typename Container::object_type::key_type&& key) : key_{std::move(key)} {}
  template <class T>
  typename Container::object_type::value_type operator=(T&& value) && {
    return {std::move(key_), std::move(value)};
  }
};

template <class Container, class Key = typename Container::key_type>
Container make_object(std::initializer_list<std::pair<Key const, Container>>&& elements) {
  return typename Container::object_type{std::move(elements)};
}

template <class Container>
Container make_array(std::initializer_list<Container>&& elements) {
  return typename Container::array_type{std::move(elements)};
}

namespace visiting_helpers {
// Applier_amker generates function pointers
template <class... Value>
struct applier_maker;
template <class... Value>
struct applier_maker<variant<Value...>> {
  template <class Visitor, class T, class... ExtraArguments>
  static void applier_fp(variant<Value...>& values, Visitor& visitor, ExtraArguments&&... extras) {
    visitor(values.template raw<T>(), std::forward<ExtraArguments>(extras)...);
  }

  template <class Visitor, class T, class... ExtraArguments>
  static void const_applier_fp(variant<Value...> const& values, Visitor const& visitor,
                               ExtraArguments&&... extras) {
    visitor(values.template raw<T>(), std::forward<ExtraArguments>(extras)...);
  }
};
// Extend to container
template <template <class...> class Object, template <class...> class Array, class Key,
          class... Value>
struct applier_maker<container<Object, Array, Key, Value...>>
    : public applier_maker<typename container<Object, Array, Key, Value...>::variant_type> {
  using container_type = container<Object, Array, Key, Value...>;
  using applier_maker<typename container_type::variant_type>::applier_maker;
};
}  // namespace visiting_helpers

template <class Visitor, class... Value, class... ExtraArguments>
void apply_visitor(variant<Value...>& values, Visitor& visitor, ExtraArguments&&... extras) {
  static std::array<void (*)(variant<Value...>&, Visitor&, ExtraArguments...), sizeof...(Value)>
      appliers = {visiting_helpers::applier_maker<variant<Value...>>::template applier_fp<
          Visitor&, Value, ExtraArguments...>...};
  appliers[values.type_index()](values, visitor, std::forward<ExtraArguments>(extras)...);
};

template <class Visitor, class... Value, class... ExtraArguments>
void apply_visitor(variant<Value...>& values, Visitor const& visitor, ExtraArguments&&... extras) {
  static std::array<void (*)(variant<Value...>&, Visitor const&, ExtraArguments...),
                    sizeof...(Value)> appliers = {
      visiting_helpers::applier_maker<variant<Value...>>::template applier_fp<
          Visitor const&, Value, ExtraArguments...>...};
  appliers[values.type_index()](values, visitor, std::forward<ExtraArguments>(extras)...);
};

template <class Visitor, class... Value, class... ExtraArguments>
void apply_visitor(variant<Value...> const& values, Visitor& visitor, ExtraArguments&&... extras) {
  static std::array<void (*)(variant<Value...> const&, Visitor&, ExtraArguments...),
                    sizeof...(Value)> appliers = {
      visiting_helpers::applier_maker<variant<Value...>>::template const_applier_fp<
          Visitor&, Value, ExtraArguments...>...};
  appliers[values.type_index()](values, visitor, std::forward<ExtraArguments>(extras)...);
};

template <class Visitor, class... Value, class... ExtraArguments>
void apply_visitor(variant<Value...> const& values, Visitor const& visitor,
                   ExtraArguments&&... extras) {
  static std::array<void (*)(variant<Value...> const&, Visitor const&, ExtraArguments...),
                    sizeof...(Value)> appliers = {
      visiting_helpers::applier_maker<variant<Value...>>::template const_applier_fp<
          Visitor const&, Value, ExtraArguments...>...};
  appliers[values.type_index()](values, visitor, std::forward<ExtraArguments>(extras)...);
};

//
// Visitor generated with functions
//
// This allows to write complex visitation without defininf an object
// There is a performance penaly though because of std::functions
//
template <class... Value>
struct func_aggregate_visitor;
template <class... Value, class... ExtraArguments>
struct func_aggregate_visitor<variant<Value...>, ExtraArguments...> {
  using variant_type = variant<Value...>;
  std::tuple<std::function<void(Value&, ExtraArguments...)>...> appliers;
  func_aggregate_visitor(std::function<void(Value&, ExtraArguments...)>... applier)
      : appliers{applier...} {}
  template <class T>
  void operator()(T&& value, ExtraArguments... extras) const {
    std::get<variant_type::type_list_type::template get_index<std::decay_t<T>>()>(appliers)(
        std::forward<T>(value), extras...);
  }
};
template <template <class...> class Object, template <class...> class Array, class Key,
          class... Value, class... ExtraArguments>
struct func_aggregate_visitor<container<Object, Array, Key, Value...>, ExtraArguments...>
    : public func_aggregate_visitor<typename container<Object, Array, Key, Value...>::variant_type,
                                    ExtraArguments...> {
  using container_type = container<Object, Array, Key, Value...>;
  using func_aggregate_visitor<typename container_type::variant_type,
                               ExtraArguments...>::func_aggregate_visitor;
};

//
// Constant visitor generated with functions
//
// This allows to write complex visitation without defininf an object
// There is a performance penaly though because of std::functions
//
template <class... Value>
struct const_func_aggregate_visitor;
template <class... Value, class... ExtraArguments>
struct const_func_aggregate_visitor<variant<Value...>, ExtraArguments...> {
  using variant_type = variant<Value...>;
  std::tuple<std::function<void(Value const&, ExtraArguments...)>...> appliers;
  const_func_aggregate_visitor(std::function<void(Value const&, ExtraArguments...)>... applier)
      : appliers{applier...} {}
  template <class T>
  void operator()(T&& value, ExtraArguments... extras) const {
    std::get<variant_type::type_list_type::template get_index<std::decay_t<T>>()>(appliers)(
        std::forward<T>(value), extras...);
  }
};
template <template <class...> class Object, template <class...> class Array, class Key,
          class... Value, class... ExtraArguments>
struct const_func_aggregate_visitor<container<Object, Array, Key, Value...>, ExtraArguments...>
    : public const_func_aggregate_visitor<
          typename container<Object, Array, Key, Value...>::variant_type, ExtraArguments...> {
  using container_type = container<Object, Array, Key, Value...>;
  using const_func_aggregate_visitor<typename container_type::variant_type,
                                     ExtraArguments...>::const_func_aggregate_visitor;
};

// Creation helper for func_aggregate_visitor
template <class... Value>
func_aggregate_visitor<variant<Value...>> make_visitor(std::function<void(Value&)>... action) {
  return {action...};
}

// creation helper for const_func_aggregate_visitor
template <class... Value>
const_func_aggregate_visitor<variant<Value...>> make_visitor(
    std::function<void(Value const&)>... action) {
  return {action...};
}

}  // namespace json_backbone

#endif  // NESTED_COMPILER_BASE_HEADER
