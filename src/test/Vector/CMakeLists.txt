cmake_minimum_required(VERSION 3.5)
project(Vector LANGUAGES CXX)

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
add_executable(vector_test VectorTest.cpp)
target_link_libraries(
  vector_test
  PRIVATE ${GTEST_LIBRARIES}
  PRIVATE pthread)
target_compile_features(vector_test PUBLIC cxx_std_17)
