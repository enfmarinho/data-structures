cmake_minimum_required(VERSION 3.5)
project(
  DataStructures
  VERSION 1.0.0
  LANGUAGES CXX)

# Create compile commands file.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

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

# Include required dependencies
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})
include_directories(include)

# Binary search tree
add_executable(binary_search_tree_test
               test/BinarySearchTree/BinarySearchTreeTest.cpp)
target_link_libraries(
  binary_search_tree_test
  PRIVATE ${GTEST_LIBRARIES}
  PRIVATE pthread)
target_compile_features(binary_search_tree_test PUBLIC cxx_std_17)

# Deque
add_executable(deque_test test/Deque/DequeTest.cpp)
target_link_libraries(
  deque_test
  PRIVATE ${GTEST_LIBRARIES}
  PRIVATE pthread)
target_compile_features(deque_test PUBLIC cxx_std_17)

# Hash table with separate chaining
add_executable(hash_table_separate_chaining_test
               test/HashTable/HashTableSeparateChainingTest.cpp)
target_link_libraries(
  hash_table_separate_chaining_test
  PRIVATE ${GTEST_LIBRARIES}
  PRIVATE pthread)
target_compile_features(hash_table_separate_chaining_test PUBLIC cxx_std_17)

# Heap
add_executable(heap_test test/Heap/HeapTest.cpp)
target_link_libraries(
  heap_test
  PRIVATE ${GTEST_LIBRARIES}
  PRIVATE pthread)
target_compile_features(heap_test PUBLIC cxx_std_17)

# Double Linked list
add_executable(double_linked_list_test test/LinkedList/DoubleLinkedListTest.cpp)
target_link_libraries(
  double_linked_list_test
  PRIVATE ${GTEST_LIBRARIES}
  PRIVATE pthread)
target_compile_features(double_linked_list_test PUBLIC cxx_std_17)

# Queue
add_executable(queue_test test/Queue/QueueTest.cpp)
target_link_libraries(
  queue_test
  PRIVATE ${GTEST_LIBRARIES}
  PRIVATE pthread)
target_compile_features(queue_test PUBLIC cxx_std_17)

# Red black tree with fat nodes
add_executable(red_black_tree_fat_nodes_test
               test/RedBlackTree/RedBlackTreeFatNodesTest.cpp)
target_link_libraries(
  red_black_tree_fat_nodes_test
  PRIVATE ${GTEST_LIBRARIES}
  PRIVATE pthread)
target_compile_features(red_black_tree_fat_nodes_test PUBLIC cxx_std_17)

# Red black tree unique
add_executable(red_black_tree_unique_test
               test/RedBlackTree/RedBlackTreeUniqueTest.cpp)
target_link_libraries(
  red_black_tree_unique_test
  PRIVATE ${GTEST_LIBRARIES}
  PRIVATE pthread)
target_compile_features(red_black_tree_unique_test PUBLIC cxx_std_17)

# Segment tree
add_executable(segment_tree_test test/SegmentTree/SegmentTreeTest.cpp)
target_link_libraries(
  segment_tree_test
  PRIVATE ${GTEST_LIBRARIES}
  PRIVATE pthread)
target_compile_features(segment_tree_test PUBLIC cxx_std_17)

# Stack
add_executable(stack_test test/Stack/StackTest.cpp)
target_link_libraries(
  stack_test
  PRIVATE ${GTEST_LIBRARIES}
  PRIVATE pthread)
target_compile_features(stack_test PUBLIC cxx_std_17)

# Trie
add_executable(trie_test test/Trie/TrieTest.cpp)
target_link_libraries(
  trie_test
  PRIVATE ${GTEST_LIBRARIES}
  PRIVATE pthread)
target_compile_features(trie_test PUBLIC cxx_std_17)

# Union find
add_executable(union_find_test test/UnionFind/UnionFindTest.cpp)
target_link_libraries(
  union_find_test
  PRIVATE ${GTEST_LIBRARIES}
  PRIVATE pthread)
target_compile_features(union_find_test PUBLIC cxx_std_17)

# Vector
add_executable(vector_test test/Vector/VectorTest.cpp)
target_link_libraries(
  vector_test
  PRIVATE ${GTEST_LIBRARIES}
  PRIVATE pthread)
target_compile_features(vector_test PUBLIC cxx_std_17)
