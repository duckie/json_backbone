#include <iostream>
#include <json_backbone.hpp>
#include <map>
#include <sstream>
#include <string>
#include <vector>
#include <cassert>

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
    : public const_func_aggregate_visitor<void, json_container, recursive_printer const&,
                                          std::ostream&> {
  using const_func_aggregate_visitor<void, json_container, recursive_printer const&,
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
                        "grades"_a = make_array<json_container>({1, true, "Ole"}),
                        "null"_a = nullptr});

  // Play with it
  auto s1 = get<std::string>(c["name"]);  // is a string
  c["firstname"] = nullptr;               // Creates a null element
  c["firstname"] = "Marcel";              // This element becomes a string

  auto v = make_view(c);
  for (auto& value : v) {
    std::cout << value.key() << " as int if convertible is " << value.as<int>() << "\n";
  }

  for (auto& value : v["children"]) {
    std::cout << value["name"].as<std::string>() << " is " << value["age"].as<int>() << "\n";
  }

  // Dump to json
  std::ostringstream result_stream;

  // Instantiate statically to avoid multiple creationc cost
  static recursive_printer const printer{
      [](std::nullptr_t const& val, auto&, auto& out) { out << "null"; },
      [](bool val, auto&, auto& out) { out << (val ? "true" : "false"); },
      [](int val, auto&, auto& out) { out << val; },
      [](double val, auto&, auto& out) { out << val; },
      [](auto const& str, auto&, auto& out) { out << '"' << str << '"'; },
      [](auto const& arr, auto& self, auto& out) {
        out << "[";
        bool sep = false;
        for (auto& v : arr) {
          out << (sep ? "," : "");
          apply_visitor<void>(v, self, self, out);
          sep = true;
        }
        out << "]";
      },
      [](auto const& obj, auto& self, auto& out) {
        out << "{";
        bool sep = false;
        for (auto& v : obj) {
          out << (sep ? "," : "") << "\"" << v.first << "\":";
          apply_visitor<void>(v.second, self, self, out);
          sep = true;
        }
        out << "}";
      }};

  // Apply the visitor. "printer" appears two times, once as the actual
  // visitor and once as an extra argument passed to it along with the stream
  apply_visitor<void>(c, printer, printer, result_stream);
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

  std::string& s1 = v4.get<std::string>();  // Checks actual type stored, throws if does not match
  auto s2 = get<std::string>(v4);           // Equivalent to previous call
  auto s3 = v4.raw<std::string>();          // Access the data directly without any check
  auto s4 = raw<std::string>(v4);           // Equivalent to previous call

  std::string& s5 =
      static_cast<std::string&>(v4);  // Makes use of conversion operators, throws if bad type

  bool is_string = v4.is<std::string>();  // Check actual type
}

struct Visitor {
  void operator()(int v) const { std::cout << v << "\n"; }

  void operator()(std::string const& v) const { std::cout << v << "\n"; }
};

void demo4() {
  using variant_t = variant<int, std::string>;
  variant_t t1{1};
  variant_t t2{"Roger"};
  Visitor v;
  apply_visitor<void>(t1, v);
  apply_visitor<void>(t2, v);
}

struct Visitor2 {
  int operator()(int v, int start) const { return v + start; }

  int operator()(std::string const& v, int start) const { return v.size() + start; }
};

void demo5() {
  using variant_t = variant<int, std::string>;
  variant_t t1{1};
  variant_t t2{"Roger"};
  Visitor2 v;
  std::cout << apply_visitor<int>(t1, v, 0) << "\n";
  std::cout << apply_visitor<int>(t2, v, 1) << "\n";
}

struct Visitor3 {
  int sum = 0;
  void operator()(int v) { sum += v; }
};

void demo6() {
  using variant_t = variant<int>;
  variant_t t1{1};
  variant_t t2{2};
  Visitor3 v;
  apply_visitor<void>(t1, v);
  apply_visitor<void>(t2, v);
  std::cout << v.sum << "\n";
}

void demo7() {
  using variant_t = variant<int, std::string>;
  variant_t t1{1};
  variant_t t2{"Roger"};
  static const_funcptr_aggregate_visitor<bool, variant_t> is_string{[](auto) { return false; },
                                                                    [](auto) { return true; }};
  std::cout << apply_visitor<bool>(t1, is_string) << "\n";
  std::cout << apply_visitor<bool>(t2, is_string) << "\n";
}

void doc_demo1() {
  json_container c{json_container::object_type{}};
  c["host"] = "thepizzabay.eat";
  c["port"] = 666;

  auto v = make_view(c);
  auto host = v["host"].get<std::string>();
  auto port = v["port"].get<int>();

  auto whatnot = v["host"]["protocol"]["wut"];
  assert(whatnot.empty());

  for(auto element : v) {
    std::cout << element.key() << " == ";

    if (element.is<int>())
      std::cout << element.get<int>();

    if (element.is<std::string>())
      std::cout << element.get<std::string>();

    std::cout << "\n";
  }
}

int main(void) {
  demo1();
  demo2();
  demo3();
  demo4();
  demo5();
  demo6();
  demo7();
  doc_demo1();
}
