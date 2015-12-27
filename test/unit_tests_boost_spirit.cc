#include <gtest/gtest.h>
#include <iostream>
#include <json_backbone/container.hpp>
#include <vector>
#include <list>
#include <chrono>
#include <sstream>
#include <json_backbone/externalize.hpp>
#include <json_backbone/extensions/boost_spirit_json/json_forward.hpp>
#include <json_backbone/extensions/boost_spirit_json/externalize_json.hpp>

JSON_BACKBONE_EXTERNALIZE(JSON_BACKBONE_CONTAINER_SIGNATURE());
JSON_BACKBONE_EXTERNALIZE_JSON(JSON_BACKBONE_CONTAINER_SIGNATURE());

using json_backbone::container;
template <typename C> using json = json_backbone::json::serializer<C>;

class UnitTests : public ::testing::Test {
protected:
  std::string str1() { return "Roger"; }
  json_backbone::container str2() { return "Roger"; }
};

using namespace std;

TEST_F(UnitTests, Construction1) {
  using _ = json_backbone::attr_init<container>;
  container c2 = {"Marcel", 2, 1.f};
  container c = {
      _("nom") = "Roger", _("prenom") = "Marcel",
      _("attributs") = {_("poids") = 95u, _("liste") = {1, 2.f, "yeah"}}};

  json<container> json_sstream_driver;
  ostringstream oss;
  oss << json_sstream_driver.serialize(c);
  std::string expect_result(R"json({"attributs":{"liste":[1,2.0,"yeah"],"poids":95},"nom":"Roger","prenom":"Marcel"})json");
  EXPECT_EQ(expect_result, oss.str());
}
