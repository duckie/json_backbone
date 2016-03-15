#include <json_backbone/base.hpp>
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

using json_container = container<std::map, std::vector, std::string, std::string, double, int,
                                       unsigned int, bool, std::nullptr_t>;

template <class Type, std::size_t Index>
using is_same_at_t = std::is_same<typename json_container::type_list_type::type_at<Index>::type, Type>;

TEST_CASE("Variant - Static invariants", "[variant][static][compile_time]") {
  SECTION("Type") {
    REQUIRE((std::is_same<typename json_container::object_type,
                          std::map<std::string, json_container>>::value));
    REQUIRE(
        (std::is_same<typename json_container::array_type, std::vector<json_container>>::value));
  }

  SECTION("Inner types indexes") {
    REQUIRE((json_container::type_list_type::get_index<std::map<std::string,json_container>>()) == 0);
    REQUIRE((json_container::type_list_type::get_index<std::vector<json_container>>()) == 1);
    REQUIRE(json_container::type_list_type::get_index<std::string>() == 2);
    REQUIRE(json_container::type_list_type::get_index<double>() == 3);
    REQUIRE(json_container::type_list_type::get_index<int>() == 4);
    REQUIRE(json_container::type_list_type::get_index<unsigned int>() == 5);
    REQUIRE(json_container::type_list_type::get_index<bool>() == 6);
    REQUIRE(json_container::type_list_type::get_index<std::nullptr_t>() == 7);
    REQUIRE(json_container::type_list_type::get_index<char>() == 8);
  }

  SECTION("Inner types from indexes") {
    REQUIRE((is_same_at_t<std::map<std::string,json_container>,0>::value));
    REQUIRE((is_same_at_t<std::vector<json_container>,1>::value));
    REQUIRE((is_same_at_t<std::string,2>::value));
    REQUIRE((is_same_at_t<double,3>::value));
    REQUIRE((is_same_at_t<int,4>::value));
    REQUIRE((is_same_at_t<unsigned int,5>::value));
    REQUIRE((is_same_at_t<bool,6>::value));
    REQUIRE((is_same_at_t<std::nullptr_t,7>::value));
    REQUIRE((is_same_at_t<void,8>::value));
  }

  SECTION("Inner types existence") {
    REQUIRE((json_container::type_list_type::has_type<std::map<std::string,json_container>>()) == true);
    REQUIRE((json_container::type_list_type::has_type<std::vector<json_container>>()) == true);
    REQUIRE(json_container::type_list_type::has_type<std::string>() == true);
    REQUIRE(json_container::type_list_type::has_type<double>() == true);
    REQUIRE(json_container::type_list_type::has_type<int>() == true);
    REQUIRE(json_container::type_list_type::has_type<unsigned int>() == true);
    REQUIRE(json_container::type_list_type::has_type<bool>() == true);
    REQUIRE(json_container::type_list_type::has_type<std::nullptr_t>() == true);
    REQUIRE(json_container::type_list_type::has_type<char>() == false);
  }
}

TEST_CASE("Variant - Construction", "[variant][construct][runtime]") {
  json_container c1;
  json_container c2 { std::string("Roger") };
  json_container c3 { 1.0 };
  json_container c4 { 1 };
  json_container c5 { true };
  json_container c6 { nullptr };

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

  SECTION("Assign values") {
    c2.get<std::string>() = "Marcel";
    REQUIRE(c2.get<std::string>() == "Marcel");
    
    c3.get<double>() = 2.0;
    REQUIRE(c3.get<double>() == 2.0);
  }

  SECTION("Assign variants by copy - same types") {
    json_container c4_2 {2};
    c4 = c4_2;
    
    json_container c2_2 {std::string("Marcel")};
    c2 = c2_2;

    REQUIRE(get<std::string>(c2) == "Marcel");
    REQUIRE(get<int>(c4) == 2);
  }

  SECTION("Assign variants by move - same types") {
    c2 = json_container {std::string("Marcel")};
    c4 = json_container {2};

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
    c1 = json_container {std::string{"Marcel"}};
    c2 = json_container {1};
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
    std::string s1 { "Marcel" };
    c2 = s1;
    REQUIRE(c2.get<std::string>() == "Marcel");

    double d1 = 2.0;
    c3 = d1;
    REQUIRE(c3.get<double>() == 2.0);
  }

  SECTION("Assign direct values - references - different types") {
    std::string s1 { "Marcel" };
    c3 = s1;
    REQUIRE(c3.get<std::string>() == "Marcel");

    double d1 = 2.0;
    c2 = d1;
    REQUIRE(c2.get<double>() == 2.0);
  }
}
