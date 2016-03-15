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
