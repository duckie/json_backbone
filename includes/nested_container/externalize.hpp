#ifndef __NESTED_COMPILER_EXTERNALIZE__
#define __NESTED_COMPILER_EXTERNALIZE__

#include <map>
#include <vector>
#include <type_traits>
#include <sstream>
#include <utility>
#include <cassert>
#include <exception>
#include <initializer_list>
#include <tuple>
#include <typeinfo>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cfloat>
#include <limits>

#ifdef NESTED_COMPILER_USE_EXTERNALIZATION_MACROS
#define NESTED_COMPILER_EXTERNALIZE(Container, ...) \
namespace nested_container { \
  extern template class Container<>; \
  /* Traits */ \
  extern template class Container< __VA_ARGS__ >::type_traits<std::nullptr_t>; \
  extern template class Container< __VA_ARGS__ >::type_traits<std::nullptr_t&>; \
  extern template class Container< __VA_ARGS__ >::type_traits<std::nullptr_t const&>; \
  extern template class Container< __VA_ARGS__ >::type_traits<std::nullptr_t&&>; \
  extern template class Container< __VA_ARGS__ >::type_traits<typename Container< __VA_ARGS__ >::map_type>; \
  extern template class Container< __VA_ARGS__ >::type_traits<typename Container< __VA_ARGS__ >::map_type&>; \
  extern template class Container< __VA_ARGS__ >::type_traits<typename Container< __VA_ARGS__ >::map_type const&>; \
  extern template class Container< __VA_ARGS__ >::type_traits<typename Container< __VA_ARGS__ >::map_type &&>; \
  extern template class Container< __VA_ARGS__ >::type_traits<typename Container< __VA_ARGS__ >::vector_type>; \
  extern template class Container< __VA_ARGS__ >::type_traits<typename Container< __VA_ARGS__ >::vector_type&>; \
  extern template class Container< __VA_ARGS__ >::type_traits<typename Container< __VA_ARGS__ >::vector_type const&>; \
  extern template class Container< __VA_ARGS__ >::type_traits<typename Container< __VA_ARGS__ >::vector_type &&>; \
  extern template class Container< __VA_ARGS__ >::type_traits<typename Container< __VA_ARGS__ >::str_type>; \
  extern template class Container< __VA_ARGS__ >::type_traits<typename Container< __VA_ARGS__ >::str_type&>; \
  extern template class Container< __VA_ARGS__ >::type_traits<typename Container< __VA_ARGS__ >::str_type const&>; \
  extern template class Container< __VA_ARGS__ >::type_traits<typename Container< __VA_ARGS__ >::str_type &&>; \
  extern template class Container< __VA_ARGS__ >::type_traits<typename Container< __VA_ARGS__ >::float_type>; \
  extern template class Container< __VA_ARGS__ >::type_traits<typename Container< __VA_ARGS__ >::float_type&>; \
  extern template class Container< __VA_ARGS__ >::type_traits<typename Container< __VA_ARGS__ >::float_type const&>; \
  extern template class Container< __VA_ARGS__ >::type_traits<typename Container< __VA_ARGS__ >::float_type &&>; \
  extern template class Container< __VA_ARGS__ >::type_traits<typename Container< __VA_ARGS__ >::int_type>; \
  extern template class Container< __VA_ARGS__ >::type_traits<typename Container< __VA_ARGS__ >::int_type&>; \
  extern template class Container< __VA_ARGS__ >::type_traits<typename Container< __VA_ARGS__ >::int_type const&>; \
  extern template class Container< __VA_ARGS__ >::type_traits<typename Container< __VA_ARGS__ >::int_type &&>; \
  extern template class Container< __VA_ARGS__ >::type_traits<typename Container< __VA_ARGS__ >::uint_type>; \
  extern template class Container< __VA_ARGS__ >::type_traits<typename Container< __VA_ARGS__ >::uint_type&>; \
  extern template class Container< __VA_ARGS__ >::type_traits<typename Container< __VA_ARGS__ >::uint_type const&>; \
  extern template class Container< __VA_ARGS__ >::type_traits<typename Container< __VA_ARGS__ >::uint_type &&>; \
  extern template class Container< __VA_ARGS__ >::type_traits<bool>; \
  extern template class Container< __VA_ARGS__ >::type_traits<bool&>; \
  extern template class Container< __VA_ARGS__ >::type_traits<bool const&>; \
  extern template class Container< __VA_ARGS__ >::type_traits<bool&&>; \
  /* Ctors */ \
  extern template Container< __VA_ARGS__ >::Container(std::nullptr_t const&); \
  extern template Container< __VA_ARGS__ >::Container(std::nullptr_t&&); \
  extern template Container< __VA_ARGS__ >::Container(std::nullptr_t&); \
  extern template Container< __VA_ARGS__ >::Container(std::nullptr_t const&); \
  extern template Container< __VA_ARGS__ >::Container(std::nullptr_t&&); \
  extern template Container< __VA_ARGS__ >::Container(typename Container< __VA_ARGS__ >::map_type&); \
  extern template Container< __VA_ARGS__ >::Container(typename Container< __VA_ARGS__ >::map_type const&); \
  extern template Container< __VA_ARGS__ >::Container(typename Container< __VA_ARGS__ >::map_type&&); \
  extern template Container< __VA_ARGS__ >::Container(typename Container< __VA_ARGS__ >::vector_type&); \
  extern template Container< __VA_ARGS__ >::Container(typename Container< __VA_ARGS__ >::vector_type const&); \
  extern template Container< __VA_ARGS__ >::Container(typename Container< __VA_ARGS__ >::vector_type&&); \
  extern template Container< __VA_ARGS__ >::Container(typename Container< __VA_ARGS__ >::str_type&); \
  extern template Container< __VA_ARGS__ >::Container(typename Container< __VA_ARGS__ >::str_type const&); \
  extern template Container< __VA_ARGS__ >::Container(typename Container< __VA_ARGS__ >::str_type&&); \
  extern template Container< __VA_ARGS__ >::Container(typename Container< __VA_ARGS__ >::float_type&); \
  extern template Container< __VA_ARGS__ >::Container(typename Container< __VA_ARGS__ >::float_type const&); \
  extern template Container< __VA_ARGS__ >::Container(typename Container< __VA_ARGS__ >::float_type&&); \
  extern template Container< __VA_ARGS__ >::Container(typename Container< __VA_ARGS__ >::int_type&); \
  extern template Container< __VA_ARGS__ >::Container(typename Container< __VA_ARGS__ >::int_type const&); \
  extern template Container< __VA_ARGS__ >::Container(typename Container< __VA_ARGS__ >::int_type&&); \
  extern template Container< __VA_ARGS__ >::Container(typename Container< __VA_ARGS__ >::uint_type&); \
  extern template Container< __VA_ARGS__ >::Container(typename Container< __VA_ARGS__ >::uint_type const&); \
  extern template Container< __VA_ARGS__ >::Container(typename Container< __VA_ARGS__ >::uint_type&&); \
  extern template Container< __VA_ARGS__ >::Container(bool&); \
  extern template Container< __VA_ARGS__ >::Container(bool const&); \
  extern template Container< __VA_ARGS__ >::Container(bool&&); \
  /* Init members */ \
  extern template void Container< __VA_ARGS__ >::init_member<std::nullptr_t>(); \
  extern template void Container< __VA_ARGS__ >::init_member<typename Container< __VA_ARGS__ >::map_type>(); \
  extern template void Container< __VA_ARGS__ >::init_member<typename Container< __VA_ARGS__ >::vector_type>(); \
  extern template void Container< __VA_ARGS__ >::init_member<typename Container< __VA_ARGS__ >::str_type>(); \
  extern template void Container< __VA_ARGS__ >::init_member<typename Container< __VA_ARGS__ >::float_type>(); \
  extern template void Container< __VA_ARGS__ >::init_member<typename Container< __VA_ARGS__ >::int_type>(); \
  extern template void Container< __VA_ARGS__ >::init_member<typename Container< __VA_ARGS__ >::uint_type>(); \
  extern template void Container< __VA_ARGS__ >::init_member<bool>(); \
  /* type switching */ \
  extern template void Container< __VA_ARGS__ >::switch_to_type<std::nullptr_t>(); \
  extern template void Container< __VA_ARGS__ >::switch_to_type<typename Container< __VA_ARGS__ >::map_type>(); \
  extern template void Container< __VA_ARGS__ >::switch_to_type<typename Container< __VA_ARGS__ >::vector_type>(); \
  extern template void Container< __VA_ARGS__ >::switch_to_type<typename Container< __VA_ARGS__ >::str_type>(); \
  extern template void Container< __VA_ARGS__ >::switch_to_type<typename Container< __VA_ARGS__ >::float_type>(); \
  extern template void Container< __VA_ARGS__ >::switch_to_type<typename Container< __VA_ARGS__ >::int_type>(); \
  extern template void Container< __VA_ARGS__ >::switch_to_type<typename Container< __VA_ARGS__ >::uint_type>(); \
  extern template void Container< __VA_ARGS__ >::switch_to_type<bool>(); \
  /* collection accessors */ \
  extern template Container< __VA_ARGS__ > const& Container< __VA_ARGS__ >::operator[](typename Container< __VA_ARGS__ >::key_type const&) const; \
  extern template Container< __VA_ARGS__ > const& Container< __VA_ARGS__ >::operator[](typename Container< __VA_ARGS__ >::key_type const&) const; \
  extern template Container< __VA_ARGS__ >& Container< __VA_ARGS__ >::operator[](typename Container< __VA_ARGS__ >::key_type&); \
  extern template Container< __VA_ARGS__ >& Container< __VA_ARGS__ >::operator[](typename Container< __VA_ARGS__ >::key_type const&); \
  extern template Container< __VA_ARGS__ >& Container< __VA_ARGS__ >::operator[](typename Container< __VA_ARGS__ >::key_type&&); \
  extern template Container< __VA_ARGS__ > const& Container< __VA_ARGS__ >::operator[](typename Container< __VA_ARGS__ >::vec_size_type const&) const; \
  extern template Container< __VA_ARGS__ >& Container< __VA_ARGS__ >::operator[](typename Container< __VA_ARGS__ >::vec_size_type&); \
  extern template Container< __VA_ARGS__ >& Container< __VA_ARGS__ >::operator[](typename Container< __VA_ARGS__ >::vec_size_type const&); \
  extern template Container< __VA_ARGS__ >& Container< __VA_ARGS__ >::operator[](typename Container< __VA_ARGS__ >::vec_size_type&&); \
  extern template Container< __VA_ARGS__ > const& Container< __VA_ARGS__ >::operator[](int const&) const; \
  extern template Container< __VA_ARGS__ >& Container< __VA_ARGS__ >::operator[](int&); \
  extern template Container< __VA_ARGS__ >& Container< __VA_ARGS__ >::operator[](int const&); \
  extern template Container< __VA_ARGS__ >& Container< __VA_ARGS__ >::operator[](int &&); \
}  // namespace nested_Container<>  


