#ifndef JSON_BACKBONE_CONTAINER_L_DBL_HEADER
#define JSON_BACKBONE_CONTAINER_L_DBL_HEADER
#include "base.hpp"
#define JSON_BACKBONE_CONTAINER_L_DBL_SIGNATURE(...)                           \
  std::string, std::string, long int, unsigned long, double
namespace json_backbone {
using container_l_dbl =
    basic_container<JSON_BACKBONE_CONTAINER_L_DBL_SIGNATURE()>;
} // namespace json_backbone
#endif // JSON_BACKBONE_CONTAINER_L_DBL_HEADER
