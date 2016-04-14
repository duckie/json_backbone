cmake_minimum_required(VERSION 2.8)
include(CheckCXXCompilerFlag)

project(CppNestedContainer)
enable_testing()

include_directories(
  ${PROJECT_SOURCE_DIR}/include/
  ${PROJECT_SOURCE_DIR}/src/
  ${PROJECT_SOURCE_DIR}/test/
  ${PROJECT_BINARY_DIR}/include/
  ${PROJECT_BINARY_DIR}/src/
  ${PROJECT_BINARY_DIR}/test/
)

# Referencing local modules
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/test/cmake)

# C++14 is a requirement here
if (NOT MSVC)
  check_cxx_compiler_flag(-std=c++1y HAVE_STD_CPP14_FLAG)
  if (HAVE_STD_CPP14_FLAG)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y")
  else()
    message(FATAL_ERROR "Your compiler must support at least C++14 to compile ${PROJECT_NAME}.")
  endif()
endif()

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fcolor-diagnostics")
endif()

add_subdirectory(test)
