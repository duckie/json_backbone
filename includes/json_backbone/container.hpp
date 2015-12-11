#ifndef JSON_BACKBONE_CONTAINER_HEADER
#define JSON_BACKBONE_CONTAINER_HEADER
#include "base.hpp"

#define JSON_BACKBONE_CONTAINER_SIGNATURE(...) std::string, std::string, int, unsigned int, float

namespace json_backbone {
using container = basic_container<JSON_BACKBONE_CONTAINER_SIGNATURE()>;
}  // namespace json_backbone

#endif  // JSON_BACKBONE_CONTAINER_HEADER
