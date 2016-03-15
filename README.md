Json backbone
==================

`json_backbone` is a C++14 container to hold dynamically structured data. `json_backbone` is easy to use, generic and type safe. 

`json_backbone` is made of three main parts:
* A versatile `variant` type optimized for small types.
* An helper to create recursive variants through a *Associative* container and a *RandomAccess* container.
* A view interface to loosely visit a structure without exceptions.

As the project just got a full rewrite, documentation will come progressively. A quick example :

```c++
#include <json_backbone.hpp>
#include <vector>
#include <map>
#include <string>

using namespace json_backbone;

using json_container = container<std::map,       // User's choice of associative container
                                 std::vector,    // User's choice of random access container
                                 std::string,    // key_type for the associative container
                                 std::string,    // A type an element could take
                                 double,         // A type an element could take
                                 int,            // A type an element could take
                                 bool,           // A type an element could take
                                 std::nullptr_t  // A type an element could take
                                 >;


void demo1() {
  json_container c1;
  json_container c2{"Roger"};  // Automatically maps to compatible ctor of std::string
  json_container c3{1.0};

  auto s1 = get<std::string>(c2);  // s1 is a string
  c3 = "Marcel";                   // c3 becomes a string
  c2 = true;                       // c2 becomes a bool
}

int main(void) {
  demo1();
  return 0;
}
```

`json_backbone` is shipped as a single header without any library to build for use.
