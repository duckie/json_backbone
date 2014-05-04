#ifndef NESTED_CONTAINER_CONTAINER_DBL_HEADER
#define NESTED_CONTAINER_CONTAINER_DBL_HEADER
#include "base.hpp"
#define NESTED_CONTAINER_CONTAINER_DBL_SIGNATURE(...) std::string, std::string, int, unsigned int, double
namespace nested_container {
using container_dbl = basic_container<NESTED_CONTAINER_CONTAINER_DBL_SIGNATURE()>;
}  // namespace nested_container
#endif  // NESTED_CONTAINER_CONTAINER_DBL_HEADER

