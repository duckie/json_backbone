#include <json_backbone.hpp>
#include <catch.hpp>
#include <chrono>
#include <iostream>
#include <list>
#include <vector>
#include <map>
#include <type_traits>

using namespace json_backbone;

using json_container = container<std::map, std::vector, std::string, std::string, double, int,
                                       unsigned int, bool, std::nullptr_t>;

TEST_CASE("Utils - constexpr computations", "[arithmetics][utils][compile_time]") {
  SECTION("Constexpr max_value") {
    using namespace json_backbone::arithmetics;
    REQUIRE((std::integral_constant<int, max_value<int, 5>({1, 2, 3, 4, 5})>::value) == 5);
    REQUIRE((std::integral_constant<int, max_value<int, 5>({1, 2, 5, 4, 3})>::value) == 5);
    REQUIRE((std::integral_constant<int, max_value<int, 5>({5, 2, 3, 4, 1})>::value) == 5);
    REQUIRE((std::integral_constant<int, max_value<std::size_t, 0>({})>::value) == 0);
  }

  SECTION("Find first true") {
    using namespace json_backbone::arithmetics;
    REQUIRE((std::integral_constant<int, find_first<bool,5>({true, true, true, true, true}, true)>::value) == 0);
    REQUIRE((std::integral_constant<int, find_first<bool,5>({true, false, false, false, false}, true)>::value) == 0);
    REQUIRE((std::integral_constant<int, find_first<bool,5>({false, false, true, false, false}, true)>::value) == 2);
    REQUIRE((std::integral_constant<int, find_first<bool,5>({false, false, false, false, true}, true)>::value) == 4);
    REQUIRE((std::integral_constant<int, find_first<bool,5>({false, false, false, false, false}, true)>::value) == 5);
    REQUIRE((std::integral_constant<int, find_first<bool,0>({}, true, 0)>::value) == 0);
  }

  SECTION("Find last true") {
    using namespace json_backbone::arithmetics;
    REQUIRE((std::integral_constant<int, find_last<bool,5>({true, true, true, true, true}, true)>::value) == 4);
    REQUIRE((std::integral_constant<int, find_last<bool,5>({true, false, false, false, false}, true)>::value) == 0);
    REQUIRE((std::integral_constant<int, find_last<bool,5>({false, false, true, false, false}, true)>::value) == 2);
    REQUIRE((std::integral_constant<int, find_last<bool,5>({false, false, false, false, true}, true)>::value) == 4);
    REQUIRE((std::integral_constant<int, find_last<bool,5>({false, false, false, false, false}, true)>::value) == 5);
    REQUIRE((std::integral_constant<int, find_last<bool,0>({}, true)>::value) == 0);
  }
}
