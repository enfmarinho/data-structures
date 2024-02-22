cmake_minimum_required(VERSION 3.5)
project(
  SegmentTree
  VERSION 1.0.0
  LANGUAGES CXX)

# Locate GTest package.
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

# Appending to existing flags.
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g") # Flag to produce debug info.

# Adding files to compile.
include_directories(src)
add_executable(segment_tree_tests tests/SegmentTreeTests.cpp)

# Link with the google test libraries.
target_link_libraries(
  segment_tree_tests
  PRIVATE ${GTEST_LIBRARIES}
  PRIVATE pthread)
target_compile_features(segment_tree_tests PUBLIC cxx_std_17)
