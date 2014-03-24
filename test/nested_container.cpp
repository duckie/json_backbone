#include <nested_container/container.hpp>

#define NESTED_COMPILER_USE_EXTERNALIZATION_MACROS
#include <nested_container/externalize.hpp>
NESTED_COMPILER_IMPLEMENT(basic_container);

#define NESTED_COMPILER_JSON_USE_EXTERNALIZATION_MACROS
#include <nested_container/externalize_json.hpp>
#include <nested_container/json.hpp>
NESTED_COMPILER_JSON_IMPLEMENT(basic_container);
