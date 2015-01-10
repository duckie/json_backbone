#include <gtest/gtest.h>
#include <nested_container/container.hpp>
#include <nested_container/driver/json/json_forward.hpp>
#include <nested_container/externalize.hpp>
//#include <nested_container/driver/json/externalize_json.hpp>

NESTED_CONTAINER_EXTERNALIZE(NESTED_CONTAINER_CONTAINER_SIGNATURE());
//NESTED_CONTAINER_EXTERNALIZE_JSON(NESTED_CONTAINER_CONTAINER_SIGNATURE());

using nested_container::container;
using _ = nested_container::attr_init<container>;

class UnitTests : public ::testing::Test {
 protected:
   container c1 = { 
     _("nom") = "Roger",
     _("prenom") = "Marcel",
     _("attributs") = {
       _("poids") = 95u,
       _("liste") = {1,-3,2.f,"yeah"}
     },
     _("isaman") = true
   };
};

TEST_F(UnitTests, Construction) {
  container c_null;
  EXPECT_EQ(true, c_null.is_null());
  //EXPECT_EQ(true, c_null.is

  container c_map = container::init_map({
      {"Roger",nullptr},
      {"Marcel",nullptr}
      });
  EXPECT_EQ(true, c_map.is_map());
  
  container c_vec {1 ,2 ,nullptr};
  EXPECT_EQ(true, c_vec.is_vector());

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

  EXPECT_EQ(true, c1.is_map());
  EXPECT_EQ(true, c1["nom"].is_string());
  EXPECT_EQ(true, c1["prenom"].is_string());
  EXPECT_EQ(true, c1["attributs"]["liste"].is_vector());
  EXPECT_EQ(true, c1["attributs"]["liste"].is_vector());
  EXPECT_EQ(true, c1["isaman"].is_bool());
  EXPECT_EQ(true, c1["nothing"].is_null());
}
