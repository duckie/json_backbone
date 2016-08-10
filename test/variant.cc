#include <iostream>
#include <json_backbone.hpp>
#include <catch.hpp>
#include <chrono>
#include <vector>
#include <map>
#include <type_traits>

using namespace json_backbone;

namespace {
using json_container = container<std::map, std::vector, std::string, std::nullptr_t, bool,
                                 unsigned int, int, double, std::string>;

template <class Type, std::size_t Index>
using is_same_at_t =
    std::is_same<typename json_container::target_type_list_t::type_at<Index>::type, Type>;

template <class Type, class... Args>
using is_the_constructible_t =
    std::is_same<typename json_container::target_type_list_t::select_constructible<
                     json_container::memory_size, Args...>::type,
                 Type>;
}

// Resolve CATCH SFINAE trait ambiguity (error on Clang, GCC is fine)
namespace Catch {
namespace Detail {
template <>
struct IsStreamInsertable<json_container> {
  enum { value = false };
};
}
}

TEST_CASE("Variant - Construction", "[variant][construct][runtime]") {
  json_container c1;
  json_container c2{std::string("Roger")};
  json_container c3{1.0};
  json_container c4{1};
  json_container c5{true};
  json_container c6{nullptr};

  SECTION("Type") {
    REQUIRE(c1.is<std::nullptr_t>());
    REQUIRE(c2.is<std::string>());
    REQUIRE(c3.is<double>());
    REQUIRE(c4.is<int>());
    REQUIRE(c5.is<bool>());
    REQUIRE(c6.is<std::nullptr_t>());

    REQUIRE(is<std::nullptr_t>(c1));
    REQUIRE(is<std::string>(c2));
    REQUIRE(is<double>(c3));
    REQUIRE(is<int>(c4));
    REQUIRE(is<bool>(c5));
    REQUIRE(is<std::nullptr_t>(c6));
  }

  SECTION("Get") {
    REQUIRE(c1.get<std::nullptr_t>() == nullptr);
    REQUIRE(c2.get<std::string>() == "Roger");
    REQUIRE(c3.get<double>() == 1.0);
    REQUIRE(c4.get<int>() == 1);
    REQUIRE(c5.get<bool>() == true);
    REQUIRE(c6.get<std::nullptr_t>() == nullptr);

    REQUIRE(get<std::nullptr_t>(c1) == nullptr);
    REQUIRE(get<std::string>(c2) == "Roger");
    REQUIRE(get<double>(c3) == 1.0);
    REQUIRE(get<int>(c4) == 1);
    REQUIRE(get<bool>(c5) == true);
    REQUIRE(get<std::nullptr_t>(c6) == nullptr);
  }

  SECTION("Raw") {
    REQUIRE(c1.raw<std::nullptr_t>() == nullptr);
    REQUIRE(c2.raw<std::string>() == "Roger");
    REQUIRE(c3.raw<double>() == 1.0);
    REQUIRE(c4.raw<int>() == 1);
    REQUIRE(c5.raw<bool>() == true);
    REQUIRE(c6.raw<std::nullptr_t>() == nullptr);

    REQUIRE(raw<std::nullptr_t>(c1) == nullptr);
    REQUIRE(raw<std::string>(c2) == "Roger");
    REQUIRE(raw<double>(c3) == 1.0);
    REQUIRE(raw<int>(c4) == 1);
    REQUIRE(raw<bool>(c5) == true);
    REQUIRE(raw<std::nullptr_t>(c6) == nullptr);
  }

  SECTION("Conversion") {
    std::string& s2_1 = static_cast<std::string&>(c2);
    REQUIRE(s2_1 == "Roger");

    std::string const& s2_2 = static_cast<std::string const&>(c2);
    REQUIRE(s2_2 == "Roger");

    std::string s2_3 = std::move(static_cast<std::string>(c2));
    REQUIRE(s2_3 == "Roger");
  }

  SECTION("Assign values") {
    c2.get<std::string>() = "Marcel";
    REQUIRE(c2.get<std::string>() == "Marcel");

    c3.get<double>() = 2.0;
    REQUIRE(c3.get<double>() == 2.0);
  }

  SECTION("Assign variants by copy - same types") {
    json_container c4_2{2};
    c4 = c4_2;

    json_container c2_2{std::string("Marcel")};
    c2 = c2_2;

    REQUIRE(get<std::string>(c2) == "Marcel");
    REQUIRE(get<int>(c4) == 2);
  }

  SECTION("Assign variants by move - same types") {
    c2 = json_container{std::string("Marcel")};
    c4 = json_container{2};

    REQUIRE(get<std::string>(c2) == "Marcel");
    REQUIRE(get<int>(c4) == 2);
  }

  SECTION("Assign variants by copy - different types") {
    c1 = c2;
    c2 = c4;
    REQUIRE(get<std::string>(c1) == "Roger");
    REQUIRE(get<int>(c2) == 1);
  }

  SECTION("Assign variants by move - different types") {
    c1 = json_container{std::string{"Marcel"}};
    c2 = json_container{1};
    REQUIRE(get<std::string>(c1) == "Marcel");
    REQUIRE(get<int>(c2) == 1);
  }

  SECTION("Assign direct values - same types") {
    c2 = std::string("Marcel");
    REQUIRE(c2.get<std::string>() == "Marcel");
    c3 = 2.0;
    REQUIRE(c3.get<double>() == 2.0);
  }

  SECTION("Assign direct values - different types") {
    c3 = std::string("Marcel");
    REQUIRE(c3.get<std::string>() == "Marcel");

    c2 = 2.0;
    REQUIRE(c2.get<double>() == 2.0);
  }

  SECTION("Assign direct values - references - same types") {
    std::string s1{"Marcel"};
    c2 = s1;
    REQUIRE(c2.get<std::string>() == "Marcel");

    double d1 = 2.0;
    c3 = d1;
    REQUIRE(c3.get<double>() == 2.0);
  }

  SECTION("Assign direct values - references - different types") {
    std::string s1{"Marcel"};
    c3 = s1;
    REQUIRE(c3.get<std::string>() == "Marcel");

    double d1 = 2.0;
    c2 = d1;
    REQUIRE(c2.get<double>() == 2.0);
  }

  SECTION("Comparison - equals") {
    REQUIRE(c2 != c3);
    REQUIRE(c2 == c2);

    json_container c2_2{"Roger"};
    REQUIRE(c2 == c2_2);

    json_container c4_2{1};
    REQUIRE(c4 == c4_2);
    c4_2 = 2;
    REQUIRE(c4 != c4_2);

    REQUIRE(c4 == 1);
    REQUIRE(1 == c4);
    REQUIRE(c4 != 1u);  // == 1 as a signed int, not uint
    REQUIRE(c4 != 2);
    REQUIRE(2 != c4);
  }

  SECTION("Comparison - less") {
    REQUIRE(c3 < c2);
    REQUIRE(!(c2 < c3));

    json_container c2_2{"Marcel"};
    REQUIRE(c2_2 < c2);
  }
}

