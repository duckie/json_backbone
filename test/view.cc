#include <iostream>
#include <json_backbone.hpp>
#include <catch.hpp>
#include <chrono>
#include <sstream>
#include <list>
#include <vector>
#include <map>
#include <type_traits>
//#include <json_backbone/externalize.hpp>

// using json_container = json_bac
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

namespace {
element_init<json_container> operator""_a(char const* name, size_t length) {
  return json_container::key_type{name, length};
}
}

TEST_CASE("View - access", "[view][access][runtime]") {
  json_container c{
      json_container::object_type{{{"name", "Roger"},     //
                                   {"size", 1.92},        //
                                   {"subscribed", true},  //
                                   {
                                       "children",                                        //
                                       json_container::array_type{"Martha", "Jesabelle"}  //
                                   }}}};

  auto v = make_view(c);
  SECTION("operators []") {
    auto v1 = v["name"];
    auto v2 = v["size"];
    auto v3 = v["subscribed"];
    auto v4 = v["children"];

    REQUIRE(v1.is<std::string>());
    REQUIRE(v2.is<double>());
    REQUIRE(v3.is<bool>());
    REQUIRE(v4.is<json_container::array_type>());

    REQUIRE(v1.get<std::string>() == "Roger");
    REQUIRE(std::string(v1) == "Roger");
    REQUIRE(double(v2) == 1.92);
    REQUIRE((v3 ? true : false));  // Odd but needed for GCC to compile here
    REQUIRE(v4.get<json_container::array_type>().size());

    auto v5 = v4[1];
    REQUIRE(v5.is<std::string>());
    REQUIRE(std::string(v5) == "Jesabelle");
  }
}
