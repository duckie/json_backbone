#include <iostream>
#include <nested_container.hpp>
#include <vector>
#include <list>
#include <chrono>

std::string str1() {
  return "Roger";
}

nested_container::container str2() {
  return "Roger";
}

int main(void) {
  using nested_container::container;
  using nested_container::drivers::json_sstream;
  using nested_container::drivers::json;
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

    std::cout << "Init by map" << std::endl;
    container c4 = { 
      {"Roger",1}
      , {"Marcel","Robert"}
      , {"Robert", container::init_vec({1,2,3u})} 
    };
    ++c4["Roger"].raw_int();
    std::cout << c4["Roger"].ref_int() << std::endl;
    std::cout << c4["Roger"].is_int() << std::endl;
    std::cout << c4["Marcel"].is_string() << std::endl;
    std::cout << c4["Rober"].is_vector() << std::endl;
  }

  {
    std::cout << "Begin test 6" << std::endl;
    container c = {
      {"nom","Roger"}
      , {"prenom","Marcel"}
      , {"attributs", {
          {"poids", 95u}
          , {"taille", 165}
          , {"membres", 5}
          , {"beauf", true}
          , {"copine", nullptr}
          , {"potes", container::init_vec({"René","Jean-Paul","Martine"})}
        }
      }
      , {"attributs_prives", {
          {"poids", 95u}
          , {"taille", 165}
          , {"membres", 5}
          , {"beauf", true}
          , {"copine", nullptr}
          , {"potes", container::init_vec({"René","Jean-Paul","Martine"})}
        }
      }
    };

    std::cout << "\nSstream json" << std::endl;
    json_sstream<container> json_sstream_driver;
    std::cout << json_sstream_driver.serialize(c) << std::endl;

    std::cout << "\nPrintf json" << std::endl;
    json<container> json_driver;
    std::cout << json_driver.serialize(c) << std::endl;

    size_t constexpr max_iter = 1e4;
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
  return 0;
}

