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

struct loop_separator {
  bool first_passed = false;
  std::string sep;
  loop_separator(std::string s = ",") : sep(s) {}
  operator bool() {
    bool res = first_passed;
    if (!first_passed) first_passed = true;
    return res;
  }
};
std::ostream& operator<<(std::ostream& out, loop_separator& sep) {
  out << (sep ? sep.sep : "");
  return out;
}
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

    REQUIRE(c1.get<std::string>() == "Roger");
    REQUIRE(std::string(c1) == "Roger");
    REQUIRE(double(c2) == 1.92);
    REQUIRE((c3 ? true : false));  // Odd but needed for GCC to compile here
    REQUIRE(c4.get<json_container::array_type>().size());

    auto& c5 = c4[1];
    REQUIRE(c5.is<std::string>());
    REQUIRE(std::string(c5) == "Jesabelle");
  }
}

struct visitor_test_01 {
  std::ostringstream& output;

  visitor_test_01(std::ostringstream& o) : output(o) {}

  void operator()(json_container::object_type const& value) {
    output << "{";
    loop_separator sep;
    for (auto& v : value) {
      output << sep << "\"" << v.first << "\":";
      apply_visitor<void>(v.second, *this);
    }
    output << "}";
  }

  void operator()(json_container::array_type const& value) {
    output << "[";
    loop_separator sep;
    for (auto& v : value) {
      output << sep;
      apply_visitor<void>(v, *this);
    }
    output << "]";
  }

  void operator()(std::string const& value) { output << '"' << value << '"'; }

  void operator()(std::nullptr_t const&) { output << "null"; }

  template <class T>
  void operator()(T const& value) {
    output << value;
  }
};

// CRTP used here so no "using" possible
struct recursive_printer
    : public const_func_aggregate_visitor<void, json_container, recursive_printer const&,
                                          std::ostringstream&> {
  using const_func_aggregate_visitor<void, json_container, recursive_printer const&,
                                     std::ostringstream&>::const_func_aggregate_visitor;
};

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

  SECTION("Apply visitor") {
    std::ostringstream result_stream;
    visitor_test_01 visitor{result_stream};
    apply_visitor<void>(c, visitor);
    REQUIRE(
        result_stream.str() ==
        R"json({"children":[{"age":6,"name":"Martha"},{"age":8,"name":"Jesabelle"}],"grades":[1,1,"Ole"],"name":"Roger","size":1.92,"subscribed":1})json");
  }

  SECTION("Apply aggregate visitor") {
    std::ostringstream result_stream;
    // Instantiate only once
    static recursive_printer const printer{
        [](std::nullptr_t val, auto&, auto& out) { out << "null"; },
        [](bool val, auto&, auto& out) { out << (val ? "true" : "false"); },
        [](int val, auto&, auto& out) { out << val; },
        [](double val, auto&, auto& out) { out << val; },
        [](auto const& str, auto&, auto& out) { out << '"' << str << '"'; },
        [](auto const& arr, auto& self, auto& out) {
          out << "[";
          loop_separator sep;
          for (auto& v : arr) {
            out << sep;
            apply_visitor<void>(v, self, self, out);
          }
          out << "]";
        },
        [](auto const& obj, auto& self, auto& out) {
          out << "{";
          loop_separator sep;
          for (auto& v : obj) {
            out << sep << "\"" << v.first << "\":";
            apply_visitor<void>(v.second, self, self, out);
          }
          out << "}";
        }};

    apply_visitor<void>(c, printer, printer, result_stream);

    REQUIRE(
        result_stream.str() ==
        R"json({"children":[{"age":6,"name":"Martha"},{"age":8,"name":"Jesabelle"}],"grades":[1,true,"Ole"],"name":"Roger","size":1.92,"subscribed":true})json");
  }
}
