cmake_minimum_required(VERSION 3.5)
project(RedBlackTree LANGUAGES CXX)

# Append to existing flags.
add_compile_options(
  "-Wall"
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

# Add and link executable for red black tree fat nodes test.
add_executable(red_black_tree_fat_nodes_test RedBlackTreeFatNodesTest.cpp)
target_link_libraries(
  red_black_tree_fat_nodes_test
  PRIVATE ${GTEST_LIBRARIES}
  PRIVATE pthread)
target_compile_features(red_black_tree_fat_nodes_test PUBLIC cxx_std_17)

# Add and link executable for red black tree unique test.
add_executable(red_black_tree_unique_test RedBlackTreeUniqueTest.cpp)
target_link_libraries(
  red_black_tree_unique_test
  PRIVATE ${GTEST_LIBRARIES}
  PRIVATE pthread)
target_compile_features(red_black_tree_unique_test PUBLIC cxx_std_17)