TEST_CASE("Variant - Construction from bounded types ctors other than copy and move",
          "[variant][construct][runtime]") {
  json_container c1{"Roger"};
  REQUIRE(is<std::string>(c1));

  json_container c2{23u, json_container{1}};
  REQUIRE(is<json_container::array_type>(c2));
}

TEST_CASE("Variant - Assignation from types convertible to a bounded types",
          "[variant][construct][runtime]") {
  json_container c1;
  c1 = "Roger";
  REQUIRE(is<std::string>(c1));

  json_container c2;
  c2 = 1.f;
  REQUIRE(is<double>(c2));
}

namespace {
struct add {};
struct sub;
struct mult;
template <typename OpTag>
struct binary_op;

using expression =
    variant<int, recursive_wrapper<binary_op<add>>, recursive_wrapper<binary_op<sub>>>;
// static_assert(completeness_test<expression>::value, "To use with clang for auto detection");

template <typename OpTag>
struct binary_op {
  expression left;  // variant instantiated here...
  expression right;

  binary_op(const expression& lhs, const expression& rhs) : left(lhs), right(rhs) {}
};
}

TEST_CASE("Variant - bad accesses", "[variant][runtime]") {
  json_container c{1};
  REQUIRE_THROWS_AS(c.get<bool>(), bad_variant_access);
  REQUIRE_THROWS_AS(c.get<typename json_container::object_type>(), bad_variant_access);
 
  // Same with const
  json_container const& c2 = c;
  REQUIRE_THROWS_AS(c2.get<bool>(), bad_variant_access);
  REQUIRE_THROWS_AS(c2.get<typename json_container::object_type>(), bad_variant_access);
}

TEST_CASE("Variant - Automatic recursion", "[variant][construct][runtime][recursion]") {
  expression exp1{1};
  expression exp2{2};
  expression exp3{exp1, exp2};  // resolves to binary_op<add>, why does it compiles !!!
  REQUIRE(get<int>(exp1) == 1);
  REQUIRE(get<int>(exp2) == 2);
}

TEST_CASE("Variant - Visit", "[variant][visitor]") {
  variant<int, std::string> t1{1};
  variant<int, std::string> t2{"Roger"};

  static func_aggregate_visitor<bool, variant<int, std::string>> aggregate{
      [](auto) { return false; }, [](auto&) { return true; }};
  static const_func_aggregate_visitor<bool, variant<int, std::string>> caggregate{
      [](auto) { return false; }, [](auto const&) { return true; }};
  static funcptr_aggregate_visitor<bool, variant<int, std::string>> paggregate{
      [](auto) { return false; }, [](auto&) { return true; }};
  static const_funcptr_aggregate_visitor<bool, variant<int, std::string>> pcaggregate{
      [](auto) { return false; }, [](auto&) { return true; }};

  REQUIRE(!apply_visitor<bool>(t1, aggregate));
  REQUIRE(apply_visitor<bool>(t2, aggregate));
  REQUIRE(!apply_visitor<bool>(t1, paggregate));
  REQUIRE(apply_visitor<bool>(t2, pcaggregate));
}
