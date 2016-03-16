#include <json_backbone.hpp>
#include <catch.hpp>
#include <chrono>
#include <iostream>
#include <list>
#include <vector>
#include <map>
#include <type_traits>
//#include <json_backbone/externalize.hpp>

// using json_container = json_bac
using namespace json_backbone;

using json_container = container<std::map, std::vector, std::string, std::nullptr_t, bool,
                                 unsigned int, int, double, std::string>;

TEST_CASE("Container - access", "[container][access][runtime]") {
  json_container c{
      json_container::object_type{{{"name", "Roger"},     //
                                   {"size", 1.92},        //
                                   {"subscribed", true},  //
                                   {
                                       "children",                                        //
                                       json_container::array_type{"Martha", "Jesabelle"}  //
                                   }}}};

  SECTION("operators []") {
    auto& c1 = c["name"];
    auto& c2 = c["size"];
    auto& c3 = c["subscribed"];
    auto& c4 = c["children"];

    REQUIRE(c1.is<std::string>());
    REQUIRE(c2.is<double>());
    REQUIRE(c3.is<bool>());
    REQUIRE(c4.is<json_container::array_type>());

    REQUIRE(std::string(c1) == "Roger");
    REQUIRE(double(c2) == 1.92);
    REQUIRE(c3);
    REQUIRE(c4.get<json_container::array_type>().size());

    auto& c5 = c4[1];
    REQUIRE(c5.is<std::string>());
    REQUIRE(std::string(c5) == "Jesabelle");
  }
}
