#ifndef SRC_INCLUDE_QUEUE_QUEUE_H_
#define SRC_INCLUDE_QUEUE_QUEUE_H_

#include <cstddef> // size_t
#include <deque>
#include <initializer_list>

// Namespace for sequential container(sc).
namespace sc {
/*!
 * Container adapter that provides the functionality of a queue(FIFO). Acts as a
 * wrapper to the underlying container.
 * \tparam T data type to store.
 * \tparam Container type of the underlying container to store the data. The
 *         container must satisfy the requirements of a Sequence Container.
 *         Default = std::deque.
 *
 * \author Eduardo Marinho (eduardo.nestor.marinho228@gmail.com)
 */
template <class T, class Container = std::deque<T>> class queue {
public:
  //=== Aliases.
  using value_type = T;
  using pointer = value_type *;
  using const_pointer = const value_type *;
  using reference = value_type &;
  using const_reference = const value_type &;
  using size_type = size_t;
  using difference_type = std::ptrdiff_t;
  using queue_reference = queue &;

  ///=== [I] Special Functions.
  /// Creates a empty container.
  queue() = default;
  /// Make this queue be a copy of "other".
  queue(queue_reference other) = default;
  /*!
   * Makes this contain the same data stored in the range [begin, end).
   * \param begin beginning of the range.
   * \param end ending of the range (not included).
   */
  template <typename InputIt> queue(InputIt first, InputIt last) {
    m_container = Container(first, last);
  }
  /*!
   * Creates a container with the same size and the same elements as the ilist.
   * \param ilist initializer_list with elements to insert in the queue.
   */
  queue(std::initializer_list<value_type> ilist) {
    m_container = Container(ilist);
  }
  /// Deallocates memory.
  ~queue() = default;
  /// Makes this queue be a copy of "other".
  queue_reference operator=(queue_reference other) {
    m_container = other.m_container;
  }

  ///=== [II] Element Access.
  /// Gets a reference to the first element in the container.
  reference front() { return m_container.front(); }
  /// Gets a constant reference to the first element in the container.
  const_reference front() const { return m_container.front(); }
  /// Gets a reference to the last element in the container.
  reference back() { return m_container.back(); }
  /// Gets a constant reference to the last element in the container.
  const_reference back() const { return m_container.back(); }

  ///=== [III] Capacity.
  /// Checks whether the container is empty.
  [[nodiscard]] bool empty() { return size() == 0; }
  /// Consults the number of elements in the container.
  [[nodiscard]] size_type size() { return m_container.size(); }

  ///=== [IV] Modifiers.
  /// Inserts the given element in the front of the container.
  void push(const_reference value) { m_container.push_back(value); }
  /// Removes an element from the front of the container.
  void pop() { m_container.pop_front(); }

private:
  Container m_container; //!< Container that stores the data.
};
} // namespace sc

#endif // SRC_INCLUDE_QUEUE_QUEUE_H_
