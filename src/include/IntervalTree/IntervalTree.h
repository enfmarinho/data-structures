#ifndef SRC_INCLUDE_INTERVALTREE_INTERVALTREE_H_
#define SRC_INCLUDE_INTERVALTREE_INTERVALTREE_H_

#include <cstddef> // size_t, ptrdiff_t
#include <cstdint> // int64_t
#include <vector>

// Namespace for tree data structures
namespace tree {
/*!
 * TODO document
 */
template <typename T> class IntervalTree {
public:
  //=== Forward declaration
  struct Node;
  //=== Aliases.
  using value_type = int64_t;
  using pointer = value_type *;
  using const_pointer = const value_type *;
  using reference = value_type &;
  using const_reference = const value_type &;
  using size_type = size_t;
  using difference_type = std::ptrdiff_t;
  using node_pointer = Node *;

  struct Node {
    value_type point;
    node_pointer right;
    node_pointer left;
    std::vector<value_type> interval_by_begin;
    std::vector<value_type> interval_by_end;
  };

  ///=== [I] Special Functions.
  ///=== [II] Element Access.
  ///=== [III] Iterators.
  ///=== [IV] Capacity.
  ///=== [V] Modifiers.
  ///=== [VI] Operations.
};
} // namespace tree

#endif // SRC_INCLUDE_INTERVALTREE_INTERVALTREE_H_
