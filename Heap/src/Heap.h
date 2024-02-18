#ifndef HEAP_H
#define HEAP_H

#include <cstddef>    // size_t, ptrdiff_t
#include <functional> // less
#include <initializer_list>
#include <utility> // swap
#include <vector>

// Namespace for tree data-structures.
namespace tree {
/*!
 * Heap data structure. By default is a max-heap, but it can be changed by
 * defining another compare function.
 * \template T data type to store.
 * \template Container underlying container to be used. Default is vector.
 * \template Compare compare function to be used. Default is std::less.
 */
template <typename T, class Container = std::vector<T>,
          class Compare = std::less<typename Container::value_type>>
class Heap {
public:
  //=== Aliases.
  using value_type = T;
  using pointer = value_type *;
  using const_pointer = const value_type *;
  using reference = value_type &;
  using const_reference = const value_type &;
  using size_type = size_t;
  using difference_type = std::ptrdiff_t;

  ///=== [I] Special Functions.
  /// Creates an empty Heap.
  Heap() = default;
  /*!
   * Creates a Heap with the values in the range[first; last).
   * \param first start of the range.
   * \param last end of the range (not included).
   */
  template <class InputIt> Heap(InputIt first, InputIt last) {
    while (first != last) {
      push(*(first++));
    }
  }
  /*!
   * Creates a Heap with the values in "ilist".
   * \param ilist initializer_list with the values to insert.
   */
  Heap(std::initializer_list<value_type> ilist) { *this = ilist; }
  /*!
   * Creates a Heap equivalent to "other".
   * \param other Heap to copy.
   */
  Heap(const Heap &other) { *this = other; }
  /*!
   * Creates a Heap from the memory of "other" moving it. "other" becomes
   * invalid after operation.
   * \param other heap to move from.
   */
  Heap(Heap &&other) { *this = other; }
  /// Deallocates heap memory.
  ~Heap() = default;
  /*!
   * Makes this Heap contains only the values from initializer list "ilist".
   * \param ilist initializer_list with the values to insert.
   */
  Heap &operator=(const std::initializer_list<value_type> ilist) {
    m_data.clear();
    for (value_type value : ilist) {
      push(value);
    }
    return *this;
  }
  /*!
   * Makes this Heap be equivalent to "other".
   * \param other Heap to copy.
   */
  Heap &operator=(const Heap &other) {
    m_data = other.m_data;
    return *this;
  }
  /*!
   * Makes this Heap from the memory of "other" moving it. "other" becomes
   * invalid after operation.
   * \param other heap to move from.
   */
  Heap &operator=(Heap &&other) {
    m_data = std::move(other.m_data);
    return *this;
  }

  ///=== [II] Element Access.
  /// Consults first element in the container, i.e. root of the tree.
  const_reference top() const { return m_data[0]; }

  ///=== [IV] Capacity.
  /// Checks whether the underlying container is empty.
  bool empty() { return m_data.empty(); }
  /// Consults the number of elements in the container.
  size_type size() { return m_data.size(); }

  ///=== [V] Modifiers.
  /*!
   * Inserts value in the container.
   * \param value data to insert.
   */
  void push(const_reference value) {
    m_data.push_back(value);
    size_type value_index = m_data.size() - 1;
    while (Compare{}(parent(value_index), value)) {
      size_type parent_index = (value_index - 1) / 2;
      std::swap(m_data[value_index], m_data[parent_index]);
      value_index = parent_index;
    }
  }
  /// Removes first value in the container, i.e. root of the tree.
  void pop() {
    m_data[0] = m_data.back();
    m_data.pop_back();
    size_type curr_index = 0;
    while (Compare{}(m_data[curr_index], left_child(curr_index)) or
           Compare{}(m_data[curr_index], right_child(curr_index))) {
      size_type new_index;
      if (Compare{}(left_child(curr_index), right_child(curr_index))) {
        new_index = curr_index * 2 + 2;
      } else {
        new_index = curr_index * 2 + 1;
      }
      std::swap(m_data[curr_index], m_data[new_index]);
      curr_index = new_index;
    }
  }

private:
  /// Return the value of the parent of the node at "index". If called for
  /// index 0, the value of itself is returned.
  value_type parent(size_type index) {
    if (index == 0) {
      return m_data[0];
    }
    return m_data[(index - 1) / 2];
  }
  /// Return the value of the left child of the node at "index". If called for
  /// a node without left child, the value of itself is returned.
  value_type left_child(size_type index) {
    size_type left_child_index = index * 2 + 1;
    if (left_child_index < m_data.size()) {
      return m_data[left_child_index];
    }
    return m_data[index];
  }
  /// Return the value of the right child of the node at "index". If called for
  /// a node without right child, the value of itself is returned.
  value_type right_child(size_type index) {
    size_type right_child_index = index * 2 + 2;
    if (right_child_index < m_data.size()) {
      return m_data[right_child_index];
    }
    return m_data[index];
  }

  Container m_data; //!< Container with the data of the Heap.
};
} // namespace tree

#endif // HEAP_H
