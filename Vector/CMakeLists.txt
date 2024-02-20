cmake_minimum_required(VERSION 3.5)
project(
  Vector
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
add_executable(vector_test tests/VectorTest.cpp)

# Link with the google test libraries.
target_link_libraries(
  vector_test
  PRIVATE ${GTEST_LIBRARIES}
  PRIVATE pthread)
target_compile_features(vector_test PUBLIC cxx_std_17)
