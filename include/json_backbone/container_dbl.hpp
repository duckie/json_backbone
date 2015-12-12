#ifndef JSON_BACKBONE_CONTAINER_DBL_HEADER
#define JSON_BACKBONE_CONTAINER_DBL_HEADER
#include "base.hpp"
#define JSON_BACKBONE_CONTAINER_DBL_SIGNATURE(...)                             \
  std::string, std::string, int, unsigned int, double
namespace json_backbone {
using container_dbl = basic_container<JSON_BACKBONE_CONTAINER_DBL_SIGNATURE()>;
} // namespace json_backbone
#endif // JSON_BACKBONE_CONTAINER_DBL_HEADER
