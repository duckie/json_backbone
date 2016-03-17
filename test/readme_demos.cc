#include <json_backbone.hpp>
#include <vector>
#include <map>
#include <string>
#include <iostream>
#include <sstream>

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

void demo1() {
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
}

struct BigType {
  char data[128];
};

struct BigType2 {
  BigType data;
};

namespace json_backbone {
template <>
struct is_small_type<BigType> {
  static constexpr bool value = true;
};
}

void demo2() {
  std::cout << sizeof(variant<BigType, BigType2>) << " != " << sizeof(variant<BigType2>)
            << std::endl;
}

void demo3() {
  using variant_t = variant<std::nullptr_t, double, int, std::string>;
  variant_t v1{};          // stored as a std::nullptr_t
  variant_t v2{short(1)};  // stored as int
  variant_t v3{1.f};       // stored as double
  variant_t v4{"Roger"};   // stored as std::string
  // variant_t v5{23u};    // would not compile

  std::cout << v1.is<std::nullptr_t>() << v2.is<int>() << v3.is<double>() << v4.is<std::string>()
            << std::endl;
}

int main(void) {
  demo1();
  demo2();
  demo3();
}
