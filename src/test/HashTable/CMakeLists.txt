cmake_minimum_required(VERSION 3.5)
project(HashTable LANGUAGES CXX)

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

# Add and link executable.
add_executable(hash_table_separate_chaining_tests
               HashTableSeparateChainingTest.cpp)
target_link_libraries(
  hash_table_separate_chaining_tests
  PRIVATE ${GTEST_LIBRARIES}
  PRIVATE pthread)
target_compile_features(hash_table_separate_chaining_tests PUBLIC cxx_std_17)
