#ifndef JSON_BACKBONE_CONTAINER_LL_LDBL_HEADER
#define JSON_BACKBONE_CONTAINER_LL_LDBL_HEADER
#include "base.hpp"
#define JSON_BACKBONE_CONTAINER_LL_LDBL_SIGNATURE(...)                         \
  std::string, std::string, long long int, unsigned long long, long double
namespace json_backbone {
using container_ll_ldbl =
    basic_container<JSON_BACKBONE_CONTAINER_LL_LDBL_SIGNATURE()>;
} // namespace json_backbone
#endif // JSON_BACKBONE_CONTAINER_LL_LDBL_HEADER
