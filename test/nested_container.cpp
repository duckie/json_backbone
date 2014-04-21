#include <nested_container/container.hpp>

#include <nested_container/externalize.hpp>
NESTED_CONTAINER_IMPLEMENT(basic_container);

#include <nested_container/externalize_json.hpp>
#include <nested_container/json.hpp>
NESTED_CONTAINER_JSON_IMPLEMENT(basic_container);
