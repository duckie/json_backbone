Find_Package(Threads REQUIRED)
Find_Package(rapidjson)

# Externalized container
#add_library(json_backbone OBJECT json_backbone.cc)
#add_library(json_backbone_boost OBJECT json_backbone_spirit.cc)
# This one is only used to test it compiles correctly
#add_library(json_backbone_ext OBJECT EXCLUDE_FROM_ALL json_backbone_ext.cc)
# GTest runner
add_library(catch_main OBJECT main.cc)

# Macro to declare tests
include_directories(${PROJECT_SOURCE_DIR}/external/catch/single_include)
add_custom_target(build-test)
function(add_nc_test name)
  if("${ARGV1}" STREQUAL "CATCH")
    #set(test_obj_libs $<TARGET_OBJECTS:json_backbone> ${ARGV2})
    set(test_obj_libs ${ARGV2})
    add_executable(${name} ${name}.cc $<TARGET_OBJECTS:catch_main> ${test_obj_libs} ${HEADER_FILES})
    target_link_libraries(${name} ${CMAKE_THREAD_LIBS_INIT})
    add_dependencies(build-test ${name})
    add_test(${name} ${name})  
  else()
    set(test_obj_libs $<TARGET_OBJECTS:json_backbone> ${ARGV1})
    add_executable(${name} ${name}.cc ${test_obj_libs} ${HEADER_FILES})
    target_link_libraries(${name} ${CMAKE_THREAD_LIBS_INIT})
    add_dependencies(build-test ${name})
    add_test(${name} ${name})  
  endif()
endfunction()

# Test declarations
#add_nc_test(readme_demos $<TARGET_OBJECTS:json_backbone_boost>)
add_nc_test(unit_tests CATCH)

#add_nc_test(unit_tests_boost_spirit GT $<TARGET_OBJECTS:json_backbone_boost>)
#add_nc_test(perf_json_serialize $<TARGET_OBJECTS:json_backbone_boost>)
#add_nc_test(container_default GT)
if (${RAPIDJSON_FOUND})
  include_directories(${RAPIDJSON_INCLUDE_DIRS})
  #add_nc_test(container_rapidjson GT)
  #add_nc_test(perf_json_parse $<TARGET_OBJECTS:json_backbone_boost>)
endif()
