CppNestedContainer
==================

`nested_container` is a dynamic C++11 container to hold dynamically structured data. `nested_container` is easy to use, generic, type safe and light in memory.

```c++
#include <nested_container/container.hpp>  // Requires C++11
#include <nested_container/driver/json/json.hpp>  // Requires boost::spirit
#include <string>
#include <iostream>

using nested_container::container;  // Default container with default types
template <typename C> using json = nested_container::json::serializer<C>;

int main(void) {
  using _ = nested_container::attr_init<container>;
  container c = {
    _("nom") = "Roger",
    _("prenom") = "Marcel",
    _("attributs") = {
      _("poids") = 95u,
      _("liste") = {1,2.f,"yeah"}
    }
  };

  json<container> serializer;
  std::string json_str = serializer.serialize(c);
  container parsed_c = serializer.deserialize(json_str);
  
  if (parsed_c["attributs"]["poids"].is_uint()) {
    std::cout << parsed_c["attributs"]["liste"][2].ref_string() << std::endl;
    return 0;
  }

  return 1;
}
```

# Introduction

The aim of `nested_container` is to reproduce the ease of use of some dynamic languages (JSON objects in Javascript, dictionaries in Python, arrays in PHP) in C++ while keeping good performances and type safety. A `nested_container` can hold eight type of data :

* A `null` value
* An associative container (default is `std::map`)
* A dynamic array (default is `std::vector`)
* A string (default is `std::string`)
* A floating point number (default is `float`)
* An integer (default is `int`)
* An unsigned integer (default is `unsigned int`)
* A boolean

Since accessing dynamically typed data in C++ can be done in a lot of different ways, particularly regarding the error cases, `nested_container` tries to provide enough of them to match every use and constraint.

#### Note to C++ developers

If you are an experienced C++ developer, you may feel uncomfortable with `nested_container`, and thinking of type erasure and other kinky concepts. Lets put things clear : `nested_container` has been written to ease the communication between C++ code and its external world, meaning here other languages and messaging protocols. It is focused on small size datasets. Yes, a beginner could (easily) write unmaintainable and unefficient code by using it in the wrong place. Dont blame the tool for allowing bad use, C++ allows a lot of bad uses of its own features.

# Examples

Code talks better than any explanation.

## Basic use

```c++
#include <nested_container/container.hpp>
#include <string>

int main(void) {
  using nested_container::container;  // Default container with default types
  container c;  // Is null
  c = "Roger"; 
  c = 7.f; 
  c = -5;
  c = 6u;
  c = true;
  // c = 2.;  // is a double, would not compile !
  
  c["name"] = "Marcel";  // Internal type is transformed to a map
  c["weight"] = 123u;
  
  c[0] = "Marcel";  // Internal type is transformed to a vector
  c[1] = "Robert";
}
```

## Braced initialisation

```c++
#include <nested_container/container.hpp>
#include <string>
#include <iostream>

int main(void) {
  using nested_container::container;  // Default container with default types
  using _ = nested_container::attr_init<container>;

  container c = {
    _("nom") = "Roger",
    _("prenom") = "Marcel",
    _("attributs") = {
      _("poids") = 95u,
      _("liste") = {1,2.f,"yeah"}
    }
  };
  
  std::cout << c["attributs"]["liste"][2].ref_string() << std::endl;
}
```

## Accessing data

The example is shown for a string, but every other contained type can be accessed the same way.

```c++
#include <nested_container/container.hpp>
#include <string>
#include <iostream>

int main(void) {
  using nested_container::container;  // Default container with default types
  container c = 3;
  {
    std::string s1 = c;  // c content is converted to a string if possible then returned
    std::string& s2 = c.ref_string();  // Return a std::string& is c contains a string, throws otherwise
    std::string* ps3 = c.get_string();  // Return a pointer a std::string const if c contains a string, nullptr otherwise
    std::string s4;
    bool success = c.get_string(s4);  // Returns true and affects s4 if c contains a string, returns false otherwise
    std::string& s5 = c.raw_string(); // Directly accesses the underlying data. If you know what you are doing
    bool is_string = c.is_string();  // Returns true if c contains a string
    std::string s6 = c.as_string();  // c content is converted to a string if possible then returned
    std::string& s7 = c.transform_string();  // c content is converted and stored as a string, then returned
  }
  
  // Templated versions are available, static_assert guards you from misuse
  {
    std::string& s2 = c.ref<std::string>();
    std::string* ps3 = c.get<std::string>();
    std::string s4;
    bool success = c.get<std::string>(s4);
    std::string& s5 = c.raw<std::string>();
    std::string s6 = c.as<std::string>();
    std::string& s7 = c.transform<std::string>();
  }
  
  // const versions are available too
  container const &c2 = c;
  {
    std::string s1 = c2;
    std::string const& s2 = c2.ref_string();
    std::string const * ps3 = c2.get_string();
    std::string s4;
    bool success = c2.get(s4);
    std::string const& s5 = c2.raw_string();
    bool is_string = c2.is_string();
    std::string s6 = c2.as_string();
    // "transform" member functions cannot be const
  }
}
```

