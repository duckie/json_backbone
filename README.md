Json backbone
==================

----------


`json_backbone` is a C++14 container to hold dynamically structured data. `json_backbone` is easy to use, generic and type safe.

`json_backbone` is made of three main parts:
* A versatile `variant` type optimized for small types.
* An helper to create recursive variants through a *Associative* container and a *RandomAccess* container.
* A view interface to loosely visit a structure without exceptions.

`json_backbone` can be used to built JSON support, BSON support or any other reduced or extended versions of those. `json_backbone` is shipped as a single header without any library to build. The implementation do not use RTTI nor any macro trick.

As the project just got a full rewrite, documentation will come progressively. An example :

```c++
#include <json_backbone.hpp>
#include <vector>
#include <map>
#include <string>

using namespace json_backbone;

// Declare a container specifically tailored for JSON data
using json_container = container<std::map,        // User's choice of associative container
                                 std::vector,     // User's choice of random access container
                                 std::string,     // key_type for the associative container
                                 std::nullptr_t,  // A type an element could take
                                 bool,            // A type an element could take
                                 int,             // A type an element could take
                                 double,          // A type an element could take
                                 std::string      // A type an element could take
                                 >;


// Create a helper to clarify creation syntax
element_init<json_container> operator""_a(char const* name, size_t length) {
  return json_container::key_type{name, length};
}

// Define a visitor helper
struct recursive_printer
    : public const_func_aggregate_visitor<json_container, recursive_printer const&, std::ostream&> {
  using const_func_aggregate_visitor<json_container, recursive_printer const&,
                                     std::ostream&>::const_func_aggregate_visitor;
};

int main(void) {
  // Create a container
  auto c = make_object({"name"_a = "Roger",     //
                        "size"_a = 1.92,        //
                        "subscribed"_a = true,  //
                        "children"_a = make_array({make_object({
                                                       "name"_a = "Martha",  //
                                                       "age"_a = 6           //
                                                   }),
                                                   make_object({
                                                       "name"_a = "Jesabelle",  //
                                                       "age"_a = 8              //
                                                   })}),
                        "grades"_a = make_array<json_container>({1, true, "Ole"})});

  // Play with it
  auto s1 = get<std::string>(c["name"]);  // is a string
  c["firstname"] = nullptr;               // Creates a null element
  c["firstname"] = "Marcel";              // This element becomes a string

  // Dump to json
  std::ostringstream result_stream;
  // Instantiate statically to avoid multiple creationc cost
  static recursive_printer const printer{
      [](std::nullptr_t val, auto&, auto& out) { out << "null"; },
      [](bool val, auto&, auto& out) { out << (val ? "true" : "false"); },
      [](int val, auto&, auto& out) { out << val; },
      [](double val, auto&, auto& out) { out << val; },
      [](auto const& str, auto&, auto& out) { out << '"' << str << '"'; },
      [](auto const& arr, auto& self, auto& out) {
        out << "[";
        bool sep = false;
        for (auto& v : arr) {
          out << (sep ? "," : "");
          apply_visitor(v, self, self, out);
          sep = true;
        }
        out << "]";
      },
      [](auto const& obj, auto& self, auto& out) {
        out << "{";
        bool sep = false;
        for (auto& v : obj) {
          out << (sep ? "," : "") << "\"" << v.first << "\":";
          apply_visitor(v.second, self, self, out);
          sep = true;
        }
        out << "}";
      }};

  // Apply the visitor. "printer" appears two times, once as the actual
  // visitor and once as an extra argument passed to it along with the stream
  apply_visitor(c, printer, printer, result_stream);
  std::cout << result_stream.str() << std::endl;
  return 0;
}
```

# Introduction

`json_backbone` comes from a frustrating situation in C++ when it comes to JSON and its siblings. Implementing a dynamic structure is a matter of trade-offs. Each library on the market makes its own. If you choose a library for its satisfying parsing performances, the memory layout may not suit your use-case. Whatever you do, if you change your mind, it means migrating your code to a new API, or writing a implementation of the abstraction layer you spent so much time to write, or just write your very own implementation.

The goal of `json_backbone` is to let you make the trade-offs while keeping a consistent API, whatever your choices are.

# `json_backbone::variant`

Most of `json_backbone` features are implemented through a discriminated union optimized for small types. This `variant`is similar to `boost::variant`but differs from it on a variety of topics.

The declaration is the same. Let's use and example :

```
using variant_t = json_backbone::variant<std::nullptr_t, int, std::string>;
```

Constraints on bounded types:
* Bounded types must be *CopyConstructible* and *MoveConstructible*.
* Complete at the point of the `variant`instantiation.

The `variant`:
* is *DefaultConstructible* if at least one of the bounded types is default constructible.
* is *CopyConstructible* and *MoveConstructible*.
* is *Assignable* from every *Assignable* bounded types and every type convertible to a bounded type.
* is *MoveAssignable* from every *MoveAssignable* bounded types and every type convertible to a bounded type.
* is not *EqualityComparable*.
* is not *LessThanComparable*.
* is not *OutputStreamable*
* is not *Hashable*
* is implicitely convertible to any of its bounded types.
* supports any constructor supported by one of its bounded types.

Rules to select assignable types and valid constructors will be detailed in a dedicated section.

## Small types optimizations

By default, the `variant`stores small types on the stack and other ones on the heap. A mix of small and big types may be used transparently.

By default, a small type is any type `T`verifying  ``sizeof(T) <= sizeof(void*)``.

This behavior can be customized by the user. Suppose we are using a type such that``sizeof(void*) < sizeof(BigType)`` and we define this variant:

```
using small_variant_t = variant<std::nullptr_t,BigType>;
```

In the context of this variant, any `BigType` value would not be considered a small type and would be allocated on the heap. You can override this behavior by specializing a template:

```
namespace json_backbone {
template <> struct is_small_type<BigType> { static constexpr bool value = true; }
}
```

Now, any variant using `BigType` will consider it a small type and allocate it on the stack. Moreover, any other type in those variants which size is less than or equal to `sizeof(BigType)` will be optimized as a small type too, even though it was not explicitely requested:

```c++
struct BigType {
  char data[128];
};

struct BigType2 {
  BigType data;
};

namespace json_backbone {
template <> struct is_small_type<BigType> {
  static constexpr bool value = true;
};
}

int main(void) {
  std::cout
    << sizeof(variant<BigType,BigType2>) // BigType2 values allocated on stack
    << " != "
    << sizeof(variant<BigType2>) // BigType2 values allocated on heap
    << std::endl;
  return 0;
}
```
