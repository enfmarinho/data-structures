# Data Structures

This repository contain a collection of data structures written in C++.

# Testing

## Requirements

 - C++ compiler that supports at least C++17. I recommend clang or gcc.
 - Cmake version 3.5 or greater.
 - Googletest framework. To install it run the commands bellow:
```bash
git clone https://github.com/google/googletest.git && cd googletest
cmake -S . -B build && cd build
sudo make install
```
## Compiling

There are two compile options: compile everything at once or compile just a 
specific test. For the former, go to the directory "src", for the latter, go to 
the directory of the test you want to compile. Then run: 
```bash
cmake -S . -B build
cmake --build build
```
## Running

If you chose to compile everything at once, the executable files are store at 
the directory build/bin, so just run: 
```bash
./build/bin/NAME
```
If you compiled just a specific test, the executable will be stored at the build
directory, so just run:
```bash
./build/NAME
```
Where NAME is the name of the executable to run. The name of the executables are
just the name of the files in snake case style, for example, the executable 
generated for BinarySearchTreeTest.cpp is named binary_search_tree_test.

# Implemented data structures

- [Binary search tree](src/include/BinarySearchTree/BinarySearchTree.h)
- [Deque](src/include/Deque/Deque.h)
- [Hash table](src/include/HashTable/HashTableSeparateChaining.h)
- [Heap](src/include/Heap/Heap.h)
- [Double Linked list](src/include/LinkedList/DoubleLinkedList.h)
- [Queue](src/include/Queue/Queue.h)
- [Red black tree](src/include/RedBlackTree/RedBlackTreeUnique.h)
- [Red black tree with fat nodes](src/include/RedBlackTree/RedBlackTreeFatNodes.h)
- [Segment tree](src/include/SegmentTree/SegmentTree.h)
- [Stack](src/include/Stack/Stack.h)
- [Trie](src/include/Trie/Trie.h)
- [Union find](src/include/UnionFind/UnionFind.h)
- [Vector](src/include/Vector/Vector.h)

# Using a implementation

To use any of the data structure here you just need to copy the .h file of its
implementation and include in your project. The tests can be used as a code demo
and the implementation is, in most cases, well documented to facilitate use.

# License

This repository is released under the [MIT license](https://opensource.org/licenses/MIT).
In short, this means you are free to use this software in any personal, 
open-source or commercial projects. Attribution is optional but appreciated.