# Serialization

`nested_container` has been written to help serialize and deserialize data. 

## JSON

A JSON parser and serializer is provided. Different parsing and serializaing strategies are available, see the test sources for more information. The best one may depend on your particular data.

### Parsing JSON

The JSON parser directly converts numbers : a number with a dot will be a floating point, a positive integer will be unsigned, a negative integer will be signed.

```c++
#include <nested_container/container.hpp>
#include <nested_container/driver/json/json.hpp>
#include <string>
#include <iostream>

using nested_container::container;
template <typename C> using json = nested_container::json::serializer<C>;

int main(void) {
  std::string input = R"json({"test1":1, "test2":1.1, "test3":["test1","test2",1.1,2,-4], "test4":true, "test5":null, "test6":{}})json";
  json<container> serializer;
  container c = serializer.deserialize(input);
  std::cout << c["test3"][2].ref_float() << std::endl;
}
```

### Dumping JSON

```c++
#include <nested_container/container.hpp>
#include <nested_container/driver/json/json.hpp>
#include <iostream>

using nested_container::container;
template <typename C> using json = nested_container::json::serializer<C>;

int main(void) {
  using _ = nested_container::attr_init<container>;
  container c = {_("nom") = "Roger", _("prenom") = "Marcel", _("attributs") = { _("poids") = 95u, _("liste") = {1,2.f,"yeah"}}};
  json<container> serializer;
  std::cout << serializer.serialize(c) << std::endl;
}
```

# Externalisation

Compilation can be long, particularly when you use the JSON driver which is based on `boost::spirit` a great tool but slow to compile due to the highly templated architecture. `nested_container` is provided with macros to help you externalize the templates and save compile time, link time and disk space.

#### To instantiate the templates

```c++
#include <nested_container/container.hpp>
#include <nested_container/externalize.hpp>
#include <nested_container/driver/json/json.hpp>
#include <nested_container/driver/json/externalize_json.hpp>

NESTED_CONTAINER_INSTANTIATE(NESTED_CONTAINER_CONTAINER_SIGNATURE());
NESTED_CONTAINER_INSTANTIATE_JSON(NESTED_CONTAINER_CONTAINER_SIGNATURE());
```

#### To use them without instantiating them

```c++
#include <nested_container/container.hpp>
#include <nested_container/externalize.hpp>
#include <nested_container/driver/json/json_forward.hpp>
#include <nested_container/driver/json/externalize_json.hpp>

NESTED_CONTAINER_EXTERNALIZE(NESTED_CONTAINER_CONTAINER_SIGNATURE());
NESTED_CONTAINER_EXTERNALIZE_JSON(NESTED_CONTAINER_CONTAINER_SIGNATURE());
```


# More to come

`nested_container` is still under active development, improvements are to come. Some flaws or bugs may remain, but the container and the serializer have been tested on huge randomized datasets.

Some ideas:
* Genericity : `std::wstring` has not been tested 
* Key ordering : when creating network messages, key order in a message can play a role. A specific underlying collection will be provided to bring this feature.
* Support for "strict" containers : trying to change the type after initialisation would be considered an error. Accessing on non existing key would be an error.
* UTF-8 : supported in theory but not tested at all.

## Other drivers

Other drivers are planned:

* BJSON driver
* Lua driver
* Python driver

Suggested by others:

* Log file reader

# Contact

Any feedback, critic, bug report or idea to improve it is welcome.

