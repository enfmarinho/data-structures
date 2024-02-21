#ifndef SEGMENT_TREE_H
#define SEGMENT_TREE_H

#include <cstddef>    // size_t, ptrdiff_t
#include <functional> // function
#include <limits>     // numeric_limits
#include <utility>    // move
#include <vector>

namespace operations {
template <class T> struct Sum {
  T operator()(const T &lhs, const T &rhs) { return lhs + rhs; }
  static T neutral_element() { return 0; }
};
template <class T> struct Max {
  T operator()(const T &lhs, const T &rhs) {
    if (lhs > rhs) {
      return lhs;
    }
    return rhs;
  }
  static T neutral_element() { return std::numeric_limits<T>::min(); }
};
template <class T> struct Min {
  T operator()(const T &lhs, const T &rhs) {
    if (lhs < rhs) {
      return lhs;
    }
    return rhs;
  }
  static T neutral_element() { return std::numeric_limits<T>::max(); }
};
}; // namespace operations

// Namespace for tree data-structures.
namespace tree {
template <typename T> class SegmentTree {
public:
  //=== Aliases.
  using value_type = T;
  using pointer = value_type *;
  using const_pointer = const value_type *;
  using reference = value_type &;
  using const_reference = const value_type &;
  using size_type = size_t;
  using difference_type = std::ptrdiff_t;
  using combine_func = std::function<value_type(value_type, value_type)>;

  ///=== [I] Special Functions.
  SegmentTree() = delete;
  SegmentTree(const std::vector<value_type> &list, combine_func func,
              value_type neutral_element)
      : m_combine(func), m_number_of_elements{list.size()},
        m_neutral_element{neutral_element} {
    m_tree.resize(4 * list.size());
    build(list, 1, 0, list.size());
  }
  SegmentTree(const SegmentTree &other) { *this = other; }
  SegmentTree(SegmentTree &&other) { *this = std::move(other); }
  SegmentTree &operator=(const SegmentTree &other) {
    m_combine = other.m_combine;
    m_number_of_elements = other.m_number_of_elements;
    m_neutral_element = other.m_neutral_element;
    m_tree = other.m_tree;
    other.m_combine = 0;
    return *this;
  }
  SegmentTree &operator=(SegmentTree &&other) {
    m_combine = std::move(other.m_combine);
    m_number_of_elements = std::move(other.m_number_of_elements);
    m_neutral_element = std::move(other.m_neutral_element);
    m_tree = std::move(other.m_tree);
    other.m_number_of_elements = 0;
    return *this;
  }
  ~SegmentTree() = default;

  ///=== [II] Element Access.
  value_type query(size_type interval_begin, size_type interval_end) {
    query(interval_begin, interval_end, 1, 0, m_number_of_elements - 1);
  }

  ///=== [III] Capacity.
  size_type size() const { return m_number_of_elements; }

  ///=== [IV] Modifiers.
  void update(size_type position, value_type value) {
    update(position, value, 1, 0, m_number_of_elements - 1);
  }

private:
  void build(const std::vector<value_type> &list, size_type index,
             size_type start, size_type end) {
    if (start == end) {
      m_tree[index] = list[start];
    }
    size_type mid = (start + end) / 2;
    m_tree[index] = m_combine(build(list, 2 * index, start, mid),
                              build(list, 2 * index + 1, mid + 1, end));
  }

  value_type query(size_type interval_begin, size_type interval_end,
                   size_type index, size_type begin, size_type end) {
    if (interval_end < begin or interval_begin > end) {
      return m_neutral_element;
    } else if (interval_begin <= begin and end <= interval_end) {
      return m_tree[index];
    }
    size_type mid = (begin + end) / 2;
    return m_combine(
        query(interval_begin, interval_end, 2 * index, begin, mid),
        query(interval_begin, interval_end, 2 * index + 1, mid + 1, end));
  }

  value_type update(size_type position, value_type value, size_type index,
                    size_type begin, size_type end) {
    if (position < begin or position > end) {
      return m_tree[index];
    } else if (begin == end) {
      return m_tree[index] = value;
    }
    size_type mid = (begin + end) / 2;
    return m_tree[position] =
               m_combine(update(position, value, 2 * index, begin, mid),
                         update(position, value, 2 * index + 1, mid + 1, end));
  }

  size_type m_number_of_elements;     //!< Number of elements in the container.
  const value_type m_neutral_element; //!< Neutral element of the operation.
  combine_func m_combine;             //!< Function to combine elements
  std::vector<value_type> m_tree;     //!< Stores tree nodes.
};
} // namespace tree

#endif // SEGMENT_TREE_H
