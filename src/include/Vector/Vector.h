#ifndef VECTOR_H
#define VECTOR_H

#include <cstddef> // size_t, ptrdiff_t
#include <initializer_list>
#include <iterator> // distance

// Namespace for sequential containers(sc).
namespace sc {
/*!
 * Implementation of a vector data structure.
 *
 * \author Eduardo Marinho (eduardo.nestor.marinho228@gmail.com)
 */
template <typename T> class vector {
public:
  // Forward declaration.
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
  vector(size_type count = 0, value_type value = T()) {
    if (count > 0) {
      m_size = count;
      m_capacity = count;
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
  vector(std::initializer_list<value_type> ilist) {
    reserve(ilist.size());
    for (auto it = ilist.begin(); it != ilist.end(); ++it) {
      push_back(*it);
    }
  }
  /*!
   * Copy constructor. Creates a vector equivalent to "copy".
   * \param copy vector to be copied.
   */
  vector(const vector &copy) {
    reserve(copy.m_capacity);
    m_size = copy.m_size;
    for (int index{0}; index < m_size; ++index) {
      m_array[index] = copy.m_array[index];
    }
  }
  /*!
   * Creates a vector with the data in the range [begin, end).
   * \param begin beginning of the range.
   * \param end ending of the range (not included).
   */
  template <typename InputIt> vector(InputIt begin, InputIt end) {
    assign(begin, end);
  }
  /*!
   * Makes the vector contain the same data stored in the range [begin, end).
   * \param begin beginning of the range.
   * \param end ending of the range (not included).
   */
  template <typename InputIt> void assign(InputIt begin, InputIt end) {
    clear();
    reserve(std::distance(begin, end));
    for (size_type index{0}; begin != end; ++begin, ++index) {
      // m_array[index] = *begin;
      push_back(*begin);
    }
  }
  /*!
   * Makes the vector equal to the ilist.
   * \param ilist initializer_list with elements to insert in the vector.
   */
  void assign(std::initializer_list<value_type> ilist) {
    clear();
    reserve(ilist.size());
    for (auto it = ilist.begin(); it != ilist.end(); ++it) {
      push_back(*it);
    }
  }
  /*!
   * Makes the vector equal to the ilist.
   * \param ilist initializer_list with elements to insert in the vector.
   */
  vector operator=(std::initializer_list<value_type> ilist) {
    assign(ilist);
    return *this;
  }
  /// Deallocates memory.
  ~vector() {
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
  /*!
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
      if (m_array != nullptr) {
        delete[] m_array;
      }
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
  /// Removes all elements of the container, i.e. empties it.
  void clear() { m_size = 0; }
  /*!
   * Inserts the data "value" before the position pointed by "pos".
   * \param pos iterator pointing to the element past the position to insert.
   * \param value data to insert.
   * \return iterator pointing to the inserted value.
   */
  iterator insert(iterator pos, const_reference value) {
    size_type pos_index = std::distance(m_array, &pos);
    if (m_capacity == 0) {
      reserve(1);
    } else if (m_size == m_capacity) {
      reserve(2 * m_size);
    }
    pos = iterator(m_array + pos_index);
    for (iterator it_first = pos, it_last = end(); it_first != end();
         ++it_first, --it_last) {
      *it_last = *it_first;
    }
    *pos = value;
    ++m_size;
    return pos;
  }
  /*!
   * Inserts "count" copies of "value" before position pointed by "pos".
   * Note: be careful to avoid ambiguity with the range insert function.
   * \param pos iterator pointing to the element past the position to insert.
   * \param count number of copies to insert.
   * \param value data to insert.
   * \return iterator pointing to the first element inserted, or "pos" if
   *         "count" == 0.
   */
  iterator insert(iterator pos, size_type count, const_reference value) {
    size_type pos_index = std::distance(m_array, &pos);
    if (m_capacity < m_size + count) {
      reserve(m_size + count);
    }
    pos = iterator(m_array + pos_index);

    for (iterator it_origin = end() - 1, it_destiny = end() + count - 1;
         it_origin != pos - 1; --it_origin, --it_destiny) {
      *it_destiny = *it_origin;
    }
    for (auto runner = pos; runner != pos + count; ++runner) {
      *runner = value;
    }
    m_size += count;
    return pos;
  }
  /*!
   * Inserts elements in range [first, last) before position pointed by "pos".
   * Note: be careful to avoid ambiguity with the copy insert function.
   * \param first beginning of the range of elements.
   * \param last end of the range of elements.
   * \return iterator pointing to the first element inserted, or "pos" if
   * "first" == "last".
   */
  template <class InputIt>
  iterator insert(iterator pos, InputIt first, InputIt last) {
    size_type pos_index = std::distance(m_array, &pos);
    size_type number_elements = std::distance(first, last);
    if (m_capacity < m_size + number_elements) {
      reserve(m_size + number_elements);
    }
    pos = iterator(m_array + pos_index);

    for (iterator it_origin = end() - 1,
                  it_destiny = end() + number_elements - 1;
         it_origin != pos - 1; --it_origin, --it_destiny) {
      *it_destiny = *it_origin;
    }
    for (iterator runner = pos; first != last; ++runner, ++first) {
      *runner = *first;
    }
    m_size += number_elements;
    return pos;
  }
  /*!
   * Inserts elements from "ilist" before position pointed by "pos".
   * \param pos iterator pointing to the element past the position to insert.
   * \param ilist initializer_list with elements to insert.
   * \return iterator pointing to first element inserted, or "pos" if "ilist" is
   *         empty.
   */
  iterator insert(iterator pos, std::initializer_list<T> ilist) {
    size_type pos_index = std::distance(m_array, &pos);
    if (m_capacity < m_size + ilist.size()) {
      reserve(m_size + ilist.size());
    }
    pos = iterator(m_array + pos_index);

    for (iterator it_origin = end() - 1, it_destiny = end() + ilist.size() - 1;
         it_origin != pos - 1; --it_origin, --it_destiny) {
      *it_destiny = *it_origin;
    }
    for (auto ilist_runner = ilist.begin(); ilist_runner != ilist.end();
         ++ilist_runner, ++pos) {
      *pos = *ilist_runner;
    }
    m_size += ilist.size();
    return pos - ilist.size();
  }
  /*!
   * Removes the element pointed by "pos".
   * \param pos iterator pointing to the element to remove.
   * \return iterator pointing to the element past the removed one. Note, that
   *         it can return end().
   */
  iterator erase(iterator pos) {
    for (iterator it = pos + 1; it != end(); ++it) {
      *(it - 1) = *it;
    }
    --m_size;
    return pos;
  }
  /*!
   * Removes the range of elements ["first", "last").
   * \param first iterator pointing to the beginning of the range.
   * \param last iterator pointing to the end of the range (not included).
   * \return iterator pointing to the element past the removed one. Note, that
   *         it can return end().
   */
  iterator erase(iterator first, iterator last) {
    for (auto copy_it = last + 1, erase_it = first; erase_it != last;
         ++erase_it, ++copy_it) {
      *erase_it = *copy_it;
    }
    m_size -= last - first;
    return iterator(first);
  }
  /*!
   * Changes the size of the container. If new_size > size, increases size
   * of the container inserting copies of "value". If new_size < size, reduces
   * the size of the container. Otherwise, do nothing.
   * \param new_size new size of the container.
   * \param value data to store in case on need. If not provided, default
   *        constructor will be used.
   */
  void resize(size_type new_size, value_type value = value_type()) {
    if (new_size > m_capacity) {
      reserve(new_size);
      for (size_type index{m_size}; index < new_size; ++index) {
        m_array[index] = value_type();
      }
    }
    m_size = new_size;
  }
  /*!
   * Inserts a element in the end of the container.
   * \param value data to store.
   */
  void push_back(value_type value) { insert(end(), value); }
  /*!
   * Inserts a element in the beginning of the container.
   * \param value data to store.
   */
  void push_front(value_type value) { insert(begin(), value); }

  class iterator {
  public:
    //=== Aliases.
    using value_type = T;
    using pointer = value_type *;
    using const_pointer = const value_type *;
    using reference = value_type &;
    using const_reference = const value_type &;
    using size_type = size_t;
    using difference_type = std::ptrdiff_t;

    // Deleting default constructor.
    iterator() = delete;
    /*!
     * Constructs the iterator.
     * \param pointer to a value in the container.
     */
    iterator(pointer pointer) { m_pointer = pointer; }
    /// Default copy constructor.
    iterator(const iterator &copy) = default;
    /// Default destructor.
    ~iterator() = default;
    /// Dereference operator.
    reference operator*() { return *m_pointer; }
    /// Constant dereference operator.
    const_reference operator*() const { return *m_pointer; }
    /// Consults the underlying pointer of the iterator.
    pointer operator&() { return m_pointer; }
    /// Consults the underlying pointer of the iterator.
    const_pointer operator&() const { return m_pointer; }
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
    /// Increment operator.
    friend iterator operator+(iterator it, int increment) {
      return iterator(&it + increment);
    }
    /// Increment operator.
    friend iterator operator+(int increment, iterator it) {
      return iterator(&it + increment);
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
    /// Decrement operator.
    friend iterator operator-(iterator it, int decrement) {
      return iterator(&it - decrement);
    }
    /// Difference operator.
    friend size_type operator-(iterator left, iterator right) {
      return left.m_pointer - right.m_pointer;
    }
    /*!
     * Makes this be equal to the iterator "copy".
     * \param copy iterator to copy the data from.
     */
    iterator operator=(const iterator &copy) {
      m_pointer = copy.m_pointer;
      return copy;
    }
    /// Checks whether two iterator are different.
    bool operator!=(const iterator &right) {
      return m_pointer != right.m_pointer;
    }
    /// Checks whether two iterator are equal.
    bool operator==(const iterator &right) {
      return m_pointer == right.m_pointer;
    }

  private:
    pointer m_pointer; //!< Pointer to a element in the container.
  };

private:
  pointer m_array{nullptr}; //!< Pointer to the beginning of the container.
  size_type m_size{0};      //!< Number of elements in the container.
  size_type m_capacity{0};  //!< Capacity of the container.
};
} // namespace sc

#endif // VECTOR_H
