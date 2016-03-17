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

element_init<json_container> operator""_a(char const* name, size_t length) {
  return json_container::key_type{name, length};
}

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

TEST_CASE("Container - creation", "[container][access][runtime]") {
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

  SECTION("operators []") {
    c["children"][1]["name"] = "Jesabel";
    c["children"][1]["size"] = 1.28;

    REQUIRE(c.is<json_container::object_type>());
    REQUIRE(c["children"].is<json_container::array_type>());
    REQUIRE(c["children"].get_array().size() == 2);
    REQUIRE(c["children"].get_array()[1]["name"].is<std::string>());
    REQUIRE(c["children"].get_array()[1]["age"].is<int>());
    REQUIRE(c["children"].get_array()[1]["size"].is<double>());

    REQUIRE(c["children"].get_array()[1]["name"].get<std::string>() == "Jesabel");
    REQUIRE(c["children"].get_array()[1]["age"].get<int>() == 8);
    REQUIRE(c["children"].get_array()[1]["size"].get<double>() == 1.28);
  }
}
