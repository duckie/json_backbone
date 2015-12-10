cmake_minimum_required(VERSION 2.8)

project(CppNestedContainer)
enable_testing()

include_directories(
  ${PROJECT_SOURCE_DIR}/includes/
  ${PROJECT_SOURCE_DIR}/src/
  ${PROJECT_SOURCE_DIR}/test/
  ${PROJECT_BINARY_DIR}/includes/
  ${PROJECT_BINARY_DIR}/src/
  ${PROJECT_BINARY_DIR}/test/
)

# Referencing local modules
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/test/cmake)

# C++11
include(CheckCXXCompilerFlag)
check_cxx_compiler_flag(-std=c++11 HAVE_STD_CPP11_FLAG)
if (HAVE_STD_CPP11_FLAG)
  message("-- C++11 support found.")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
else()
  message(FATAL_ERROR "Your compiler must support at least C++11 to compile ${PROJECT_NAME}.")
endif()

### Boost support ###
FIND_PACKAGE(Boost REQUIRED)
if(Boost_FOUND)
	include_directories(${Boost_INCLUDE_DIRS})
endif()

#add_subdirectory(test EXCLUDE_FROM_ALL)
add_subdirectory(test)
