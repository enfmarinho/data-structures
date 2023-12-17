#ifndef VECTOR_H
#define VECTOR_H

#include <initializer_list>
#include <iterator>

// Namespace for sequential containers(sc).
namespace sc {
template <typename T> class Vector {
public:
  class iterator;
  //=== Aliases.
  using value_type = T;
  using pointer = value_type *;
  using const_pointer = const value_type *;
  using reference = value_type &;
  using const_reference = const value_type &;
  using size_type = size_t;
  using difference_type = std::ptrdiff_t;

  //=== [I] Special Functions
  /*!
   * Default constructor. Creates a vector with "count" copies of "value".
   * In case no parameters are provided, the vector will be created empty.
   * \param count number of elements to insert.
   * \param value data to store in the vector.
   */
  Vector(size_type count = 0, value_type value = T()) {
    if (count > 0) {
      m_size = count;
      m_array = new T[m_size];
      for (size_type index{0}; index < m_size; ++index) {
        m_array[index] = value;
      }
    }
  }
  /*!
   * Creates a vector with the same size and the same elements as the ilist.
   * \param ilist initializer_list with elements to insert in the vector.
   */
  Vector(std::initializer_list<value_type> ilist) {
    for (auto it = ilist.begin(); it != ilist.end(); ++it) {
      push_back(*it);
    }
  }
  /*!
   * Creates a vector with the data in the range [begin, end).
   * \param begin beginning of the range.
   * \param end ending of the range (not included).
   */
  template <typename InputIt> Vector(InputIt begin, InputIt end) {
    assign(begin, end);
  }
  /*!
   * Makes the vector contain the same data stored in the range [begin, end).
   * \param begin beginning of the range.
   * \param end ending of the range (not included).
   */
  template <typename InputIt> Vector assign(InputIt begin, InputIt end) {
    reserve(std::distance(begin, end));
    m_size = std::distance(begin, end);
    for (size_type index{0}; begin != end; ++begin, ++index) {
      m_array[index] = *begin;
    }
  }
  /*!
   * Makes the vector equal to the ilist.
   * \param ilist initializer_list with elements to insert in the vector.
   */
  Vector assign(std::initializer_list<value_type> ilist) {
    clear();
    for (auto it = ilist.begin(); it != ilist.end(); ++it) {
      push_back(*it);
    }
    return *this;
  }
  /*!
   * Makes the vector equal to the ilist.
   * \param ilist initializer_list with elements to insert in the vector.
   */
  Vector operator=(std::initializer_list<value_type> ilist) {
    return assign(ilist);
  }
  /// Deallocates memory.
  ~Vector() {
    if (m_array != nullptr) {
      delete[] m_array;
    }
  }

  //=== [II] Element Access
  /// Access the element in the front of the container.
  reference front() { return m_array[0]; }
  /// Access the element in the front of the container.
  const_reference front() const { return m_array[0]; }
  /// Access the element in the end of the container.
  reference back() { return m_array[m_size - 1]; }
  /// Access the element in the end of the container.
  const_reference back() const { return m_array[m_size - 1]; }
  /// Consults the pointer to the underlying array storing the data.
  pointer data() { return m_array; }
  /// Consults the pointer to the underlying array storing the data.
  const_pointer data() const { return m_array; }
  /// Access the element at the "pos" position in the container.
  reference at(size_type pos) { return m_array[pos]; }
  /// Access the element at the "pos" position in the container.
  const_reference at(size_type pos) const { return m_array[pos]; }
  /// Access the element at the "pos" position in the container.
  reference operator[](size_type pos) { return at(pos); }
  /// Access the element at the "pos" position in the container.
  const_reference operator[](size_type pos) const { return at(pos); }

  //=== [III] Iterators
  /*!
   * Gets a iterator to the beginning of the container.
   * \return iterator pointing to the beginning of the container.
   */
  iterator begin() { return iterator(m_array); }
  /*!
   * Gets a iterator to the element past the last element of  the container.
   * \return iterator to the element past the last element of the container.
   */
  iterator end() { return iterator(m_array + m_size); }

  //=== [IV] Capacity
  /// Consults whether the container is empty.
  bool empty() const { return m_size == 0; }
  /// Consults the number of elements in the container.
  size_type size() const { return m_size; }
  /*!\
   * If new_capacity is greater than the current capacity of the container, the
   * capacity if increased to new_capacity, otherwise the function does nothing.
   * \param new_capacity capacity that the container should have.
   */
  void reserve(size_type new_capacity) {
    if (new_capacity > m_capacity) {
      pointer tmp = new value_type[new_capacity];
      m_capacity = new_capacity;
      for (size_type index{0}; index < m_size; ++index) {
        tmp[index] = m_array[index];
      }
      delete[] m_array;
      m_array = tmp;
    }
  }
  /// Consults the capacity of the container.
  size_type capacity() const { return m_capacity; }
  /*!
   * Frees up unused memory, reducing the capacity of the container. Makes
   * the capacity equal to size. If the capacity is greater than size, all
   * references to elements are invalidated.
   */
  void shrink_to_fit() {
    if (m_capacity > m_size) {
      pointer tmp = new value_type[m_size];
      for (int index{0}; index < m_size; ++index) {
        tmp[index] = m_array[m_size];
      }
      delete[] m_array;
      m_capacity = m_size;
      m_array = tmp;
    }
  }

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
  class iterator {
  public:
    /*!
     * Constructs the iterator.
     * \param pointer to a value in the container.
     */
    iterator(pointer pointer = nullptr) { m_pointer = pointer; }
    /// Default copy constructor.
    iterator(const iterator &copy) = default;
    /// Default destructor.
    ~iterator() = default;
    /// Dereference operator.
    reference operator*() { return *m_pointer; }
    /// Constant dereference operator.
    const_reference operator*() const { return *m_pointer; }
    /// Arrow operator.
    pointer operator->() const { return m_pointer; }
    /// Pre-increment.
    iterator operator++() {
      ++m_pointer;
      return iterator(m_pointer);
    }
    /// Pos-increment.
    iterator operator++(int) {
      pointer copy = m_pointer;
      ++m_pointer;
      return iterator(copy);
    }
    /// Pre-decrement.
    iterator operator--() {
      --m_pointer;
      return iterator(m_pointer);
    }
    /// Pos-decrement.
    iterator operator--(int) {
      pointer copy = m_pointer;
      --m_pointer;
      return iterator(copy);
    }
    /*!
     * Checks whether two iterators are different.
     * \return flag that indicates whether iterator are different.
     */
    bool operator!=(const iterator &right) {
      return m_pointer != right.m_pointer;
    }
    /*!
     * Checks whether two iterator are equal.
     * \return flag that indicates whether iterator are equal.
     */
    bool operator==(const iterator &right) {
      return m_pointer == right.m_pointer;
    }

  private:
    pointer m_pointer; //!< Pointer to a element in the container.
  };

private:
  pointer m_array{nullptr};
  size_type m_size{0};
  size_type m_capacity{0};
};
} // namespace sc
  //
#include "incl.h"

#endif // VECTOR_H
