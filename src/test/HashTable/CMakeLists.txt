cmake_minimum_required(VERSION 3.5)
project(
  HashTable
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
add_executable(hash_table_separate_chaining_tests
               tests/HashTableSeparateChainingTests.cpp)

# Link with the google test libraries.
target_link_libraries(
  hash_table_separate_chaining_tests
  PRIVATE ${GTEST_LIBRARIES}
  PRIVATE pthread)
target_compile_features(hash_table_separate_chaining_tests PUBLIC cxx_std_17)
