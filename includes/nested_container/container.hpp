#ifndef NESTED_CONTAINER_CONTAINER
#define NESTED_CONTAINER_CONTAINER

#include "base.hpp"
#include "externalize.hpp"

namespace nested_container {
using container = basic_container<>;
}  // namespace nested_container

#define NESTED_CONTAINER_EXTERNALIZE_CONTAINER(...) NESTED_CONTAINER_EXTERNALIZE(basic_container, std::string, std::string, int, unsigned int, float)
#define NESTED_CONTAINER_INSTANTIATE_CONTAINER(...) NESTED_CONTAINER_INSTANTIATE(basic_container, std::string, std::string, int, unsigned int, float)

#endif  // NESTED_CONTAINER_CONTAINER
