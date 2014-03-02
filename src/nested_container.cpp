#include "nested_container.hpp"

namespace nested_container {
template class basic_container<>;
template basic_container<>::basic_container(std::nullptr_t&& arg);
template basic_container<>::basic_container(typename basic_container<>::map_type&& arg);
template basic_container<>::basic_container(typename basic_container<>::array_type&& arg);
template basic_container<>::basic_container(typename basic_container<>::str_type&& arg);
template basic_container<>::basic_container(typename basic_container<>::float_type&& arg);
template basic_container<>::basic_container(typename basic_container<>::int_type&& arg);
template basic_container<>::basic_container(typename basic_container<>::uint_type&& arg);
template basic_container<>::basic_container(std::nullptr_t const& arg);
template basic_container<>::basic_container(typename basic_container<>::map_type const& arg);
template basic_container<>::basic_container(typename basic_container<>::array_type const& arg);
template basic_container<>::basic_container(typename basic_container<>::str_type const& arg);
template basic_container<>::basic_container(typename basic_container<>::float_type const& arg);
template basic_container<>::basic_container(typename basic_container<>::int_type const& arg);
template basic_container<>::basic_container(typename basic_container<>::uint_type const& arg);


template class basic_container<>::type_traits<typename basic_container<>::str_type>;
template class basic_container<>::type_traits<typename basic_container<>::str_type&&>;
template class basic_container<>::type_traits<typename basic_container<>::str_type const&>;

// TODO: and so on...
};

