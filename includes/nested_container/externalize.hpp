#ifndef HEADER_GUARD_NESTED_CONTAINER_EXTERNALIZE
#define HEADER_GUARD_NESTED_CONTAINER_EXTERNALIZE

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

//#ifdef NESTED_CONTAINER_USE_EXTERNALIZATION_MACROS
#define NESTED_CONTAINER_EXTERNALIZE(Container, ...) \
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
  extern template Container< __VA_ARGS__ >::Container(typename Container< __VA_ARGS__ >::str_type::value_type const*); \
  extern template Container< __VA_ARGS__ >::Container(typename Container< __VA_ARGS__ >::str_type::value_type*); \
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
  /* Array Ctors up to 12 */ \
  extern template Container< __VA_ARGS__ >::Container(typename Container< __VA_ARGS__ >::str_type::value_type const (&) [1]); \
  extern template Container< __VA_ARGS__ >::Container(typename Container< __VA_ARGS__ >::str_type::value_type const (&) [2]); \
  extern template Container< __VA_ARGS__ >::Container(typename Container< __VA_ARGS__ >::str_type::value_type const (&) [3]); \
  extern template Container< __VA_ARGS__ >::Container(typename Container< __VA_ARGS__ >::str_type::value_type const (&) [4]); \
  extern template Container< __VA_ARGS__ >::Container(typename Container< __VA_ARGS__ >::str_type::value_type const (&) [5]); \
  extern template Container< __VA_ARGS__ >::Container(typename Container< __VA_ARGS__ >::str_type::value_type const (&) [6]); \
  extern template Container< __VA_ARGS__ >::Container(typename Container< __VA_ARGS__ >::str_type::value_type const (&) [7]); \
  extern template Container< __VA_ARGS__ >::Container(typename Container< __VA_ARGS__ >::str_type::value_type const (&) [8]); \
  extern template Container< __VA_ARGS__ >::Container(typename Container< __VA_ARGS__ >::str_type::value_type const (&) [9]); \
  extern template Container< __VA_ARGS__ >::Container(typename Container< __VA_ARGS__ >::str_type::value_type const (&) [10]); \
  extern template Container< __VA_ARGS__ >::Container(typename Container< __VA_ARGS__ >::str_type::value_type const (&) [11]); \
  extern template Container< __VA_ARGS__ >::Container(typename Container< __VA_ARGS__ >::str_type::value_type const (&) [12]); \
  /* Assignment from other types */ \
  extern template Container< __VA_ARGS__ >& Container< __VA_ARGS__ >::operator=(typename Container< __VA_ARGS__ >::map_type&); \
  extern template Container< __VA_ARGS__ >& Container< __VA_ARGS__ >::operator=(typename Container< __VA_ARGS__ >::map_type const&); \
  extern template Container< __VA_ARGS__ >& Container< __VA_ARGS__ >::operator=(typename Container< __VA_ARGS__ >::map_type&&); \
  extern template Container< __VA_ARGS__ >& Container< __VA_ARGS__ >::operator=(typename Container< __VA_ARGS__ >::vector_type&); \
  extern template Container< __VA_ARGS__ >& Container< __VA_ARGS__ >::operator=(typename Container< __VA_ARGS__ >::vector_type const&); \
  extern template Container< __VA_ARGS__ >& Container< __VA_ARGS__ >::operator=(typename Container< __VA_ARGS__ >::vector_type&&); \
  extern template Container< __VA_ARGS__ >& Container< __VA_ARGS__ >::operator=(typename Container< __VA_ARGS__ >::str_type&); \
  extern template Container< __VA_ARGS__ >& Container< __VA_ARGS__ >::operator=(typename Container< __VA_ARGS__ >::str_type const&); \
  extern template Container< __VA_ARGS__ >& Container< __VA_ARGS__ >::operator=(typename Container< __VA_ARGS__ >::str_type&&); \
  extern template Container< __VA_ARGS__ >& Container< __VA_ARGS__ >::operator=(typename Container< __VA_ARGS__ >::float_type&); \
  extern template Container< __VA_ARGS__ >& Container< __VA_ARGS__ >::operator=(typename Container< __VA_ARGS__ >::float_type const&); \
  extern template Container< __VA_ARGS__ >& Container< __VA_ARGS__ >::operator=(typename Container< __VA_ARGS__ >::float_type&&); \
  extern template Container< __VA_ARGS__ >& Container< __VA_ARGS__ >::operator=(typename Container< __VA_ARGS__ >::int_type&); \
  extern template Container< __VA_ARGS__ >& Container< __VA_ARGS__ >::operator=(typename Container< __VA_ARGS__ >::int_type const&); \
  extern template Container< __VA_ARGS__ >& Container< __VA_ARGS__ >::operator=(typename Container< __VA_ARGS__ >::int_type&&); \
  extern template Container< __VA_ARGS__ >& Container< __VA_ARGS__ >::operator=(typename Container< __VA_ARGS__ >::uint_type&); \
  extern template Container< __VA_ARGS__ >& Container< __VA_ARGS__ >::operator=(typename Container< __VA_ARGS__ >::uint_type const&); \
  extern template Container< __VA_ARGS__ >& Container< __VA_ARGS__ >::operator=(typename Container< __VA_ARGS__ >::uint_type&&); \
  extern template Container< __VA_ARGS__ >& Container< __VA_ARGS__ >::operator=(bool&); \
  extern template Container< __VA_ARGS__ >& Container< __VA_ARGS__ >::operator=(bool const&); \
  extern template Container< __VA_ARGS__ >& Container< __VA_ARGS__ >::operator=(bool&&); \
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
  extern template Container< __VA_ARGS__ > const& Container< __VA_ARGS__ >::operator[](typename Container< __VA_ARGS__ >::key_type::value_type const*) const; \
  extern template Container< __VA_ARGS__ > const& Container< __VA_ARGS__ >::operator[](typename Container< __VA_ARGS__ >::key_type const&) const; \
  extern template Container< __VA_ARGS__ >& Container< __VA_ARGS__ >::operator[](typename Container< __VA_ARGS__ >::key_type::value_type const*); \
  extern template Container< __VA_ARGS__ >& Container< __VA_ARGS__ >::operator[](typename Container< __VA_ARGS__ >::key_type&);\
  extern template Container< __VA_ARGS__ >& Container< __VA_ARGS__ >::operator[](typename Container< __VA_ARGS__ >::key_type const&); \
  extern template Container< __VA_ARGS__ >& Container< __VA_ARGS__ >::operator[](typename Container< __VA_ARGS__ >::key_type&&); \
  extern template Container< __VA_ARGS__ > const& Container< __VA_ARGS__ >::operator[](typename Container< __VA_ARGS__ >::vec_size_type const&) const; \
  extern template Container< __VA_ARGS__ >& Container< __VA_ARGS__ >::operator[](typename Container< __VA_ARGS__ >::vec_size_type&);\
  extern template Container< __VA_ARGS__ >& Container< __VA_ARGS__ >::operator[](typename Container< __VA_ARGS__ >::vec_size_type const&);\
  extern template Container< __VA_ARGS__ >& Container< __VA_ARGS__ >::operator[](typename Container< __VA_ARGS__ >::vec_size_type&&);\
  extern template Container< __VA_ARGS__ > const& Container< __VA_ARGS__ >::operator[](int const&) const; \
  extern template Container< __VA_ARGS__ >& Container< __VA_ARGS__ >::operator[](int&);\
  extern template Container< __VA_ARGS__ >& Container< __VA_ARGS__ >::operator[](int const&); \
  extern template Container< __VA_ARGS__ >& Container< __VA_ARGS__ >::operator[](int &&);\
  /* Accessors */ \
  extern template typename Container< __VA_ARGS__ >::map_type*    Container< __VA_ARGS__ >::ptr_to<typename Container< __VA_ARGS__ >::map_type, 0>(); \
  extern template typename Container< __VA_ARGS__ >::vector_type* Container< __VA_ARGS__ >::ptr_to<typename Container< __VA_ARGS__ >::vector_type, 0>(); \
  extern template typename Container< __VA_ARGS__ >::str_type*    Container< __VA_ARGS__ >::ptr_to<typename Container< __VA_ARGS__ >::str_type, 0>(); \
  extern template typename Container< __VA_ARGS__ >::float_type*  Container< __VA_ARGS__ >::ptr_to<typename Container< __VA_ARGS__ >::float_type, 0>(); \
  extern template typename Container< __VA_ARGS__ >::int_type*    Container< __VA_ARGS__ >::ptr_to<typename Container< __VA_ARGS__ >::int_type, 0>(); \
  extern template typename Container< __VA_ARGS__ >::uint_type*   Container< __VA_ARGS__ >::ptr_to<typename Container< __VA_ARGS__ >::uint_type, 0>(); \
  extern template bool*                                           Container< __VA_ARGS__ >::ptr_to<bool, 0>(); \
  extern template typename Container< __VA_ARGS__ >::map_type     const* Container< __VA_ARGS__ >::ptr_to<typename Container< __VA_ARGS__ >::map_type, 0>() const; \
  extern template typename Container< __VA_ARGS__ >::vector_type  const* Container< __VA_ARGS__ >::ptr_to<typename Container< __VA_ARGS__ >::vector_type, 0>() const; \
  extern template typename Container< __VA_ARGS__ >::str_type     const* Container< __VA_ARGS__ >::ptr_to<typename Container< __VA_ARGS__ >::str_type, 0>() const; \
  extern template typename Container< __VA_ARGS__ >::float_type   const* Container< __VA_ARGS__ >::ptr_to<typename Container< __VA_ARGS__ >::float_type, 0>() const; \
  extern template typename Container< __VA_ARGS__ >::int_type     const* Container< __VA_ARGS__ >::ptr_to<typename Container< __VA_ARGS__ >::int_type, 0>() const; \
  extern template typename Container< __VA_ARGS__ >::uint_type    const* Container< __VA_ARGS__ >::ptr_to<typename Container< __VA_ARGS__ >::uint_type, 0>() const; \
  extern template bool                                            const* Container< __VA_ARGS__ >::ptr_to<bool, 0>() const; \
  extern template typename Container< __VA_ARGS__ >::map_type*    Container< __VA_ARGS__ >::get<typename Container< __VA_ARGS__ >::map_type>() noexcept; \
  extern template typename Container< __VA_ARGS__ >::vector_type* Container< __VA_ARGS__ >::get<typename Container< __VA_ARGS__ >::vector_type>() noexcept; \
  extern template typename Container< __VA_ARGS__ >::str_type*    Container< __VA_ARGS__ >::get<typename Container< __VA_ARGS__ >::str_type>() noexcept; \
  extern template typename Container< __VA_ARGS__ >::float_type*  Container< __VA_ARGS__ >::get<typename Container< __VA_ARGS__ >::float_type>() noexcept; \
  extern template typename Container< __VA_ARGS__ >::int_type*    Container< __VA_ARGS__ >::get<typename Container< __VA_ARGS__ >::int_type>() noexcept; \
  extern template typename Container< __VA_ARGS__ >::uint_type*   Container< __VA_ARGS__ >::get<typename Container< __VA_ARGS__ >::uint_type>() noexcept; \
  extern template bool*                                           Container< __VA_ARGS__ >::get<bool>() noexcept; \
  extern template typename Container< __VA_ARGS__ >::map_type     const* Container< __VA_ARGS__ >::get<typename Container< __VA_ARGS__ >::map_type>() const noexcept; \
  extern template typename Container< __VA_ARGS__ >::vector_type  const* Container< __VA_ARGS__ >::get<typename Container< __VA_ARGS__ >::vector_type>() const noexcept; \
  extern template typename Container< __VA_ARGS__ >::str_type     const* Container< __VA_ARGS__ >::get<typename Container< __VA_ARGS__ >::str_type>() const noexcept; \
  extern template typename Container< __VA_ARGS__ >::float_type   const* Container< __VA_ARGS__ >::get<typename Container< __VA_ARGS__ >::float_type>() const noexcept; \
  extern template typename Container< __VA_ARGS__ >::int_type     const* Container< __VA_ARGS__ >::get<typename Container< __VA_ARGS__ >::int_type>() const noexcept; \
  extern template typename Container< __VA_ARGS__ >::uint_type    const* Container< __VA_ARGS__ >::get<typename Container< __VA_ARGS__ >::uint_type>() const noexcept; \
  extern template bool                                            const* Container< __VA_ARGS__ >::get<bool>() const noexcept; \
  extern template bool Container< __VA_ARGS__ >::get(typename Container< __VA_ARGS__ >::map_type&) const noexcept; \
  extern template bool Container< __VA_ARGS__ >::get(typename Container< __VA_ARGS__ >::vector_type&) const noexcept; \
  extern template bool Container< __VA_ARGS__ >::get(typename Container< __VA_ARGS__ >::str_type&) const noexcept; \
  extern template bool Container< __VA_ARGS__ >::get(typename Container< __VA_ARGS__ >::float_type&) const noexcept; \
  extern template bool Container< __VA_ARGS__ >::get(typename Container< __VA_ARGS__ >::int_type&) const noexcept; \
  extern template bool Container< __VA_ARGS__ >::get(typename Container< __VA_ARGS__ >::uint_type&) const noexcept; \
  extern template bool Container< __VA_ARGS__ >::get(bool&) const noexcept; \
  /* Conversions */ \
  extern template std::nullptr_t                                  Container< __VA_ARGS__ >::convert_to<std::nullptr_t, 0>() const; \
  extern template typename Container< __VA_ARGS__ >::map_type     Container< __VA_ARGS__ >::convert_to<typename Container< __VA_ARGS__ >::map_type, 0>() const; \
  extern template typename Container< __VA_ARGS__ >::vector_type  Container< __VA_ARGS__ >::convert_to<typename Container< __VA_ARGS__ >::vector_type, 0>() const; \
  extern template typename Container< __VA_ARGS__ >::str_type     Container< __VA_ARGS__ >::convert_to<typename Container< __VA_ARGS__ >::str_type, 0>() const; \
  extern template typename Container< __VA_ARGS__ >::float_type   Container< __VA_ARGS__ >::convert_to<typename Container< __VA_ARGS__ >::float_type, 0>() const; \
  extern template typename Container< __VA_ARGS__ >::int_type     Container< __VA_ARGS__ >::convert_to<typename Container< __VA_ARGS__ >::int_type, 0>() const; \
  extern template typename Container< __VA_ARGS__ >::uint_type    Container< __VA_ARGS__ >::convert_to<typename Container< __VA_ARGS__ >::uint_type, 0>() const; \
  extern template bool                                            Container< __VA_ARGS__ >::convert_to<bool, 0>() const; \
  extern template Container< __VA_ARGS__ >::operator std::nullptr_t() const; \
  extern template Container< __VA_ARGS__ >::operator typename Container< __VA_ARGS__ >::map_type() const; \
  extern template Container< __VA_ARGS__ >::operator typename Container< __VA_ARGS__ >::vector_type() const; \
  extern template Container< __VA_ARGS__ >::operator typename Container< __VA_ARGS__ >::str_type() const; \
  extern template Container< __VA_ARGS__ >::operator typename Container< __VA_ARGS__ >::float_type() const; \
  extern template Container< __VA_ARGS__ >::operator typename Container< __VA_ARGS__ >::int_type() const; \
  extern template Container< __VA_ARGS__ >::operator typename Container< __VA_ARGS__ >::uint_type() const; \
  extern template Container< __VA_ARGS__ >::operator bool() const; \
  extern template std::nullptr_t                                  Container< __VA_ARGS__ >::as<std::nullptr_t>() const; \
  extern template typename Container< __VA_ARGS__ >::map_type     Container< __VA_ARGS__ >::as<typename Container< __VA_ARGS__ >::map_type>() const; \
  extern template typename Container< __VA_ARGS__ >::vector_type  Container< __VA_ARGS__ >::as<typename Container< __VA_ARGS__ >::vector_type>() const; \
  extern template typename Container< __VA_ARGS__ >::str_type     Container< __VA_ARGS__ >::as<typename Container< __VA_ARGS__ >::str_type>() const; \
  extern template typename Container< __VA_ARGS__ >::float_type   Container< __VA_ARGS__ >::as<typename Container< __VA_ARGS__ >::float_type>() const; \
  extern template typename Container< __VA_ARGS__ >::int_type     Container< __VA_ARGS__ >::as<typename Container< __VA_ARGS__ >::int_type>() const; \
  extern template typename Container< __VA_ARGS__ >::uint_type    Container< __VA_ARGS__ >::as<typename Container< __VA_ARGS__ >::uint_type>() const; \
  extern template bool                                            Container< __VA_ARGS__ >::as<bool>() const; \
  /* lexical cast */ \
  extern template typename Container< __VA_ARGS__ >::float_type lexical_cast<typename Container< __VA_ARGS__ >::float_type, typename Container< __VA_ARGS__ >::str_type>(typename Container< __VA_ARGS__ >::str_type const&); \
  extern template typename Container< __VA_ARGS__ >::int_type lexical_cast<typename Container< __VA_ARGS__ >::int_type, typename Container< __VA_ARGS__ >::str_type>(typename Container< __VA_ARGS__ >::str_type const&); \
  extern template typename Container< __VA_ARGS__ >::uint_type lexical_cast<typename Container< __VA_ARGS__ >::uint_type, typename Container< __VA_ARGS__ >::str_type>(typename Container< __VA_ARGS__ >::str_type const&); \
  extern template bool lexical_cast<bool, typename Container< __VA_ARGS__ >::str_type>(typename Container< __VA_ARGS__ >::str_type const&); \
  /* STL */ \
  /*extern template class Container< __VA_ARGS__ >::vector_template< Container< __VA_ARGS__ > >;*/ \
}  // namespace nested_Container<>  


