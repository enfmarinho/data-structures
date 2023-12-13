cmake_minimum_required(VERSION 3.5)
project (BinarySearchTree VERSION 1.0.0 LANGUAGES CXX )

# Locate GTest package.
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

# Adding files to compile.
include_directories( src )
add_executable(run_tests tests/BinarySearchTreeTest.cpp)

# Link with the google test libraries.
target_link_libraries(run_tests PRIVATE ${GTEST_LIBRARIES} PRIVATE pthread )
target_compile_features(run_tests PUBLIC cxx_std_17)
