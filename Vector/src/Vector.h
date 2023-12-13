#ifndef VECTOR_H
#define VECTOR_H

#include <initializer_list>
#include <vector>

// Namespace for sequential containers(sc).
namespace sc {
template <typename T> class Vector {
public:
  class iterator;
  class const_iterator;
  //=== Aliases.
  using value_type = T;
  using pointer = value_type *;
  using const_pointer = const value_type *;
  using reference = value_type &;
  using const_reference = const value_type &;
  using size_type = size_t;
  using difference_type = std::ptrdiff_t;

  //=== [I] Members Functions
  Vector(size_type size = 0, value_type value = T()) {
    if (size > 0) {
      m_size = size;
      m_array = new T[m_size];
      for (size_type index{0}; index < m_size; ++index) {
        m_array[index] = value;
      }
    }
  }
  Vector(std::initializer_list<value_type> ilist) { return assign(ilist); }

  Vector assing(std::initializer_list<value_type> ilist) {
    if (ilist.size() < m_size) {
      m_size = ilist.size();
    }
    size_type index{0};
    while (index < m_size) {
      m_array[index] = ilist[index];
      ++index;
    }
    for (; index < ilist.size(); ++index) {
      push_back(ilist[index]);
    }
    return *this;
  }
  Vector operator=(std::initializer_list<value_type> ilist) {
    return assign(ilist);
  }
  template <typename InputIt> Vector assign(InputIt begin, InputIt end) {
    while (begin != end) {
    }
  }
  template <typename InputIt> Vector(InputIt begin, InputIt end) {}

  //=== [II] Element Access
  // TODO
  void resize(size_type size) {}
  void reserve(size_type size) {}
  bool empty() const { return m_size == 0; }
  size_type size() const { return m_size; }
  size_type capacity() const { return m_capacity; }
  reference front() { return m_array[0]; }
  const_reference front() const { return m_array[0]; }
  reference back() { return m_array[m_size - 1]; }
  const_reference back() const { return m_array[m_size - 1]; }
  pointer data() const { return m_array; }
  reference at(size_type pos) { return m_array[pos]; }
  const_reference at(size_type pos) const { return m_array[pos]; }

  //=== [III] Iterators
  //=== [IV] Capacity
  //=== [V] Modifiers
  void clear() {}
  iterator insert(const_iterator pos, const_reference value) {}
  iterator insert(const_iterator pos, size_type count, const_reference value) {}
  template <class InputIt>
  iterator insert(const_iterator pos, InputIt first, InputIt last) {}
  iterator insert(const_iterator pos, std::initializer_list<T> ilist) {}
  void push_back(value_type value) {
    ++m_size;
    // TODO
  }

  // TODO
  class iterator {};
  // TODO
  class const_iterator {};

private:
  pointer m_array{nullptr};
  size_type m_size{0};
  size_type m_capacity{0};
};
} // namespace sc
  //
#include "incl.h"

#endif // VECTOR_H
