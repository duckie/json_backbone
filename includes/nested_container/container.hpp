#ifndef NESTED_CONTAINER_CONTAINER_HEADER
#define NESTED_CONTAINER_CONTAINER_HEADER
#include "base.hpp"

#define NESTED_CONTAINER_CONTAINER_SIGNATURE(...) std::string, std::string, int, unsigned int, float

namespace nested_container {
using container = basic_container<NESTED_CONTAINER_CONTAINER_SIGNATURE()>;
}  // namespace nested_container

#endif  // NESTED_CONTAINER_CONTAINER_HEADER
