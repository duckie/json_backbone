Json backbone
==================

`json_backbone` is a C++14 container to hold dynamically structured data. `json_backbone` is easy to use, generic and type safe.

`json_backbone` is made of three main parts:
* A versatile `variant` type optimized for small types.
* An helper to create recursive variants through a *Associative* container and a *RandomAccess* container.
* A view interface to loosely visit a structure without exceptions.

`json_backbone` can be used to built JSON support, BSON support or any other reduced or extended versions of those. `json_backbone` is shipped as a single header without any library to build. The implementation do not use RTTI nor any pre-processor trick.

`json_backbone` is known to compile on:
* GCC 5.3.0
* Clang 3.7.1
* MSVC 2015 Update 2 (00322-20000-00000-AA744)

A quick example :

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
  return 0;
}
```

`json_backbone` comes from a frustrating situation in C++ when it comes to JSON and its siblings. Implementing a dynamic structure is a matter of trade-offs. Each library on the market makes its own. If you choose a library for its satisfying parsing performances, the memory layout may not suit your use-case. Whatever you do, if you change your mind, it means migrating your code to a new API, or writing a implementation of the abstraction layer you spent so much time to write, or just write your very own implementation. `json_backbone` aims at letting you make the trade-offs while keeping a consistent and reusable API. To learn more, head to the [documentation](doc/md/main.md).

## Documentation

[Available here](doc/md/main.md).

## Build

Just include `json_backbone.hpp` in your file and you can go.

## Build an run tests

```bash
git clone https://github.com/duckie/json_backbone.git
cd json_backbone
git submodule update --init --recursive # To get the CATCH dependency
mkdir build
cd build
cmake ../ # -GNinja to use ninja instead of make
make
make test
``` 

## Contribute

The project is still on active developpement. Help is welcome.
