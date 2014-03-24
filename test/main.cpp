#include <iostream>
#include <nested_container/container.hpp>
#include <nested_container/json.hpp>
#include <nested_container/experimental/cjson.hpp>
#include <vector>
#include <list>
#include <chrono>

using nested_container::container;
template <typename C> using json = nested_container::json::serializer<C>;
template <typename C> using json_cstring = nested_container::experimental::cjson::serializer<C>;

std::string str1() {
  return "Roger";
}

nested_container::container str2() {
  return "Roger";
}


int main(void) {

  // Construction
  {
    container c1;
    std::cout << c1.is_null() << std::endl;
    container c2("Roger");
    container c3(3.f);
    container c4(4);
    container c5(4u);

    //std::string roger = c2;
    std::cout << c2.is_string() << std::endl;
  }

  {
    container c1 = container::init<std::string>();
    std::cout << c1.is_int() << std::endl;
    std::cout << c1.is_string() << std::endl;

    std::string const roger(std::string("Marcel"));
    container c2(roger);
  }

  {
    std::cout << "Begin test 3" << std::endl;
    int roger = 2;
    container c1(roger);

    c1["value1"] = 1;
    c1["value2"] = "123";
    c1["value3"] = 1.f;
    c1["value4"]["s_value1"] = "Hello subsection";
    c1["value4"]["s_value2"] = true;

    int value2 = c1["value2"];
    unsigned int value3 = c1["value3"];
    std::cout << value2 << std::endl;
    std::cout << value3 << std::endl;
    if (c1["value4"]["s_value2"]) {
      std::cout << c1["value4"]["s_value1"].ref_string() << std::endl;
    }

    float& test1 = c1["value2"].transform_float();
    std::cout << test1 << std::endl;
    test1 = 2.f;  // Modifies the underlying container !
    std::cout << c1["value2"].ref_float() << std::endl;
  }
  
  {
    std::cout << "Begin test 4" << std::endl;
    container c1(str1());
  }

  {
    std::cout << "Begin test 5" << std::endl;
    container c2(1);
    std::vector<container> t;
    t.push_back(c2);
    
    std::cout << "Move of container" << std::endl;
    container c5(str2());
    std::cout << c5.is_string() << std::endl;

    std::cout << "Init by vector" << std::endl;
    container c1 = container::init_vec({ "Al1", "Allo", "Deloin", "Roger","Marcel"});
    std::cout << c1[0].is_int() << std::endl;

    //std::cout << "Init by map" << std::endl;
    //container c4 = { 
      //{"Roger",1}
      //, {"Marcel","Robert"}
      //, {"Robert", container::init_vec({1,2,3u})} 
    //};
    //++c4["Roger"].raw_int();
    //std::cout << c4["Roger"].ref_int() << std::endl;
    //std::cout << c4["Roger"].is_int() << std::endl;
    //std::cout << c4["Marcel"].is_string() << std::endl;
    //std::cout << c4["Rober"].is_vector() << std::endl;
  }

  {
    std::cout << "Begin test 6" << std::endl;
    using _ = nested_container::attr_init<container>;
    
    container c2 = {"Marcel", 2, 1.f};
    container c = { 
      _("nom") = "Roger",
      _("prenom") = "Marcel",
      _("attributs") = {
        _("poids") = 95u,
        _("liste") = {1,2.f,"yeah"}
      }
    };

    std::cout << c["nom"].ref_string() << std::endl;
    std::cout << c["attributs"]["liste"][2].ref_string() << std::endl;

    std::cout << "\nSstream json" << std::endl;
    json<container> json_sstream_driver;
    std::cout << json_sstream_driver.serialize(c) << std::endl;

    std::cout << "\nPrintf json" << std::endl;
    json_cstring<container> json_driver;
    std::cout << json_driver.serialize(c) << std::endl;
  }

  if(false)
  {
    std::cout << "Begin test 7" << std::endl;
    container c = container::init_vec({});
    size_t nb_elements = 1e6;
    for(size_t i=0u; i < nb_elements; ++i) {
      c.raw_vector().push_back(static_cast<unsigned int>(i));
    }
    
    json<container> json_sstream_driver;
    json_cstring<container> json_driver;

    size_t constexpr max_iter = 1e0;
    using std::chrono::high_resolution_clock;
    using std::chrono::time_point;
    using std::chrono::duration_cast;
    using std::chrono::milliseconds;

    time_point<high_resolution_clock> start, end;

    // Sstream
    start = high_resolution_clock::now();
    for(size_t i=0u; i < max_iter; ++i) json_sstream_driver.serialize(c);
    end = high_resolution_clock::now();
    std::cout << "Sstream test took " << duration_cast<milliseconds>(end-start).count() << "ms" << std::endl;

    // Fast
    start = high_resolution_clock::now();
    for(size_t i=0u; i < max_iter; ++i) json_driver.serialize(c);
    end = high_resolution_clock::now();
    std::cout << "Printf test took " << duration_cast<milliseconds>(end-start).count() << "ms" << std::endl;
  }


  {
    std::cout << "Begin test 8" << std::endl;
    json<container> json_driver;
    std::string input = R"json(["test1","test2",1.1,2,-4])json";
    input = R"json(
    {"test1":1,
      "test2":1.1,
      "test3":["test1","test2",1.1,2,-4],
      "test4":true,  
         "test5":null
  , "test6":{}}
    )json";

    std::cout << "To deserialize: " << input << std::endl;
    //std::string input = R"json([])json";
    container c = json_driver.deserialize(input);
    std::cout << json_driver.serialize(c) << std::endl;
    container& c2 = c["test3"];
    c = c["test3"];
    
    std::cout << c[1].is_string() << "\n" << c[2].is_float() << "\n" << c[3].is_uint() << "\n" << c[4].is_int() << std::endl;
  }
  return 0;
}

