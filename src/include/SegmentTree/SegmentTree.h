#ifndef SEGMENT_TREE_H
#define SEGMENT_TREE_H

#include <cstddef> // size_t
#include <limits>  // numeric_limits
#include <utility> // move
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
/*!
 * Segment tree. Stores information about intervals of the data, allowing query
 * of this intervals.
 * \template T data type to store.
 * \template Combine operation to realize on the intervals.
 *
 * \author Eduardo Marinho (eduardo.nestor.marinho228@gmail.com)
 */
template <typename T, class Combine> class SegmentTree {
public:
  //=== Aliases.
  using value_type = T;
  using size_type = size_t;

  ///=== [I] Special Functions.
  SegmentTree() = delete;
  /*!
   * Construct a segment tree from the elements of "list".
   * \param list list of elements.
   * \param neutral_element neutral element of the operation.
   */
  SegmentTree(const std::vector<value_type> &list, value_type neutral_element)
      : m_number_of_elements{list.size()}, m_neutral_element{neutral_element} {
    m_tree.resize(4 * list.size(), m_neutral_element);
    build(list, 1, 0, list.size() - 1);
  }
  /// Makes this container be equivalent to "other" by coping its data.
  SegmentTree(const SegmentTree &other)
      : m_neutral_element{other.m_neutral_element} {
    *this = other;
  }
  /// Makes this container take ownership of memory of "other".
  SegmentTree(SegmentTree &&other)
      : m_neutral_element{other.m_neutral_element} {
    *this = std::move(other);
  }
  /*!
   * Makes this container be equivalent to "other" by coping its data.
   * \param other container to copy.
   * \return reference to this container.
   */
  SegmentTree &operator=(const SegmentTree &other) {
    m_number_of_elements = other.m_number_of_elements;
    m_tree = other.m_tree;
    return *this;
  }
  /*!
   * Makes this container be equivalent to "other" by taking ownership of its
   * memory. "other" becomes invalid. \param other container to take memory
   * from. \return reference to this container.
   */
  SegmentTree &operator=(SegmentTree &&other) {
    m_number_of_elements = std::move(other.m_number_of_elements);
    m_tree = std::move(other.m_tree);
    other.m_number_of_elements = 0;
    return *this;
  }
  /// Default destructor.
  ~SegmentTree() = default;

  ///=== [II] Element Access.
  /*!
   * Consults the resultant value of a interval in the container.
   * \param interval_begin begin of the interval to consult.
   * \param interval_end end of the interval to consult (not include).
   */
  value_type query(size_type interval_begin, size_type interval_end) {
    return query(interval_begin, interval_end - 1, 1, 0,
                 m_number_of_elements - 1);
  }

  ///=== [III] Capacity.
  /// Consults the number of elements in the container.
  size_type size() const { return m_number_of_elements; }

  ///=== [IV] Modifiers.
  /*!
   * Changes a value in the container.
   * \param position index of the element to change.
   * \param value new value to store.
   */
  void update(size_type position, value_type value) {
    update(position, value, 1, 0, m_number_of_elements - 1);
  }

private:
  /// Builds the segment tree.
  value_type build(const std::vector<value_type> &list, size_type index,
                   size_type start, size_type end) {
    if (start == end) {
      return m_tree[index] = list[start];
    }
    size_type mid = (start + end) / 2;
    return m_tree[index] = Combine{}(build(list, 2 * index, start, mid),
                                     build(list, 2 * index + 1, mid + 1, end));
  }
  /// Consults a interval in the container.
  value_type query(size_type interval_begin, size_type interval_end,
                   size_type index, size_type begin, size_type end) {
    if (interval_end < begin or interval_begin > end) {
      return m_neutral_element;
    } else if (interval_begin <= begin and end <= interval_end) {
      return m_tree[index];
    }
    size_type mid = (begin + end) / 2;
    return Combine{}(
        query(interval_begin, interval_end, 2 * index, begin, mid),
        query(interval_begin, interval_end, 2 * index + 1, mid + 1, end));
  }
  /// Changes a value in the container.
  value_type update(size_type position, value_type value, size_type index,
                    size_type begin, size_type end) {
    if (position < begin or position > end) {
      return m_tree[index];
    } else if (begin == end) {
      return m_tree[index] = value;
    }
    size_type mid = (begin + end) / 2;
    return m_tree[index] =
               Combine{}(update(position, value, 2 * index, begin, mid),
                         update(position, value, 2 * index + 1, mid + 1, end));
  }

  size_type m_number_of_elements;     //!< Number of elements in the container.
  const value_type m_neutral_element; //!< Neutral element of the operation.
  std::vector<value_type> m_tree;     //!< Stores tree nodes.
};
} // namespace tree

#endif // SEGMENT_TREE_H
