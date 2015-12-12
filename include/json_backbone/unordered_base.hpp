#ifndef JSON_BACKBONE_UNORDERED_BASE_HEADER
#define JSON_BACKBONE_UNORDERED_BASE_HEADER
#include <unordered_map>

namespace json_backbone {
template <typename key_type, typename value_type> using std_unordered_map_default_allocators = std::unordered_map<key_type, value_type>;
}  // namespace json_backbone

#endif  // JSON_BACKBONE_UNORDERED_BASE_HEADER
