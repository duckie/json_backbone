#ifndef JSON_BACKBONE_CONTAINER_U_HEADER
#define JSON_BACKBONE_CONTAINER_U_HEADER
#include "base.hpp"
#include "unordered_base.hpp"

#define JSON_BACKBONE_CONTAINER_U_SIGNATURE(...) std::string, std::string, int, unsigned int, float, std_unordered_map_default_allocators 

namespace json_backbone {
using container_u = basic_container<JSON_BACKBONE_CONTAINER_U_SIGNATURE()>;
}  // namespace json_backbone

#endif  // JSON_BACKBONE_CONTAINER_U_HEADER
