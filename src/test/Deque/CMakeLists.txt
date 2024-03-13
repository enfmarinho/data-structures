cmake_minimum_required(VERSION 3.5)
project(Deque LANGUAGES CXX)

# Append to existing flags.
add_compile_options(
  "-Wall"
  "-Werror"
  "-Wpedantic"
  "-Wshadow"
  "-Wold-style-cast"
  "-Wcast-align"
  "-Wcast-qual")
add_compile_options("-g") # Flag to produce debug info.

# Include required dependencies.
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})
include_directories(../../include)

# Add and link executable.
add_executable(deque_test DequeTest.cpp)
target_link_libraries(
  deque_test
  PRIVATE ${GTEST_LIBRARIES}
  PRIVATE pthread)
target_compile_features(deque_test PUBLIC cxx_std_17)
