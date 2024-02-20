# Data Structures

This repository contain a collection of data structures written in C++.

# Testing

## Requirements

 - C++ compiler that supports at least C++17. I recommend clang or gcc
 - googletest framework. To install it run the commands bellow:
```bash
git clone https://github.com/google/googletest.git && cd googletest
cmake -S . -B build
cmake --build build
cd build
sudo make install
```
## Running

Go to the directory of the data structure you want to test and then run:
```bash
cmake -S . -B build
cmake --build build
```
This will trigger the compilation process, generating the binary file in the
'build' directory. The name of the executable depends on the data structure to
test. For example, lets say you want to test the vector data structure, the
executable is called "vector_test", so you would just need to run:
```bash
./build/vector_test
```
You can use the ls command to find out the name of the executable file.

# List of implemented Data structures

- [Binary search tree](BinarySearchTree/src/BinarySearchTree.h)
- [Deque](Deque/src/Deque.h)
- [Hash table](HashTable/src/HashTableSeparateChaining.h)
- [Heap](Heap/src/Heap.h)
- [Linked list](LinkedList/src/DoubleLinkedList.h)
- [Queue](Queue/src/Queue.h)
- [Red black tree](RedBlackTree/src/RedBlackTree.h)
- [Segment tree](SegmentTree/src/SegmentTree.h)
- [Stack](Stack/src/Stack.h)
- [Union find](UnionFind/src/UnionFind.h)
- [Vector](Vector/src/Vector.h)

# Using a implementation

To use any of the data structure here you just need to copy the .h file of its
implementation and include in your project. The tests can be used as a code demo
and the implementation is, in most cases, well documented to facilitate use.
