Find_Package(Threads REQUIRED)
Find_Package(rapidjson)

# Externalized container
#add_library(json_backbone OBJECT json_backbone.cc)
#add_library(json_backbone_boost OBJECT json_backbone_spirit.cc)
# This one is only used to test it compiles correctly
#add_library(json_backbone_ext OBJECT EXCLUDE_FROM_ALL json_backbone_ext.cc)
# GTest runner
add_library(catch_main OBJECT main.cc)
set(catch_exe_source_list)

# Macro to declare tests
include_directories(${PROJECT_SOURCE_DIR}/external/catch/single_include)
add_custom_target(build-test)
macro(add_project_test name)
  if("${ARGV1}" STREQUAL "CATCH")
    list(APPEND catch_exe_source_list ${name}.cc)
  else()
    #set(test_obj_libs $<TARGET_OBJECTS:json_backbone> ${ARGV1})
    add_executable(${name} ${name}.cc ${test_obj_libs} ${HEADER_FILES})
    target_link_libraries(${name} ${CMAKE_THREAD_LIBS_INIT})
    add_dependencies(build-test ${name})
    add_test(${name} ${name})  
  endif()
endmacro()

# Test declarations
#add_nc_test(readme_demos $<TARGET_OBJECTS:json_backbone_boost>)
add_project_test(arithmetics CATCH)
add_project_test(variant CATCH)
add_project_test(container CATCH)
add_project_test(static CATCH)
add_project_test(readme_demos)

if (${RAPIDJSON_FOUND})
  include_directories(${RAPIDJSON_INCLUDE_DIRS})
endif()

# Bundle all tests in one executable. No problem witch catch tags
set(unit_tests_name "unit_tests")
add_executable(${unit_tests_name} ${catch_exe_source_list} $<TARGET_OBJECTS:catch_main>)
add_test(run_${unit_tests_name} ${unit_tests_name})
target_link_libraries(${unit_tests_name} ${CMAKE_THREAD_LIBS_INIT})
add_dependencies(build-test ${unit_tests_name})
