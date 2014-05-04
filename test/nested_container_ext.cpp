#include <nested_container/container_double.hpp>
#include <nested_container/container_longint_double.hpp>
#include <nested_container/container_longlong_longdouble.hpp>
#include <nested_container/externalize.hpp>
#include <nested_container/driver/json/json.hpp>
#include <nested_container/driver/json/externalize_json.hpp>

NESTED_CONTAINER_INSTANTIATE(NESTED_CONTAINER_CONTAINER_DBL_SIGNATURE());
NESTED_CONTAINER_INSTANTIATE(NESTED_CONTAINER_CONTAINER_L_DBL_SIGNATURE());
NESTED_CONTAINER_INSTANTIATE(NESTED_CONTAINER_CONTAINER_LL_LDBL_SIGNATURE());

NESTED_CONTAINER_INSTANTIATE_JSON(NESTED_CONTAINER_CONTAINER_DBL_SIGNATURE());
NESTED_CONTAINER_INSTANTIATE_JSON(NESTED_CONTAINER_CONTAINER_L_DBL_SIGNATURE());
NESTED_CONTAINER_INSTANTIATE_JSON(NESTED_CONTAINER_CONTAINER_LL_LDBL_SIGNATURE());
