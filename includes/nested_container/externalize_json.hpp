#ifndef __NESTED_COMPILER_EXTERNALIZE_JSON__
#define __NESTED_COMPILER_EXTERNALIZE_JSON__

#include "json_forward.hpp"

#ifdef NESTED_COMPILER_JSON_USE_EXTERNALIZATION_MACROS
#define NESTED_COMPILER_JSON_EXTERNALIZE(Container, ...) \
namespace nested_container { \
namespace json { \
  extern template class serializer_impl_envelop <Container< __VA_ARGS__ >, typename Container< __VA_ARGS__ >::str_type>; \
  extern template class serializer<Container< __VA_ARGS__ >, typename Container< __VA_ARGS__ >::str_type>; \
}  /* namespace json */ \
}  /* namespace nested_container */ \
// -- macro end --

# define NESTED_COMPILER_JSON_IMPLEMENT(Container,...) \
namespace nested_container { \
namespace json { \
  template class serializer_impl_envelop <Container< __VA_ARGS__ >, typename Container< __VA_ARGS__ >::str_type>; \
  template class serializer<Container< __VA_ARGS__ >, typename Container< __VA_ARGS__ >::str_type>; \
}  /* namespace json */ \
}  /* namespace nested_container */ \
// -- macro end --

#else
# define NESTED_COMPILER_JSON_EXTERNALIZE(...)
# define NESTED_COMPILER_JSON_IMPLEMENT(...)
#endif

#endif  // __NESTED_COMPILER_EXTERNALIZE__
