Find_Package(Threads REQUIRED)
Find_Package(GTest REQUIRED)

# Externalized container
add_library(nested_container OBJECT nested_container.cc)
# This one is only used to test it compiles correctly
add_library(nested_container_ext OBJECT EXCLUDE_FROM_ALL nested_container_ext.cc)
# GTest runner
add_library(main_gtest OBJECT main.cc)

# Macro to declare tests
include_directories(${GTEST_INCLUDE_DIRS})
add_custom_target(build-test)
function(add_nc_test name)
  set(test_obj_libs $<TARGET_OBJECTS:nested_container>)
  if("${ARGV1}" STREQUAL "GT")
    add_executable(${name} ${name}.cc $<TARGET_OBJECTS:main_gtest> ${test_obj_libs})
  else()
    add_executable(${name} ${name}.cc ${test_obj_libs})
  endif()
  target_link_libraries(${name} ${GTEST_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})
  add_dependencies(build-test ${name})
  add_test(${name} ${name})  
endfunction()

# Test declarations
add_nc_test(readme_demos)
add_nc_test(unit_tests)
add_nc_test(perf_json_serialize)
add_nc_test(perf_json_parse)
add_nc_test(container_default GT)
