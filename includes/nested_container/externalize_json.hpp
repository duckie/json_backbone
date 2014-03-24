#ifndef __NESTED_COMPILER_EXTERNALIZE_JSON__
#define __NESTED_COMPILER_EXTERNALIZE_JSON__

#include "json_forward.hpp"

#ifdef NESTED_COMPILER_JSON_USE_EXTERNALIZATION_MACROS
#define NESTED_COMPILER_JSON_EXTERNALIZE(Container, ...) \
namespace boost { \
namespace spirit { \
namespace qi { \
  extern template class rule<typename nested_container::Container< __VA_ARGS__ >::str_type::const_iterator, std::nullptr_t(), boost::spirit::ascii::space_type>; \
  extern template class rule<typename nested_container::Container< __VA_ARGS__ >::str_type::const_iterator, bool(), boost::spirit::ascii::space_type>; \
  extern template class rule<typename nested_container::Container< __VA_ARGS__ >::str_type::const_iterator, nested_container::Container< __VA_ARGS__ >(), boost::spirit::ascii::space_type>; \
  extern template class rule<typename nested_container::Container< __VA_ARGS__ >::str_type::const_iterator, typename nested_container::Container< __VA_ARGS__ >::map_type (), boost::spirit::ascii::space_type>; \
  extern template class rule<typename nested_container::Container< __VA_ARGS__ >::str_type::const_iterator, std::pair<typename nested_container::Container< __VA_ARGS__ >::str_type, nested_container::Container< __VA_ARGS__ >> (), boost::spirit::ascii::space_type>; \
  extern template class rule<typename nested_container::Container< __VA_ARGS__ >::str_type::const_iterator, typename nested_container::Container< __VA_ARGS__ >::vector_type (), boost::spirit::ascii::space_type>; \
  extern template class rule<typename nested_container::Container< __VA_ARGS__ >::str_type::const_iterator, typename nested_container::Container< __VA_ARGS__ >::key_type (), boost::spirit::ascii::space_type>; \
  extern template class rule<typename nested_container::Container< __VA_ARGS__ >::str_type::const_iterator, typename nested_container::Container< __VA_ARGS__ >::str_type (), boost::spirit::ascii::space_type>; \
  extern template class rule<typename nested_container::Container< __VA_ARGS__ >::str_type::const_iterator, typename nested_container::Container< __VA_ARGS__ >::float_type (), boost::spirit::ascii::space_type>; \
  extern template class rule<typename nested_container::Container< __VA_ARGS__ >::str_type::const_iterator, typename nested_container::Container< __VA_ARGS__ >::int_type (), boost::spirit::ascii::space_type>; \
  extern template class rule<typename nested_container::Container< __VA_ARGS__ >::str_type::const_iterator, typename nested_container::Container< __VA_ARGS__ >::uint_type (), boost::spirit::ascii::space_type>; \
}}} \
\
namespace nested_container { \
namespace json { \
  extern template class serializer<Container< __VA_ARGS__ > >; \
  extern template struct raw_grammar< Container< __VA_ARGS__ >, typename Container< __VA_ARGS__ >::str_type::const_iterator >; \
}  /* namespace json */ \
}  // namespace nested_container


# define NESTED_COMPILER_JSON_IMPLEMENT(Container,...) \
namespace nested_container { \
namespace json { \
  template class serializer<Container< __VA_ARGS__ > >; \
  template struct raw_grammar< Container< __VA_ARGS__ >, typename Container< __VA_ARGS__ >::str_type::const_iterator >; \
}  /* namespace json */ \
}  // namespace nested_container  
#else
# define NESTED_COMPILER_JSON_EXTERNALIZE(...)
# define NESTED_COMPILER_JSON_IMPLEMENT(...)
#endif

#endif  // __NESTED_COMPILER_EXTERNALIZE__
