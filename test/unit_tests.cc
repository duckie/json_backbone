#include <catch.hpp>
#include <chrono>
#include <iostream>
#include <json_backbone/container.hpp>
#include <list>
#include <vector>
//#include <json_backbone/externalize.hpp>

using json_backbone::container;

TEST_CASE("Json container - Construction","[json]") {
    REQUIRE(true);
}

/*
class UnitTests : public ::testing::Test {
protected:
  //std::string str1() { return "Roger"; }
  //json_backbone::container str2() { return "Roger"; }
  // container c1 = {
  //_("nom") = "Roger", _("prenom") = "Marcel",
  //_("attributs") = {_("poids") = 95u, _("liste") = {1, -3, 2.f, "yeah"}},
  //_("isaman") = true};
};


TEST_F(UnitTests, Construction1) {
  EXPECT_TRUE(c1.is_null());

  container c2("Roger");
  EXPECT_TRUE(c2.is_string());

  container c3(3.f);
  EXPECT_TRUE(c3.is_float());

  container c4(4);
  EXPECT_TRUE(c4.is_int());

  container c5(4u);
  EXPECT_TRUE(c5.is_uint());
}

TEST_F(UnitTests, Construction2) {
  container c1 = container::init<std::string>();
  EXPECT_FALSE(c1.is_int());
  EXPECT_TRUE(c1.is_string());

  std::string const roger(std::string("Marcel"));
  container c2(roger);
}

TEST_F(UnitTests, Construction3) {
  int roger = 2;
  container c1(roger);

  c1["value1"] = 1;
  c1["value2"] = "123";
  c1["value3"] = 1.f;
  c1["value4"]["s_value1"] = "Hello subsection";
  c1["value4"]["s_value2"] = true;

  int value2 = c1["value2"];
  EXPECT_EQ(123, value2);
  unsigned int value3 = c1["value3"];
  EXPECT_EQ(1u, value3);
  EXPECT_TRUE(static_cast<bool>(c1["value4"]["s_value2"]));
  EXPECT_EQ("Hello subsection", c1["value4"]["s_value1"].ref_string());

  float &test1 = c1["value2"].transform_float();
  EXPECT_EQ(123.f, test1);
  test1 = 2.f; // Modifies the underlying container !
  EXPECT_EQ(2.f, c1["value2"].ref_float());
}

TEST_F(UnitTests, Construction4) { container c1(str1()); }

TEST_F(UnitTests, Construction5) {
  container c2(1);
  std::vector<container> t;
  t.push_back(c2);

  container c5(str2());
  EXPECT_TRUE(c5.is_string());

  container c1 =
      container::init_vec({"Al1", "Allo", "Deloin", "Roger", "Marcel"});
  EXPECT_FALSE(c1[0u].is_int());

  container c4 =
      container::init_object({{"Roger", 1},
                           {"Marcel", "Robert"},
                           {"Marcel2", 2u},
                           {"Marcel2", 2.f},
                           {"Marcel2", container::init_object({{"Robert", 1}})},
                           {"Robert", container::init_vec({1, 2, 3u})}});
  ++c4["Roger"].raw_int();
  EXPECT_EQ(2, c4["Roger"].ref_int());
  EXPECT_TRUE(c4["Roger"].is_int());
  EXPECT_TRUE(c4["Marcel"].is_string());
  EXPECT_FALSE(c4["Rober"].is_array());
}

namespace {
inline json_backbone::attr_init<container> operator""_m(char const *data,
                                                        size_t length) {
  return json_backbone::attr_init<container>(data);
}
}

TEST_F(UnitTests, Construction6) {

  container c2 = {"Marcel", 2, 1.f};
  
  container c = {
      "nom"_m = "Roger", "prenom"_m = "Marcel",
      "attributs"_m = {"poids"_m = 95u, "liste"_m = {1, 2.f, "yeah"}}};

  c["attributs"]["sub_attributes"][0] = true;
  c["attributs"]["sub_attributes"][3] = "falsy man";

  c["attributs"]["sub_attributes2"].transform_array().emplace_back(nullptr);
  c["attributs"]["sub_attributes2"].ref_array().emplace_back(-1);

  EXPECT_EQ(std::string("Roger"),c["nom"].ref_string());
  std::cout << c["attributs"]["liste"][2].ref_string() << std::endl;

  std::string yeah = c["attributs"]["liste"][0];
  std::cout << "String from number " << yeah << std::endl;

  std::string test1("Yeah ma poule");
  container c3(test1.c_str());
  EXPECT_EQ(test1, c3.ref_string());
}
*/
