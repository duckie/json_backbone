#ifndef NESTED_CONTAINER_CONTAINER_LL_LDBL_HEADER
#define NESTED_CONTAINER_CONTAINER_LL_LDBL_HEADER
#include "base.hpp"
#define NESTED_CONTAINER_CONTAINER_LL_LDBL_SIGNATURE(...) std::string, std::string, long long int, unsigned long long, long double
namespace nested_container {
using container_ll_ldbl = basic_container<NESTED_CONTAINER_CONTAINER_LL_LDBL_SIGNATURE()>;
}  // namespace nested_container
#endif  // NESTED_CONTAINER_CONTAINER_LL_LDBL_HEADER

