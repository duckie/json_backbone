#ifndef NESTED_CONTAINER_CONTAINER_U_HEADER
#define NESTED_CONTAINER_CONTAINER_U_HEADER
#include "base.hpp"
#include "unordered_base.hpp"

#define NESTED_CONTAINER_CONTAINER_U_SIGNATURE(...) std::string, std::string, int, unsigned int, float, std_unordered_map_default_allocators 

namespace nested_container {
using container_u = basic_container<NESTED_CONTAINER_CONTAINER_U_SIGNATURE()>;
}  // namespace nested_container

#endif  // NESTED_CONTAINER_CONTAINER_U_HEADER
