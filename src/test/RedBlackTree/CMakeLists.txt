cmake_minimum_required(VERSION 3.5)
project(RedBlackTree LANGUAGES CXX)

# Append to existing flags.
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g") # Flag to produce debug info.

# Include required dependencies.
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})
include_directories(../../include)

# Add and link executable.
add_executable(red_black_tree_test RedBlackTreeTest.cpp)
target_link_libraries(
  red_black_tree_test
  PRIVATE ${GTEST_LIBRARIES}
  PRIVATE pthread)
target_compile_features(red_black_tree_test PUBLIC cxx_std_17)
