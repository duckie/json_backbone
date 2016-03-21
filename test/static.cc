#include <iostream>
#include <json_backbone.hpp>
#include <catch.hpp>
#include <chrono>
#include <vector>
#include <map>
#include <type_traits>

using namespace json_backbone;

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

namespace {
struct not_complete;
struct complete {};
}

TEST_CASE("Variant - Static invariants", "[variant][static][compile_time]") {
  // Types
  static_assert((std::is_same<typename json_container::object_type,
                              std::map<std::string, json_container>>::value),
                "Type");
  static_assert(
      (std::is_same<typename json_container::array_type, std::vector<json_container>>::value),
      "Type");

  // Indexes
  static_assert(json_container::target_type_list_t::get_index<std::nullptr_t>() == 0,
                "Inner types indexes");
  static_assert(json_container::target_type_list_t::get_index<bool>() == 1, "Inner types indexes");
  static_assert(json_container::target_type_list_t::get_index<unsigned int>() == 2,
                "Inner types indexes");
  static_assert(json_container::target_type_list_t::get_index<int>() == 3, "Inner types indexes");
  static_assert(json_container::target_type_list_t::get_index<double>() == 4,
                "Inner types indexes");
  static_assert(json_container::target_type_list_t::get_index<std::string>() == 5,
                "Inner types indexes");
  static_assert((json_container::target_type_list_t::get_index<std::vector<json_container>>()) == 6,
                "Inner types indexes");
  static_assert(
      (json_container::target_type_list_t::get_index<std::map<std::string, json_container>>()) == 7,
      "Inner types indexes");
  static_assert(json_container::target_type_list_t::get_index<char>() == 8, "Inner types indexes");

  // Type from constexpr index
  static_assert((is_same_at_t<std::nullptr_t, 0>::value), "Inner types from indexes");
  static_assert((is_same_at_t<bool, 1>::value), "Inner types from indexes");
  static_assert((is_same_at_t<unsigned int, 2>::value), "Inner types from indexes");
  static_assert((is_same_at_t<int, 3>::value), "Inner types from indexes");
  static_assert((is_same_at_t<double, 4>::value), "Inner types from indexes");
  static_assert((is_same_at_t<std::string, 5>::value), "Inner types from indexes");
  static_assert((is_same_at_t<std::vector<json_container>, 6>::value), "Inner types from indexes");
  static_assert((is_same_at_t<std::map<std::string, json_container>, 7>::value),
                "Inner types from indexes");
  static_assert((is_same_at_t<void, 8>::value), "Inner types from indexes");

  // has_type
  static_assert(
      (json_container::target_type_list_t::has_type<std::map<std::string, json_container>>()) ==
          true,
      "Inner types existence");
  static_assert(
      (json_container::target_type_list_t::has_type<std::vector<json_container>>()) == true,
      "Inner types existence");
  static_assert(json_container::target_type_list_t::has_type<std::string>() == true,
                "Inner types existence");
  static_assert(json_container::target_type_list_t::has_type<double>() == true,
                "Inner types existence");
  static_assert(json_container::target_type_list_t::has_type<int>() == true,
                "Inner types existence");
  static_assert(json_container::target_type_list_t::has_type<unsigned int>() == true,
                "Inner types existence");
  static_assert(json_container::target_type_list_t::has_type<bool>() == true,
                "Inner types existence");
  static_assert(json_container::target_type_list_t::has_type<std::nullptr_t>() == true,
                "Inner types existence");
  static_assert(json_container::target_type_list_t::has_type<char>() == false,
                "Inner types existence");

  // Constructible resolution
  static_assert((is_the_constructible_t<double, float>::value), "Is constructible");
  static_assert((is_the_constructible_t<int, short>::value), "Is constructible");
  static_assert((is_the_constructible_t<std::string, char const*>::value), "Is constructible");
  static_assert((is_the_constructible_t<bool, bool>::value), "Is constructible");
  static_assert((is_the_constructible_t<std::nullptr_t, std::nullptr_t>::value),
                "Is constructible");
  static_assert((is_the_constructible_t<std::vector<json_container>,
                                        std::initializer_list<json_container>&&>::value),
                "Is constructible");

  // Completeness
  // static_assert(!is_small_type_impl<recursive_wrapper<not_complete>>::value, "Type should not be
  // complete");
  // static_assert(is_small_type_impl<complete>::value, "Type should be complete");
  static_assert(!is_recursive<int>::value, "Recursive wrapper.");
  static_assert(is_recursive<recursive_wrapper<not_complete>>::value, "Recursive wrapper.");

  REQUIRE(true);
}
