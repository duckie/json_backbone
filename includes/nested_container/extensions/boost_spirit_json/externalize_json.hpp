#ifndef JSON_BACKBONE_DRIVER_JSON_EXTERNALIZE_JSON_HEADER
#define JSON_BACKBONE_DRIVER_JSON_EXTERNALIZE_JSON_HEADER

#include "json_forward.hpp"

#define JSON_BACKBONE_EXTERNALIZE_TEMPLATE_JSON(Container, ...) \
namespace json_backbone { \
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
}  /* namespace json_backbone */ \
// -- macro end --

# define JSON_BACKBONE_INSTANTIATE_TEMPLATE_JSON(Container,...) \
namespace json_backbone { \
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
}  /* namespace json_backbone */ \
// -- macro end --

#define JSON_BACKBONE_EXTERNALIZE_JSON(...) JSON_BACKBONE_EXTERNALIZE_TEMPLATE_JSON(basic_container, __VA_ARGS__)
#define JSON_BACKBONE_INSTANTIATE_JSON(...) JSON_BACKBONE_INSTANTIATE_TEMPLATE_JSON(basic_container, __VA_ARGS__)

#endif  // NESTED_COMPILER_DRIVER_JSON_EXTERNALIZE_JSON_HEADER
