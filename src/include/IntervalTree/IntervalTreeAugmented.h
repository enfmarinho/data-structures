#ifndef SRC_INCLUDE_INTERVALTREE_INTERVALTREEAUGMENTED_H_
#define SRC_INCLUDE_INTERVALTREE_INTERVALTREEAUGMENTED_H_

#include <cstddef> // size_t, ptrdiff_t

// Namespace for tree data structures
namespace tree {
template <typename T> class IntervalTreeAugmented {
public:
  //=== Forward declaration
  struct Node;
  //=== Aliases.
  using value_type = long long int;
  using pointer = value_type *;
  using const_pointer = const value_type *;
  using reference = value_type &;
  using const_reference = const value_type &;
  using size_type = size_t;
  using difference_type = std::ptrdiff_t;
  using node_pointer = Node *;

  struct Node {
    value_type start;
    value_type end;
    value_type max_end;
    node_pointer right_child;
    node_pointer left_child;
  };

  ///=== [I] Special Functions.
  ///=== [II] Element Access.
  ///=== [III] Iterators.
  ///=== [IV] Capacity.
  ///=== [V] Modifiers.
  ///=== [VI] Operations.
private:
  node_pointer m_root;
  size_type m_size;
};
} // namespace tree

#endif // SRC_INCLUDE_INTERVALTREE_INTERVALTREEAUGMENTED_H_
