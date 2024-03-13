cmake_minimum_required(VERSION 3.5)
project(SegmentTree LANGUAGES CXX)

# Append to existing flags.
add_compile_options(
  "-Wall"
  "-Werror"
  "-Wpedantic"
  "-Wshadow"
  "-Wsign-compare"
  "-Wold-style-cast"
  "-Wcast-align"
  "-Wcast-qual")
# add_compile_options("-g") # Flag to produce debug info.

# Include required dependencies.
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})
include_directories(../../include)

# Add and link executable.
add_executable(segment_tree_test SegmentTreeTest.cpp)
target_link_libraries(
  segment_tree_test
  PRIVATE ${GTEST_LIBRARIES}
  PRIVATE pthread)
target_compile_features(segment_tree_test PUBLIC cxx_std_17)
