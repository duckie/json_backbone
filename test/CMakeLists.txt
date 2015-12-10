Find_Package(Threads REQUIRED)
Find_Package(GTest)
Find_Package(rapidjson)

# Externalized container
add_library(nested_container OBJECT nested_container.cc)
# This one is only used to test it compiles correctly
add_library(nested_container_ext OBJECT EXCLUDE_FROM_ALL nested_container_ext.cc)
# GTest runner
if (${GTEST_FOUND})
  add_library(test-main OBJECT test-main.cc)
endif()

# Macro to declare tests
include_directories(${GTEST_INCLUDE_DIRS})
add_custom_target(build-test)
function(add_nc_test name)
  set(test_obj_libs $<TARGET_OBJECTS:nested_container>)
  if("${ARGV1}" STREQUAL "GT")
    if (GTEST_FOUND)
      add_executable(${name} ${name}.cc $<TARGET_OBJECTS:test-main> ${test_obj_libs} ${HEADER_FILES})
      target_link_libraries(${name} ${GTEST_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})
      add_dependencies(build-test ${name})
      add_test(${name} ${name})  
    endif()
  else()
    add_executable(${name} ${name}.cc ${test_obj_libs} ${HEADER_FILES})
    target_link_libraries(${name} ${CMAKE_THREAD_LIBS_INIT})
    add_dependencies(build-test ${name})
    add_test(${name} ${name})  
  endif()
endfunction()

# Test declarations
add_nc_test(readme_demos)
add_nc_test(unit_tests)
add_nc_test(perf_json_serialize)
add_nc_test(perf_json_parse)
add_nc_test(container_default GT)
if (${RAPIDJSON_FOUND})
  include_directories(${RAPIDJSON_INCLUDE_DIRS})
  add_nc_test(container_rapidjson GT)
endif()