# define NESTED_COMPILER_IMPLEMENT(Container,...) \
namespace nested_container { \
  template class Container< __VA_ARGS__ >; \
  /* Traits */ \
  template class Container< __VA_ARGS__ >::type_traits<std::nullptr_t>; \
  template class Container< __VA_ARGS__ >::type_traits<std::nullptr_t&>; \
  template class Container< __VA_ARGS__ >::type_traits<std::nullptr_t const&>; \
  template class Container< __VA_ARGS__ >::type_traits<std::nullptr_t&&>; \
  template class Container< __VA_ARGS__ >::type_traits<typename Container< __VA_ARGS__ >::map_type>; \
  template class Container< __VA_ARGS__ >::type_traits<typename Container< __VA_ARGS__ >::map_type&>; \
  template class Container< __VA_ARGS__ >::type_traits<typename Container< __VA_ARGS__ >::map_type const&>; \
  template class Container< __VA_ARGS__ >::type_traits<typename Container< __VA_ARGS__ >::map_type &&>; \
  template class Container< __VA_ARGS__ >::type_traits<typename Container< __VA_ARGS__ >::vector_type>; \
  template class Container< __VA_ARGS__ >::type_traits<typename Container< __VA_ARGS__ >::vector_type&>; \
  template class Container< __VA_ARGS__ >::type_traits<typename Container< __VA_ARGS__ >::vector_type const&>; \
  template class Container< __VA_ARGS__ >::type_traits<typename Container< __VA_ARGS__ >::vector_type &&>; \
  template class Container< __VA_ARGS__ >::type_traits<typename Container< __VA_ARGS__ >::str_type>; \
  template class Container< __VA_ARGS__ >::type_traits<typename Container< __VA_ARGS__ >::str_type&>; \
  template class Container< __VA_ARGS__ >::type_traits<typename Container< __VA_ARGS__ >::str_type const&>; \
  template class Container< __VA_ARGS__ >::type_traits<typename Container< __VA_ARGS__ >::str_type &&>; \
  template class Container< __VA_ARGS__ >::type_traits<typename Container< __VA_ARGS__ >::float_type>; \
  template class Container< __VA_ARGS__ >::type_traits<typename Container< __VA_ARGS__ >::float_type&>; \
  template class Container< __VA_ARGS__ >::type_traits<typename Container< __VA_ARGS__ >::float_type const&>; \
  template class Container< __VA_ARGS__ >::type_traits<typename Container< __VA_ARGS__ >::float_type &&>; \
  template class Container< __VA_ARGS__ >::type_traits<typename Container< __VA_ARGS__ >::int_type>; \
  template class Container< __VA_ARGS__ >::type_traits<typename Container< __VA_ARGS__ >::int_type&>; \
  template class Container< __VA_ARGS__ >::type_traits<typename Container< __VA_ARGS__ >::int_type const&>; \
  template class Container< __VA_ARGS__ >::type_traits<typename Container< __VA_ARGS__ >::int_type &&>; \
  template class Container< __VA_ARGS__ >::type_traits<typename Container< __VA_ARGS__ >::uint_type>; \
  template class Container< __VA_ARGS__ >::type_traits<typename Container< __VA_ARGS__ >::uint_type&>; \
  template class Container< __VA_ARGS__ >::type_traits<typename Container< __VA_ARGS__ >::uint_type const&>; \
  template class Container< __VA_ARGS__ >::type_traits<typename Container< __VA_ARGS__ >::uint_type &&>; \
  template class Container< __VA_ARGS__ >::type_traits<bool>; \
  template class Container< __VA_ARGS__ >::type_traits<bool&>; \
  template class Container< __VA_ARGS__ >::type_traits<bool const&>; \
  template class Container< __VA_ARGS__ >::type_traits<bool&&>; \
  /* Ctors */ \
  template Container< __VA_ARGS__ >::Container(std::nullptr_t&); \
  template Container< __VA_ARGS__ >::Container(std::nullptr_t const&); \
  template Container< __VA_ARGS__ >::Container(std::nullptr_t&&); \
  template Container< __VA_ARGS__ >::Container(typename Container< __VA_ARGS__ >::map_type&); \
  template Container< __VA_ARGS__ >::Container(typename Container< __VA_ARGS__ >::map_type const&); \
  template Container< __VA_ARGS__ >::Container(typename Container< __VA_ARGS__ >::map_type&&); \
  template Container< __VA_ARGS__ >::Container(typename Container< __VA_ARGS__ >::vector_type&); \
  template Container< __VA_ARGS__ >::Container(typename Container< __VA_ARGS__ >::vector_type const&); \
  template Container< __VA_ARGS__ >::Container(typename Container< __VA_ARGS__ >::vector_type&&); \
  template Container< __VA_ARGS__ >::Container(typename Container< __VA_ARGS__ >::str_type&); \
  template Container< __VA_ARGS__ >::Container(typename Container< __VA_ARGS__ >::str_type const&); \
  template Container< __VA_ARGS__ >::Container(typename Container< __VA_ARGS__ >::str_type&&); \
  template Container< __VA_ARGS__ >::Container(typename Container< __VA_ARGS__ >::float_type&); \
  template Container< __VA_ARGS__ >::Container(typename Container< __VA_ARGS__ >::float_type const&); \
  template Container< __VA_ARGS__ >::Container(typename Container< __VA_ARGS__ >::float_type&&); \
  template Container< __VA_ARGS__ >::Container(typename Container< __VA_ARGS__ >::int_type&); \
  template Container< __VA_ARGS__ >::Container(typename Container< __VA_ARGS__ >::int_type const&); \
  template Container< __VA_ARGS__ >::Container(typename Container< __VA_ARGS__ >::int_type&&); \
  template Container< __VA_ARGS__ >::Container(typename Container< __VA_ARGS__ >::uint_type&); \
  template Container< __VA_ARGS__ >::Container(typename Container< __VA_ARGS__ >::uint_type const&); \
  template Container< __VA_ARGS__ >::Container(typename Container< __VA_ARGS__ >::uint_type&&); \
  template Container< __VA_ARGS__ >::Container(bool&); \
  template Container< __VA_ARGS__ >::Container(bool const&); \
  template Container< __VA_ARGS__ >::Container(bool&&); \
  /* Init members */ \
  template void Container< __VA_ARGS__ >::init_member<std::nullptr_t>(); \
  template void Container< __VA_ARGS__ >::init_member<typename Container< __VA_ARGS__ >::map_type>(); \
  template void Container< __VA_ARGS__ >::init_member<typename Container< __VA_ARGS__ >::vector_type>(); \
  template void Container< __VA_ARGS__ >::init_member<typename Container< __VA_ARGS__ >::str_type>(); \
  template void Container< __VA_ARGS__ >::init_member<typename Container< __VA_ARGS__ >::float_type>(); \
  template void Container< __VA_ARGS__ >::init_member<typename Container< __VA_ARGS__ >::int_type>(); \
  template void Container< __VA_ARGS__ >::init_member<typename Container< __VA_ARGS__ >::uint_type>(); \
  template void Container< __VA_ARGS__ >::init_member<bool>(); \
  /* type switching */ \
  template void Container< __VA_ARGS__ >::switch_to_type<std::nullptr_t>(); \
  template void Container< __VA_ARGS__ >::switch_to_type<typename Container< __VA_ARGS__ >::map_type>(); \
  template void Container< __VA_ARGS__ >::switch_to_type<typename Container< __VA_ARGS__ >::vector_type>(); \
  template void Container< __VA_ARGS__ >::switch_to_type<typename Container< __VA_ARGS__ >::str_type>(); \
  template void Container< __VA_ARGS__ >::switch_to_type<typename Container< __VA_ARGS__ >::float_type>(); \
  template void Container< __VA_ARGS__ >::switch_to_type<typename Container< __VA_ARGS__ >::int_type>(); \
  template void Container< __VA_ARGS__ >::switch_to_type<typename Container< __VA_ARGS__ >::uint_type>(); \
  template void Container< __VA_ARGS__ >::switch_to_type<bool>(); \
  /* collection accessors */ \
  template Container< __VA_ARGS__ > const& Container< __VA_ARGS__ >::operator[](typename Container< __VA_ARGS__ >::key_type const&) const; \
  template Container< __VA_ARGS__ >& Container< __VA_ARGS__ >::operator[](typename Container< __VA_ARGS__ >::key_type&); \
  template Container< __VA_ARGS__ >& Container< __VA_ARGS__ >::operator[](typename Container< __VA_ARGS__ >::key_type const&); \
  template Container< __VA_ARGS__ >& Container< __VA_ARGS__ >::operator[](typename Container< __VA_ARGS__ >::key_type&&); \
  template Container< __VA_ARGS__ > const& Container< __VA_ARGS__ >::operator[](typename Container< __VA_ARGS__ >::vec_size_type const&) const; \
  template Container< __VA_ARGS__ >& Container< __VA_ARGS__ >::operator[](typename Container< __VA_ARGS__ >::vec_size_type&); \
  template Container< __VA_ARGS__ >& Container< __VA_ARGS__ >::operator[](typename Container< __VA_ARGS__ >::vec_size_type const&); \
  template Container< __VA_ARGS__ >& Container< __VA_ARGS__ >::operator[](typename Container< __VA_ARGS__ >::vec_size_type&&); \
  template Container< __VA_ARGS__ > const& Container< __VA_ARGS__ >::operator[](int const&) const; \
  template Container< __VA_ARGS__ >& Container< __VA_ARGS__ >::operator[](int&); \
  template Container< __VA_ARGS__ >& Container< __VA_ARGS__ >::operator[](int const&); \
  template Container< __VA_ARGS__ >& Container< __VA_ARGS__ >::operator[](int &&); \
}  // namespace nested_container  
#else
# define NESTED_COMPILER_EXTERNALIZE(...)
# define NESTED_COMPILER_IMPLEMENT(...)
#endif

#endif  // __NESTED_COMPILER_EXTERNALIZE__
