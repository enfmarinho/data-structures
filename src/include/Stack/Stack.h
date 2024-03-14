#ifndef SRC_INCLUDE_STACK_STACK_H_
#define SRC_INCLUDE_STACK_STACK_H_

#include <cstddef> // size_t, ptrdiff_t
#include <deque>
#include <initializer_list>

// Namespace for sequential container(sc).
namespace sc {
/*!
 * Container adapter that provides the functionality of a stack(LIFO). Acts as a
 * wrapper to the underlying container.
 * \tparam T type of data to store.
 * \tparam Container type of the underlying container to store the data. The
 *         container must satisfy the requirements of a Sequence Container.
 *         Default = std::deque.
 *
 * \author Eduardo Marinho (eduardo.nestor.marinho228@gmail.com)
 */
template <class T, class Container = std::deque<T>> class stack {
public:
  //=== Aliases.
  using value_type = T;
  using pointer = value_type *;
  using const_pointer = const value_type *;
  using reference = value_type &;
  using const_reference = const value_type &;
  using size_type = size_t;
  using difference_type = std::ptrdiff_t;
  using stack_reference = stack &;

  ///=== [I] Special Functions.
  /// Creates a empty container.
  stack() = default;
  /// Make this be a copy of "other".
  stack(stack_reference other) = default;
  /*!
   * Makes this contain the same data stored in the range [begin, end).
   * \param begin beginning of the range.
   * \param end ending of the range (not included).
   */
  template <typename InputIt> stack(InputIt first, InputIt last) {
    m_container = Container(first, last);
  }
  /*!
   * Creates a container with the same size and the same elements as the ilist.
   * \param ilist initializer_list with elements to insert in the stack.
   */
  stack(std::initializer_list<value_type> ilist) {
    m_container = Container(ilist);
  }
  /// Deallocates memory.
  ~stack() = default;
  /// Makes this be a copy of "other".
  stack_reference operator=(stack_reference other) {
    m_container = other.m_container;
  }

  ///=== [II] Element Access.
  /// Gets a reference to the last element in the container.
  reference top() { return m_container.back(); }
  /// Gets a constant reference to the last element in the container.
  const_reference top() const { return m_container.back(); }

  ///=== [III] Capacity.
  /// Checks whether the container is empty.
  [[nodiscard]] bool empty() { return size() == 0; }
  /// Consults the number of elements in the container.
  [[nodiscard]] size_type size() { return m_container.size(); }

  ///=== [IV] Modifiers.
  /// Inserts the given element in the back of the container.
  void push(const_reference value) { m_container.push_back(value); }
  /// Removes an element from the back of the container.
  void pop() { m_container.pop_back(); }

private:
  Container m_container; //!< Container that stores the data.
};
} // namespace sc

#endif // SRC_INCLUDE_STACK_STACK_H_
