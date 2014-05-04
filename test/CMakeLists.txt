add_library(nested_container OBJECT nested_container.cpp)
add_library(nested_container_ext OBJECT nested_container_ext.cpp)

add_executable(readme_demos readme_demos.cpp $<TARGET_OBJECTS:nested_container>)
add_executable(tests unit_tests.cpp $<TARGET_OBJECTS:nested_container>)
add_executable(perf_json_serialize perf_json_serialize.cpp $<TARGET_OBJECTS:nested_container>)
add_executable(perf_json_parse perf_json_parse.cpp $<TARGET_OBJECTS:nested_container>)
