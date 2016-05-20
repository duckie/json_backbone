#include <json_backbone.hpp>
#include <catch.hpp>
#include <chrono>
#include <list>
#include <vector>
#include <map>
#include <type_traits>

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

TEST_CASE("View - converter", "[view][converter]") {
  REQUIRE(base_converter::convert<int>(1.0) == 1);
  REQUIRE(base_converter::convert<std::string>(1.0) == "");
  REQUIRE(base_converter::convert<int>(std::string("1")) == 0);
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
    REQUIRE(v4.get<json_container::array_type>().size() != 0);

    auto v5 = v4[1];
    REQUIRE(v5.is<std::string>());
    REQUIRE(std::string(v5) == "Jesabelle");
  }

  SECTION("conversion") {
    REQUIRE(static_cast<int>(1.92) == 1);
    REQUIRE(v["size"].get<double>() == 1.92);
    REQUIRE(v["size"].as<double>() == 1.92);
    REQUIRE(v["size"].as<int>() == 1);
    REQUIRE(v["size"].as<std::string>() == "");
  };

  SECTION("iterator over object") {
    auto it_end = v.end();
    REQUIRE(it_end == v.end());

    auto it1 = v.begin();
    REQUIRE(it1 == v.begin());

    for (int i = 0; i < 3; ++i) {
      ++it1;
      REQUIRE(it1 != v.begin());
      REQUIRE(it1 != v.end());
    }

    ++it1;
    REQUIRE(it1 != v.begin());
    REQUIRE(it1 == v.end());

    std::ostringstream output;
    for (auto& value : v) { output << value.key(); }
    REQUIRE(output.str() == "childrennamesizesubscribed");
  }

  SECTION("iterator over array") {
    auto v2 = v["children"];
    REQUIRE(!v2.empty());

    auto it_end = v2.end();
    bool result = (it_end == v2.end());

    REQUIRE(it_end == v2.end());

    auto it1 = v2.begin();
    REQUIRE(it1 == v2.begin());

    for (int i = 0; i < 1; ++i) {
      ++it1;
      REQUIRE(it1 != v2.begin());
      REQUIRE(it1 != v2.end());
    }

    ++it1;
    REQUIRE(it1 != v2.begin());
    REQUIRE(it1 == v2.end());

    std::ostringstream output;
    for (auto& value : v2) { output << value.get<std::string>(); }
    REQUIRE(output.str() == "MarthaJesabelle");
  }
}