# define NESTED_CONTAINER_INSTANTIATE(Container,...) \
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
  template Container< __VA_ARGS__ >::Container(typename Container< __VA_ARGS__ >::str_type::value_type const*);\
  template Container< __VA_ARGS__ >::Container(typename Container< __VA_ARGS__ >::str_type::value_type*);\
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
  /* Array Ctors up to 12 */ \
  template Container< __VA_ARGS__ >::Container(typename Container< __VA_ARGS__ >::str_type::value_type const (&) [1]); \
  template Container< __VA_ARGS__ >::Container(typename Container< __VA_ARGS__ >::str_type::value_type const (&) [2]); \
  template Container< __VA_ARGS__ >::Container(typename Container< __VA_ARGS__ >::str_type::value_type const (&) [3]); \
  template Container< __VA_ARGS__ >::Container(typename Container< __VA_ARGS__ >::str_type::value_type const (&) [4]); \
  template Container< __VA_ARGS__ >::Container(typename Container< __VA_ARGS__ >::str_type::value_type const (&) [5]); \
  template Container< __VA_ARGS__ >::Container(typename Container< __VA_ARGS__ >::str_type::value_type const (&) [6]); \
  template Container< __VA_ARGS__ >::Container(typename Container< __VA_ARGS__ >::str_type::value_type const (&) [7]); \
  template Container< __VA_ARGS__ >::Container(typename Container< __VA_ARGS__ >::str_type::value_type const (&) [8]); \
  template Container< __VA_ARGS__ >::Container(typename Container< __VA_ARGS__ >::str_type::value_type const (&) [9]); \
  template Container< __VA_ARGS__ >::Container(typename Container< __VA_ARGS__ >::str_type::value_type const (&) [10]); \
  template Container< __VA_ARGS__ >::Container(typename Container< __VA_ARGS__ >::str_type::value_type const (&) [11]); \
  template Container< __VA_ARGS__ >::Container(typename Container< __VA_ARGS__ >::str_type::value_type const (&) [12]); \
  /* Assignment from other types */ \
  template Container< __VA_ARGS__ >& Container< __VA_ARGS__ >::operator=(typename Container< __VA_ARGS__ >::map_type&); \
  template Container< __VA_ARGS__ >& Container< __VA_ARGS__ >::operator=(typename Container< __VA_ARGS__ >::map_type const&); \
  template Container< __VA_ARGS__ >& Container< __VA_ARGS__ >::operator=(typename Container< __VA_ARGS__ >::map_type&&); \
  template Container< __VA_ARGS__ >& Container< __VA_ARGS__ >::operator=(typename Container< __VA_ARGS__ >::vector_type&); \
  template Container< __VA_ARGS__ >& Container< __VA_ARGS__ >::operator=(typename Container< __VA_ARGS__ >::vector_type const&); \
  template Container< __VA_ARGS__ >& Container< __VA_ARGS__ >::operator=(typename Container< __VA_ARGS__ >::vector_type&&); \
  template Container< __VA_ARGS__ >& Container< __VA_ARGS__ >::operator=(typename Container< __VA_ARGS__ >::str_type&); \
  template Container< __VA_ARGS__ >& Container< __VA_ARGS__ >::operator=(typename Container< __VA_ARGS__ >::str_type const&); \
  template Container< __VA_ARGS__ >& Container< __VA_ARGS__ >::operator=(typename Container< __VA_ARGS__ >::str_type&&); \
  template Container< __VA_ARGS__ >& Container< __VA_ARGS__ >::operator=(typename Container< __VA_ARGS__ >::float_type&); \
  template Container< __VA_ARGS__ >& Container< __VA_ARGS__ >::operator=(typename Container< __VA_ARGS__ >::float_type const&); \
  template Container< __VA_ARGS__ >& Container< __VA_ARGS__ >::operator=(typename Container< __VA_ARGS__ >::float_type&&); \
  template Container< __VA_ARGS__ >& Container< __VA_ARGS__ >::operator=(typename Container< __VA_ARGS__ >::int_type&); \
  template Container< __VA_ARGS__ >& Container< __VA_ARGS__ >::operator=(typename Container< __VA_ARGS__ >::int_type const&); \
  template Container< __VA_ARGS__ >& Container< __VA_ARGS__ >::operator=(typename Container< __VA_ARGS__ >::int_type&&); \
  template Container< __VA_ARGS__ >& Container< __VA_ARGS__ >::operator=(typename Container< __VA_ARGS__ >::uint_type&); \
  template Container< __VA_ARGS__ >& Container< __VA_ARGS__ >::operator=(typename Container< __VA_ARGS__ >::uint_type const&); \
  template Container< __VA_ARGS__ >& Container< __VA_ARGS__ >::operator=(typename Container< __VA_ARGS__ >::uint_type&&); \
  template Container< __VA_ARGS__ >& Container< __VA_ARGS__ >::operator=(bool&); \
  template Container< __VA_ARGS__ >& Container< __VA_ARGS__ >::operator=(bool const&); \
  template Container< __VA_ARGS__ >& Container< __VA_ARGS__ >::operator=(bool&&); \
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
  template Container< __VA_ARGS__ > const& Container< __VA_ARGS__ >::operator[](typename Container< __VA_ARGS__ >::key_type::value_type const*) const; \
  template Container< __VA_ARGS__ > const& Container< __VA_ARGS__ >::operator[](typename Container< __VA_ARGS__ >::key_type const&) const; \
  template Container< __VA_ARGS__ >& Container< __VA_ARGS__ >::operator[](typename Container< __VA_ARGS__ >::key_type::value_type const*); \
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
  /* Accessors */ \
  template typename Container< __VA_ARGS__ >::map_type*    Container< __VA_ARGS__ >::ptr_to<typename Container< __VA_ARGS__ >::map_type, 0>(); \
  template typename Container< __VA_ARGS__ >::vector_type* Container< __VA_ARGS__ >::ptr_to<typename Container< __VA_ARGS__ >::vector_type, 0>(); \
  template typename Container< __VA_ARGS__ >::str_type*    Container< __VA_ARGS__ >::ptr_to<typename Container< __VA_ARGS__ >::str_type, 0>(); \
  template typename Container< __VA_ARGS__ >::float_type*  Container< __VA_ARGS__ >::ptr_to<typename Container< __VA_ARGS__ >::float_type, 0>(); \
  template typename Container< __VA_ARGS__ >::int_type*    Container< __VA_ARGS__ >::ptr_to<typename Container< __VA_ARGS__ >::int_type, 0>(); \
  template typename Container< __VA_ARGS__ >::uint_type*   Container< __VA_ARGS__ >::ptr_to<typename Container< __VA_ARGS__ >::uint_type, 0>(); \
  template bool*                                           Container< __VA_ARGS__ >::ptr_to<bool, 0>(); \
  template typename Container< __VA_ARGS__ >::map_type     const* Container< __VA_ARGS__ >::ptr_to<typename Container< __VA_ARGS__ >::map_type, 0>() const; \
  template typename Container< __VA_ARGS__ >::vector_type  const* Container< __VA_ARGS__ >::ptr_to<typename Container< __VA_ARGS__ >::vector_type, 0>() const; \
  template typename Container< __VA_ARGS__ >::str_type     const* Container< __VA_ARGS__ >::ptr_to<typename Container< __VA_ARGS__ >::str_type, 0>() const; \
  template typename Container< __VA_ARGS__ >::float_type   const* Container< __VA_ARGS__ >::ptr_to<typename Container< __VA_ARGS__ >::float_type, 0>() const; \
  template typename Container< __VA_ARGS__ >::int_type     const* Container< __VA_ARGS__ >::ptr_to<typename Container< __VA_ARGS__ >::int_type, 0>() const; \
  template typename Container< __VA_ARGS__ >::uint_type    const* Container< __VA_ARGS__ >::ptr_to<typename Container< __VA_ARGS__ >::uint_type, 0>() const; \
  template bool                                            const* Container< __VA_ARGS__ >::ptr_to<bool, 0>() const; \
  template typename Container< __VA_ARGS__ >::map_type*    Container< __VA_ARGS__ >::get<typename Container< __VA_ARGS__ >::map_type>() noexcept; \
  template typename Container< __VA_ARGS__ >::vector_type* Container< __VA_ARGS__ >::get<typename Container< __VA_ARGS__ >::vector_type>() noexcept; \
  template typename Container< __VA_ARGS__ >::str_type*    Container< __VA_ARGS__ >::get<typename Container< __VA_ARGS__ >::str_type>() noexcept; \
  template typename Container< __VA_ARGS__ >::float_type*  Container< __VA_ARGS__ >::get<typename Container< __VA_ARGS__ >::float_type>() noexcept; \
  template typename Container< __VA_ARGS__ >::int_type*    Container< __VA_ARGS__ >::get<typename Container< __VA_ARGS__ >::int_type>() noexcept; \
  template typename Container< __VA_ARGS__ >::uint_type*   Container< __VA_ARGS__ >::get<typename Container< __VA_ARGS__ >::uint_type>() noexcept; \
  template bool*                                           Container< __VA_ARGS__ >::get<bool>() noexcept; \
  template typename Container< __VA_ARGS__ >::map_type     const* Container< __VA_ARGS__ >::get<typename Container< __VA_ARGS__ >::map_type>() const noexcept; \
  template typename Container< __VA_ARGS__ >::vector_type  const* Container< __VA_ARGS__ >::get<typename Container< __VA_ARGS__ >::vector_type>() const noexcept; \
  template typename Container< __VA_ARGS__ >::str_type     const* Container< __VA_ARGS__ >::get<typename Container< __VA_ARGS__ >::str_type>() const noexcept; \
  template typename Container< __VA_ARGS__ >::float_type   const* Container< __VA_ARGS__ >::get<typename Container< __VA_ARGS__ >::float_type>() const noexcept; \
  template typename Container< __VA_ARGS__ >::int_type     const* Container< __VA_ARGS__ >::get<typename Container< __VA_ARGS__ >::int_type>() const noexcept; \
  template typename Container< __VA_ARGS__ >::uint_type    const* Container< __VA_ARGS__ >::get<typename Container< __VA_ARGS__ >::uint_type>() const noexcept; \
  template bool                                            const* Container< __VA_ARGS__ >::get<bool>() const noexcept; \
  template bool Container< __VA_ARGS__ >::get(typename Container< __VA_ARGS__ >::map_type&) const noexcept; \
  template bool Container< __VA_ARGS__ >::get(typename Container< __VA_ARGS__ >::vector_type&) const noexcept; \
  template bool Container< __VA_ARGS__ >::get(typename Container< __VA_ARGS__ >::str_type&) const noexcept; \
  template bool Container< __VA_ARGS__ >::get(typename Container< __VA_ARGS__ >::float_type&) const noexcept; \
  template bool Container< __VA_ARGS__ >::get(typename Container< __VA_ARGS__ >::int_type&) const noexcept; \
  template bool Container< __VA_ARGS__ >::get(typename Container< __VA_ARGS__ >::uint_type&) const noexcept; \
  template bool Container< __VA_ARGS__ >::get(bool&) const noexcept; \
  /* Conversions */ \
  template std::nullptr_t                                  Container< __VA_ARGS__ >::convert_to<std::nullptr_t, 0>() const; \
  template typename Container< __VA_ARGS__ >::map_type     Container< __VA_ARGS__ >::convert_to<typename Container< __VA_ARGS__ >::map_type, 0>() const; \
  template typename Container< __VA_ARGS__ >::vector_type  Container< __VA_ARGS__ >::convert_to<typename Container< __VA_ARGS__ >::vector_type, 0>() const; \
  template typename Container< __VA_ARGS__ >::str_type     Container< __VA_ARGS__ >::convert_to<typename Container< __VA_ARGS__ >::str_type, 0>() const; \
  template typename Container< __VA_ARGS__ >::float_type   Container< __VA_ARGS__ >::convert_to<typename Container< __VA_ARGS__ >::float_type, 0>() const; \
  template typename Container< __VA_ARGS__ >::int_type     Container< __VA_ARGS__ >::convert_to<typename Container< __VA_ARGS__ >::int_type, 0>() const; \
  template typename Container< __VA_ARGS__ >::uint_type    Container< __VA_ARGS__ >::convert_to<typename Container< __VA_ARGS__ >::uint_type, 0>() const; \
  template bool                                            Container< __VA_ARGS__ >::convert_to<bool, 0>() const; \
  template Container< __VA_ARGS__ >::operator std::nullptr_t() const; \
  template Container< __VA_ARGS__ >::operator typename Container< __VA_ARGS__ >::map_type() const; \
  template Container< __VA_ARGS__ >::operator typename Container< __VA_ARGS__ >::vector_type() const; \
  template Container< __VA_ARGS__ >::operator typename Container< __VA_ARGS__ >::str_type() const; \
  template Container< __VA_ARGS__ >::operator typename Container< __VA_ARGS__ >::float_type() const; \
  template Container< __VA_ARGS__ >::operator typename Container< __VA_ARGS__ >::int_type() const; \
  template Container< __VA_ARGS__ >::operator typename Container< __VA_ARGS__ >::uint_type() const; \
  template Container< __VA_ARGS__ >::operator bool() const; \
  template std::nullptr_t                                  Container< __VA_ARGS__ >::as<std::nullptr_t>() const; \
  template typename Container< __VA_ARGS__ >::map_type     Container< __VA_ARGS__ >::as<typename Container< __VA_ARGS__ >::map_type>() const; \
  template typename Container< __VA_ARGS__ >::vector_type  Container< __VA_ARGS__ >::as<typename Container< __VA_ARGS__ >::vector_type>() const; \
  template typename Container< __VA_ARGS__ >::str_type     Container< __VA_ARGS__ >::as<typename Container< __VA_ARGS__ >::str_type>() const; \
  template typename Container< __VA_ARGS__ >::float_type   Container< __VA_ARGS__ >::as<typename Container< __VA_ARGS__ >::float_type>() const; \
  template typename Container< __VA_ARGS__ >::int_type     Container< __VA_ARGS__ >::as<typename Container< __VA_ARGS__ >::int_type>() const; \
  template typename Container< __VA_ARGS__ >::uint_type    Container< __VA_ARGS__ >::as<typename Container< __VA_ARGS__ >::uint_type>() const; \
  template bool                                            Container< __VA_ARGS__ >::as<bool>() const; \
  /* lexical cast */ \
  template typename Container< __VA_ARGS__ >::float_type lexical_cast<typename Container< __VA_ARGS__ >::float_type, typename Container< __VA_ARGS__ >::str_type>(typename Container< __VA_ARGS__ >::str_type const&); \
  template typename Container< __VA_ARGS__ >::int_type lexical_cast<typename Container< __VA_ARGS__ >::int_type, typename Container< __VA_ARGS__ >::str_type>(typename Container< __VA_ARGS__ >::str_type const&); \
  template typename Container< __VA_ARGS__ >::uint_type lexical_cast<typename Container< __VA_ARGS__ >::uint_type, typename Container< __VA_ARGS__ >::str_type>(typename Container< __VA_ARGS__ >::str_type const&); \
  template bool lexical_cast<bool, typename Container< __VA_ARGS__ >::str_type>(typename Container< __VA_ARGS__ >::str_type const&); \
}  // namespace nested_container  
//#else
//# define NESTED_COMPILER_EXTERNALIZE(...)
//# define NESTED_COMPILER_IMPLEMENT(...)
//#endif

#endif  // HEADER_GUARD_NESTED_CONTANER_EXTERNALIZE_
