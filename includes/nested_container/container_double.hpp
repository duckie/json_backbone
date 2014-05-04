#ifndef NESTED_CONTAINER_CONTAINER_DOUBLE_HEADER
#define NESTED_CONTAINER_CONTAINER_DOUBLE_HEADER
#include "base.hpp"
#define NESTED_CONTAINER_CONTAINER_DOUBLE_SIGNATURE(...) std::string, std::string, int, unsigned int, double
namespace nested_container {
using container_double = basic_container<NESTED_CONTAINER_CONTAINER_DOUBLE_SIGNATURE()>;
}  // namespace nested_container
#endif  // NESTED_CONTAINER_CONTAINER_DOUBLE_HEADER

