#include <gtest/gtest.h>
#include <json_backbone/container.hpp>
#include <json_backbone/extensions/boost_spirit_json/json_forward.hpp>
#include <json_backbone/externalize.hpp>
//#include <json_backbone/extensions/boost_spirit_json/externalize_json.hpp>

JSON_BACKBONE_EXTERNALIZE(JSON_BACKBONE_CONTAINER_SIGNATURE());
// JSON_BACKBONE_EXTERNALIZE_JSON(JSON_BACKBONE_CONTAINER_SIGNATURE());

using json_backbone::container;
using _ = json_backbone::attr_init<container>;

class UnitTests : public ::testing::Test {
 protected:
  container c1 = {
      _("nom") = "Roger", _("prenom") = "Marcel",
      _("attributs") = {_("poids") = 95u, _("liste") = {1, -3, 2.f, "yeah"}},
      _("isaman") = true};
};

TEST_F(UnitTests, Construction) {
  container c_null;
  EXPECT_EQ(true, c_null.is_null());
  // EXPECT_EQ(true, c_null.is

  container c_object =
      container::init_object({{"Roger", nullptr}, {"Marcel", nullptr}});
  EXPECT_EQ(true, c_object.is_object());

  container c_vec{1, 2, nullptr};
  EXPECT_EQ(true, c_vec.is_array());

  container c_str(std::string("Roger"));
  EXPECT_EQ(true, c_str.is_string());

  container c_int(1);
  EXPECT_EQ(true, c_int.is_int());

  container c_uint(1u);
  EXPECT_EQ(true, c_uint.is_uint());

  container c_fpn(1.f);
  EXPECT_EQ(true, c_fpn.is_float());

  container c_bool(true);
  EXPECT_EQ(true, c_bool.is_bool());

  EXPECT_EQ(true, c1.is_object());
  EXPECT_EQ(true, c1["nom"].is_string());
  EXPECT_EQ(true, c1["prenom"].is_string());
  EXPECT_EQ(true, c1["attributs"]["liste"].is_array());
  EXPECT_EQ(true, c1["attributs"]["liste"].is_array());
  EXPECT_EQ(true, c1["isaman"].is_bool());
  EXPECT_EQ(true, c1["nothing"].is_null());
}
