#ifndef NESTED_CONTAINER_UNORDERED_BASE_HEADER
#define NESTED_CONTAINER_UNORDERED_BASE_HEADER
#include <unordered_map>

namespace nested_container {
template <typename key_type, typename value_type> using std_unordered_map_default_allocators = std::unordered_map<key_type, value_type>;
}  // namespace nested_container

#endif  // NESTED_CONTAINER_UNORDERED_BASE_HEADER
