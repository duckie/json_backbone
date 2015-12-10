#include <gtest/gtest.h>
#include <nested_container/container.hpp>
#include <nested_container/externalize.hpp>
#include <nested_container/extensions/rapidjson/rapidjson.hpp>

NESTED_CONTAINER_EXTERNALIZE(NESTED_CONTAINER_CONTAINER_SIGNATURE());

using nested_container::container;

nested_container::attr_init<container> operator ""_m (char const* value, size_t length) { return nested_container::attr_init<container>(value); }

class UnitTests : public ::testing::Test {
 protected:
   container c1 = { 
     "nom"_m = "Roger",
     "prenom"_m = "Marcel",
     "attributs"_m = {
       "poids"_m = 95u,
       "liste"_m = {1,-3,2.f,"yeah"}
     },
     "isaman"_m = true
   };
};

TEST_F(UnitTests, RapidJson1) {
  using nested_container::extensions::rapidjson::make_reader_handler;

  std::string input = R"json({"age":57,"name":"Marcelo","size":1.8})json";
  container output;  
  auto handler = make_reader_handler(output);

  ::rapidjson::Reader reader;
  ::rapidjson::StringStream ss(input.c_str());
  EXPECT_TRUE(reader.Parse(ss, handler));
  EXPECT_EQ("Marcelo", output["name"].ref_string());
  EXPECT_EQ(57, output["age"].ref_uint());
}
