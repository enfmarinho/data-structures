#ifndef DOUBLE_LINKED_LIST_H
#define DOUBLE_LINKED_LIST_H

#include <climits>    // INT_MAX
#include <cstddef>    // size_t
#include <functional> // function<>, equal_to<>, greater<>
#include <initializer_list>
#include <iterator> // distance()

// Namespace for sequential containers(sc).
namespace sc {
template <typename T> class list {
public:
  // Forward declaration.
  class iterator;
  struct Node;
  //=== Aliases.
  using value_type = T;
  using pointer = value_type *;
  using const_pointer = const value_type *;
  using node_pointer = Node *;
  using const_node_pointer = const Node *;
  using reference = value_type &;
  using const_reference = const value_type &;
  using size_type = size_t;
  using difference_type = std::ptrdiff_t;
  using list_reference = list &;
  using const_list_reference = const list &;
  using Compare = std::function<bool(const_reference, const_reference)>;

  struct Node {
    value_type data;       //!< Data stored.
    node_pointer next;     //!< Pointer to the next node.
    node_pointer previous; //!< Pointer to the previous node.
    Node(value_type data = value_type(), node_pointer next = nullptr,
         node_pointer previous = nullptr)
        : data(data), next(next), previous(previous) {}
  };

  ///=== [I] Special Functions.
  ///=== [II] Element Access.
  ///=== [III] Iterators.
  ///=== [IV] Capacity.
  ///=== [V] Modifiers.
  ///=== [VI] Operations.

  class iterator {
  public:
    ///=== Aliases.
    using value_type = T;
    using pointer = value_type *;
    using const_pointer = const value_type *;
    using reference = value_type &;
    using const_reference = const value_type &;
    using size_type = size_t;
    using difference_type = std::ptrdiff_t;

    iterator() = default;
    iterator(node_pointer pointer) : m_pointer{pointer} {}
    ~iterator() = default;
    node_pointer operator&() { return m_pointer; }
    // const_node_pointer operator&() const { return m_pointer; }
    reference operator*() { return m_pointer->data; }
    const_reference operator*() const { return m_pointer->data; }
    iterator operator++() {
      m_pointer = m_pointer->next;
      return *this;
    }
    iterator operator++(int) {
      iterator copy = *this;
      ++(*this);
      return copy;
    }
    friend iterator operator+(iterator it, size_type increment) {
      for (size_type counter{0}; counter < increment; ++counter) {
        ++it;
      }
      for (size_type counter{0}; counter > increment; --counter) {
        --it;
      }
      return it;
    }
    friend iterator operator+(size_type increment, iterator it) {
      return it + increment;
    }
    iterator operator--() {
      m_pointer = m_pointer->previous;
      return *this;
    }
    iterator operator--(int) {
      iterator copy = *this;
      --(*this);
      return copy;
    }
    friend iterator operator-(iterator it, size_type decrement) {
      return it + (-decrement);
    }
    difference_type operator-(const iterator &right) {
      iterator runner = right;
      difference_type counter{0};
      while (runner != nullptr) {
        if (runner.m_pointer == m_pointer) {
          return counter;
        }
        ++counter;
        ++runner;
      }
      runner = right;
      counter = 0;
      while (runner != nullptr) {
        if (runner.m_pointer == m_pointer) {
          return counter;
        }
        --runner;
        --counter;
      }
      return INT_MAX; // Case iterators can not reach each other.
    }
    iterator operator=(const iterator &copy) {
      m_pointer = copy.m_pointer;
      return *this;
    }
    bool operator!=(const iterator &right) {
      return this->m_pointer != right.m_pointer;
    }
    bool operator==(const iterator &right) {
      return this->m_pointer == right.m_pointer;
    }

  protected:
    node_pointer m_pointer{nullptr};
  };

private:
  node_pointer m_head; //!< Pointer to the head node.
  node_pointer m_tail; //!< Pointer to the tail node.
  size_type m_size{0}; //!< Number of elements in the list.
};
} // namespace sc

#endif
