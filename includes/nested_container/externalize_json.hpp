#ifndef __NESTED_COMPILER_EXTERNALIZE_JSON__
#define __NESTED_COMPILER_EXTERNALIZE_JSON__

#include "json_forward.hpp"

//#ifdef NESTED_CONTAINER_JSON_USE_EXTERNALIZATION_MACROS
#define NESTED_CONTAINER_JSON_EXTERNALIZE(Container, ...) \
namespace nested_container { \
namespace json { \
  extern template class serializer_impl_envelop <Container< __VA_ARGS__ >, typename Container< __VA_ARGS__ >::str_type, generation_policies::visitor_ostream, parsing_policies::full_spirit>; \
  extern template class serializer<Container< __VA_ARGS__ >, typename Container< __VA_ARGS__ >::str_type, generation_policies::visitor_ostream, parsing_policies::full_spirit>; \
  extern template class serializer_impl_envelop <Container< __VA_ARGS__ >, typename Container< __VA_ARGS__ >::str_type, generation_policies::visitor_partial_karma, parsing_policies::full_spirit>; \
  extern template class serializer<Container< __VA_ARGS__ >, typename Container< __VA_ARGS__ >::str_type, generation_policies::visitor_partial_karma, parsing_policies::full_spirit>; \
  extern template class serializer_impl_envelop <Container< __VA_ARGS__ >, typename Container< __VA_ARGS__ >::str_type, generation_policies::full_karma, parsing_policies::full_spirit>; \
  extern template class serializer<Container< __VA_ARGS__ >, typename Container< __VA_ARGS__ >::str_type, generation_policies::full_karma, parsing_policies::full_spirit>; \
  extern template class serializer_impl_envelop <Container< __VA_ARGS__ >, typename Container< __VA_ARGS__ >::str_type, generation_policies::visitor_ostream, parsing_policies::partial_spirit>; \
  extern template class serializer<Container< __VA_ARGS__ >, typename Container< __VA_ARGS__ >::str_type, generation_policies::visitor_ostream, parsing_policies::partial_spirit>; \
  extern template class serializer_impl_envelop <Container< __VA_ARGS__ >, typename Container< __VA_ARGS__ >::str_type, generation_policies::visitor_partial_karma, parsing_policies::partial_spirit>; \
  extern template class serializer<Container< __VA_ARGS__ >, typename Container< __VA_ARGS__ >::str_type, generation_policies::visitor_partial_karma, parsing_policies::partial_spirit>; \
  extern template class serializer_impl_envelop <Container< __VA_ARGS__ >, typename Container< __VA_ARGS__ >::str_type, generation_policies::full_karma, parsing_policies::partial_spirit>; \
  extern template class serializer<Container< __VA_ARGS__ >, typename Container< __VA_ARGS__ >::str_type, generation_policies::full_karma, parsing_policies::partial_spirit>; \
}  /* namespace json */ \
}  /* namespace nested_container */ \
// -- macro end --

# define NESTED_CONTAINER_JSON_INSTANTIATE(Container,...) \
namespace nested_container { \
namespace json { \
  template class serializer_impl_envelop <Container< __VA_ARGS__ >, typename Container< __VA_ARGS__ >::str_type, generation_policies::visitor_ostream, parsing_policies::full_spirit>; \
  template class serializer<Container< __VA_ARGS__ >, typename Container< __VA_ARGS__ >::str_type, generation_policies::visitor_ostream, parsing_policies::full_spirit>; \
  template class serializer_impl_envelop <Container< __VA_ARGS__ >, typename Container< __VA_ARGS__ >::str_type, generation_policies::visitor_partial_karma, parsing_policies::full_spirit>; \
  template class serializer<Container< __VA_ARGS__ >, typename Container< __VA_ARGS__ >::str_type, generation_policies::visitor_partial_karma, parsing_policies::full_spirit>; \
  template class serializer_impl_envelop <Container< __VA_ARGS__ >, typename Container< __VA_ARGS__ >::str_type, generation_policies::full_karma, parsing_policies::full_spirit>; \
  template class serializer<Container< __VA_ARGS__ >, typename Container< __VA_ARGS__ >::str_type, generation_policies::full_karma, parsing_policies::full_spirit>; \
  template class serializer_impl_envelop <Container< __VA_ARGS__ >, typename Container< __VA_ARGS__ >::str_type, generation_policies::visitor_ostream, parsing_policies::partial_spirit>; \
  template class serializer<Container< __VA_ARGS__ >, typename Container< __VA_ARGS__ >::str_type, generation_policies::visitor_ostream, parsing_policies::partial_spirit>; \
  template class serializer_impl_envelop <Container< __VA_ARGS__ >, typename Container< __VA_ARGS__ >::str_type, generation_policies::visitor_partial_karma, parsing_policies::partial_spirit>; \
  template class serializer<Container< __VA_ARGS__ >, typename Container< __VA_ARGS__ >::str_type, generation_policies::visitor_partial_karma, parsing_policies::partial_spirit>; \
  template class serializer_impl_envelop <Container< __VA_ARGS__ >, typename Container< __VA_ARGS__ >::str_type, generation_policies::full_karma, parsing_policies::partial_spirit>; \
  template class serializer<Container< __VA_ARGS__ >, typename Container< __VA_ARGS__ >::str_type, generation_policies::full_karma, parsing_policies::partial_spirit>; \
}  /* namespace json */ \
}  /* namespace nested_container */ \
// -- macro end --

//#else
//# define NESTED_CONTAINER_JSON_EXTERNALIZE(...)
//# define NESTED_CONTAINER_JSON_IMPLEMENT(...)
//#endif

#endif  // __NESTED_COMPILER_EXTERNALIZE__
