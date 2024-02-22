cmake_minimum_required(VERSION 3.5)
project(LinkedList LANGUAGES CXX)

# Append to existing flags.
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g") # Flag to produce debug info.

# Include required dependencies.
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})
include_directories(../../include)

# Add and link executable.
add_executable(double_linked_list_test DoubleLinkedListTest.cpp)
target_link_libraries(
  double_linked_list_test
  PRIVATE ${GTEST_LIBRARIES}
  PRIVATE pthread)
target_compile_features(double_linked_list_test PUBLIC cxx_std_17)
