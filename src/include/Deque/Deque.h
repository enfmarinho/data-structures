#ifndef DEQUE_H
#define DEQUE_H

#include <algorithm> // move
#include <array>
#include <cstddef> // size_t, ptrdiff_t
#include <cstdlib> // abs
#include <initializer_list>
#include <iterator> // distance
#include <memory>   // unique_ptr, make_unique
#include <vector>

// Namespace for sequential containers(sc).
namespace sc {
/*!
 * Double ended queue. Allows quick storage and deletion of values in both ends
 * of the container, as well as allows quick retrieval of elements. Important:
 * it is not implemented as a double linked list, so its not efficient to store
 * and delete in the middle of the container, however it is faster to traverse
 * and search for values at specific positions the container.
 * \tparam T data type to store.
 * \tparam BlockSize size of each block in the deque.
 * \tparam DefaultMobCapacity initial capacity of the map of blocks.
 *
 * \author Eduardo Marinho (eduardo.nestor.marinho228@gmail.com)
 */
template <typename T, size_t BlockSize = 3, size_t DefaultMobCapacity = 5>
class deque {
public:
  // Forward declaration.
  template <typename ItType, typename BlockItr, typename ItemItr>
  class DequeIterator;
  //=== Aliases.
  using value_type = T;
  using pointer = value_type *;
  using const_pointer = const value_type *;
  using reference = value_type &;
  using const_reference = const value_type &;
  using size_type = size_t;
  using difference_type = std::ptrdiff_t;
  using block_t = std::array<T, BlockSize>;
  using block_list_t = std::vector<std::unique_ptr<block_t>>;
  using iterator = DequeIterator<T, typename block_list_t::iterator,
                                 typename block_t::iterator>;
  using const_iterator =
      DequeIterator<const T, typename block_list_t::const_iterator,
                    typename block_t::const_iterator>;

  //=== [I] Special Functions.
  /// Creates an empty container.
  deque() { initialize_control_variables(); }
  /*!
   * Construct a deque containing the elements presents in "ilist".
   * \param ilist initializer_list with elements to insert in the deque.
   */
  deque(std::initializer_list<value_type> ilist) {
    initialize_control_variables();
    reallocate_mob((2 * ilist.size()));
    if (ilist.size() == 0) {
      return;
    }
    for (auto runner = ilist.begin() + ilist.size() / 2;
         runner != ilist.begin(); --runner) {
      push_front(*runner);
    }
    push_front(*ilist.begin());
    for (auto runner = ilist.begin() + ilist.size() / 2 + 1;
         runner != ilist.end(); ++runner) {
      push_back(*runner);
    }
  }
  /*!
   * Construct a deque containing the elements presents in the range
   * [first, last).
   * \param first beginning of the range to insert.
   * \param last end of the range to insert (not included).
   */
  template <typename InputIt> deque(InputIt first, InputIt last) {
    initialize_control_variables();
    size_type range_size = last - first;
    if (range_size == 0) {
      return;
    }
    reallocate_mob(2 * range_size);
    for (difference_type index = range_size / 2; index >= 0; --index) {
      push_front(*(first + index));
    }
    for (difference_type index = range_size / 2 + 1; index < range_size;
         ++index) {
      push_back(*(first + index));
    }
  }
  /// Construct a clone of the deque "other".
  deque(const deque &other) {
    initialize_control_variables();
    reallocate_mob(other.m_mob_capacity);
    if (other.empty()) {
      return;
    }
    for (iterator it = other.m_head + other.m_size / 2; it != other.m_head;
         --it) {
      push_front(*it);
    }
    push_front(*other.m_head);
    for (iterator it = other.m_head + other.m_size / 2 + 1; it != other.m_tail;
         ++it) {
      push_back(*it);
    }
  }
  /// Construct a deque that takes ownership of memory from "other".
  deque(deque &&other) {
    initialize_control_variables();
    std::swap(m_mob_capacity, other.m_mob_capacity);
    std::swap(m_size, other.m_size);
    m_mob = std::move(other.m_mob);
    m_head = std::move(other.m_head);
    m_tail = std::move(other.m_tail);
  }

  //=== [II] Element Access.
  /// Consults the element in the "pos" index at the container.
  reference at(size_type pos) { return *(m_head + pos); }
  /// Consults the element in the "pos" index at the container.
  const_reference at(size_type pos) const { return *(m_head + pos); }
  /// Consults the element in the "pos" index at the container.
  reference operator[](size_type pos) { return at(pos); }
  /// Consults the element in the "pos" index at the container.
  const_reference operator[](size_type pos) const { return at(pos); }
  /// Consults the element at the front of the container.
  reference front() { return *m_head; }
  /// Consults the element at the front of the container.
  const_reference front() const { return *m_head; }
  /// Consults the element at the back of the container.
  reference back() { return *(m_tail - 1); }
  /// Consults the element at the back of the container.
  const_reference back() const { return *(m_tail - 1); }

  //=== [III] Iterators.
  /// Gets an iterator to the begin of the container.
  iterator begin() { return m_head; }
  /// Gets an iterator to the end of the container.
  iterator end() { return m_tail; }
  /// Gets a constant iterator to the begin of the container.
  const_iterator cbegin() const { return const_iterator(m_head); }
  /// Gets a constant iterator to the end of the container.
  const_iterator cend() const { return const_iterator(m_tail); }

  //=== [IV] Capacity.
  /// Consults whether the container is empty.
  [[nodiscard]] bool empty() const { return m_size == 0; }
  /// Consults the number of elements in the container.
  [[nodiscard]] size_type size() const { return m_size; }
  /// Frees the memory not being used.
  void shrink_to_fit() {
    if (m_head.get_block_it() != m_mob.begin()) {
      for (typename block_list_t::iterator runner = m_head.get_block_it() - 1;
           runner != m_mob.begin(); --runner) {
        if (*runner != nullptr) {
          runner->reset();
        }
      }
      if (*m_mob.begin() != nullptr) {
        m_mob.begin()->reset();
      }
    }
    for (typename block_list_t::iterator runner = m_tail.get_block_it() + 1;
         runner != m_mob.end(); ++runner) {
      if (*runner != nullptr) {
        runner->reset();
      }
    }
  }

  //=== [V] Modifiers.
  /// Clears the contents of the container, i.e. makes it empty.
  void clear() {
    typename block_list_t::iterator mob_mid_it =
        m_mob.begin() + m_mob_capacity / 2;
    typename block_t::iterator block_mid_it =
        (*mob_mid_it)->begin() + BlockSize / 2;
    m_head = iterator(mob_mid_it, block_mid_it);
    m_tail = m_head;
    m_size = 0;
  }
  /*!
   * Inserts the element "value" before the iterator "pos".
   * \param pos iterator to the element past the position to insert "value".
   * \param value element to insert.
   * \return iterator pointing to the inserted value.
   */
  iterator insert(iterator pos, const_reference value) {
    position_e position = closer_to(pos);
    pos = space_vacancy(position, pos, 1);
    if (pos == m_head) {
      --m_head;
      --pos;
    } else if (position == position_e::FRONT) {
      copy(m_head, pos, m_head - 1);
      --m_head;
    } else {
      copy_backward(pos, m_tail, m_tail + 1);
      ++m_tail;
    }
    ++m_size;
    *pos = value;
    return pos;
  }
  /*!
   * Inserts "count" copies of the element "value" before the iterator "pos".
   * \param count number of copies to insert.
   * \param value element to insert.
   * \param pos iterator to the element past the position to insert the
   *            elements.
   * \return iterator pointing to the first inserted value, or "pos" if none was
   *         inserted.
   */
  iterator insert(size_type count, const_reference value, iterator pos) {
    m_size += count;
    position_e position = closer_to(pos);
    pos = space_vacancy(position, pos, count);
    if (pos == m_tail) {
      m_tail = m_tail + count;
      pos = m_tail - 1;
    } else if (position == position_e::FRONT) {
      copy(m_head, pos, m_head - count);
      --pos;
      m_head = m_head - count;
    } else {
      copy_backward(pos + 1, m_tail, m_tail + count);
      m_tail = m_tail + count;
      pos = pos + count - 1;
    }
    for (size_type counter{0}; counter < count; ++counter) {
      *pos = value;
      --pos;
    }
    return pos + 1;
  }
  /*!
   * Inserts the elements in the range [first, last) before the iterator "pos".
   * \param pos iterator to the element past the position to insert the
   *            elements.
   * \param first iterator to the start of the range.
   * \param last iterator to the end of the range (not included).
   * \return iterator pointing to the first inserted value, or "pos" if none was
   *         inserted.
   */
  template <class InputIt>
  iterator insert(iterator pos, InputIt first, InputIt last) {
    size_type range_size = last - first;
    m_size += range_size;
    position_e position = closer_to(pos);
    pos = space_vacancy(position, pos, range_size);
    if (position == position_e::FRONT) {
      copy(m_head, pos, m_head - range_size);
      for (InputIt pointer = last - 1; pointer != first; --pointer) {
        --pos;
        *pos = *pointer;
      }
      --pos;
      *pos = *first;
      m_head = m_head - range_size;
      return pos;
    } else if (pos != m_tail) {
      copy_backward(pos + 1, m_tail, m_tail + range_size);
    }
    for (; first != last; ++first) {
      *pos = *first;
      ++pos;
    }
    m_tail = m_tail + range_size;
    return pos - range_size;
  }
  /*!
   * Inserts the elements in "ilist" before the iterator "pos".
   * \param pos iterator to the element past the position to insert the
   *            elements.
   * \param ilist initializer_list with the elements to be inserted.
   * \return iterator pointing to the first inserted value, or "pos" if none was
   *         inserted.
   */
  iterator insert(iterator pos, std::initializer_list<value_type> ilist) {
    if (ilist.size() == 0) {
      return pos;
    }
    m_size += ilist.size();
    position_e position = closer_to(pos);
    pos = space_vacancy(position, pos, ilist.size());
    if (position == position_e::FRONT) {
      copy(m_head, pos, m_head - ilist.size());
      for (auto list_it = ilist.end() - 1; list_it != ilist.begin();
           --list_it) {
        --pos;
        *pos = *list_it;
      }
      --pos;
      *pos = *ilist.begin();
      m_head = m_head - ilist.size();
      return pos;
    } else if (pos != m_tail) {
      copy_backward(pos, m_tail, m_tail + ilist.size());
    }
    for (value_type value : ilist) {
      *pos = value;
      ++pos;
    }
    m_tail = m_tail + ilist.size();
    return pos - ilist.size();
  }
  /*!
   * Erase the element being pointed by "pos".
   * \param pos iterator pointing to the element to be removed.
   * \return iterator pointing to the element following the removed one.
   */
  iterator erase(iterator pos) {
    --m_size;
    position_e position = closer_to(pos);
    if (position == position_e::FRONT) {
      copy_backward(m_head, pos, pos + 1);
      ++m_head;
      return pos + 1;
    } else {
      copy(pos + 1, m_tail, pos);
      --m_tail;
      return pos;
    }
  }
  /*!
   * Erases all elements in the range [first, last).
   * \param first iterator to the start of the range.
   * \param last iterator to the end of the range (not included).
   * \return iterator to the element past the last one removed or "last" if none
   *         was removed
   */
  iterator erase(iterator first, iterator last) {
    if (first == last) {
      return last;
    }
    difference_type range_size = last - first;
    m_size -= range_size;
    if (closer_to(first) == position_e::FRONT) {
      copy_backward(m_head, first, last);
      m_head = m_head + range_size;
      return last;
    } else {
      copy(last, m_tail, first);
      m_tail = m_tail - range_size;
      return first;
    }
  }
  /// Inserts "value" in the back of the deque.
  void push_back(const_reference value) { insert(end(), value); }
  /// Inserts "value" in the front of the deque.
  void push_front(const_reference value) { insert(begin(), value); }
  /// Removes the last element of the deque.
  void pop_back() { erase(--end()); }
  /// Removes the first element of the deque.
  void pop_front() { erase(begin()); }
  /*!
   * Resizes the deque to contain "count" elements. If "counts" == m_size does
   * nothing, else if "count" < m_size the container is reduced to the first
   * "count" elements, else if "count" > m_size additional copies of value are
   * appended, if "value" is not provided, default constructor will be used.
   * \param count new size of the container.
   * \param value the value to initialize the new elements with, if not provided
   *        the default constructor of the type will be used.
   */
  void resize(size_type count, const_reference value = value_type()) {
    if (m_size > count) {
      m_tail = m_tail - (m_size - count);
      m_size = count;
    }
    for (size_type counter{m_size}; counter < count; ++counter) {
      push_back(value);
    }
  }

  template <typename ItType, typename BlockItr, typename ElementItr>
  class DequeIterator {
  public:
    ///=== Aliases.
    using value_type = ItType;
    using pointer = value_type *;
    using const_pointer = const value_type *;
    using reference = value_type &;
    using const_reference = const value_type &;
    using size_type = size_t;
    using difference_type = std::ptrdiff_t;

    /// Default constructor.
    DequeIterator() = default;
    /// Constructs an iterator.
    DequeIterator(BlockItr block, ElementItr element)
        : m_block(block), m_element(element) {}
    /// Copy constructor.
    DequeIterator(const DequeIterator &copy)
        : m_block(copy.m_block), m_element(copy.m_element) {}
    /// Makes this iterator points to the same element as "other".
    DequeIterator operator=(const DequeIterator &other) {
      m_block = other.m_block;
      m_element = other.m_element;
      return *this;
    }
    DequeIterator(iterator other)
        : m_block(other.get_block_it()), m_element(other.get_element_it()) {}
    /// Dereference operator
    reference operator*() { return *m_element; }
    /// Constant dereference operator
    const_reference operator*() const { return *m_element; }
    /// Gets the iterator to the block this iterator points to.
    BlockItr get_block_it() const { return m_block; }
    /// Gets the iterator to the element this iterator points to.
    ElementItr get_element_it() const { return m_element; }
    /// Increment operator.
    DequeIterator &operator++() {
      if (++m_element == (*m_block)->end()) {
        ++m_block;
        m_element = (*m_block)->begin();
      }
      return *this;
    }
    /// Post increment operator.
    DequeIterator &operator++(int) {
      DequeIterator copy = *this;
      ++(*this);
      return *this;
    }
    /// Decrement operator.
    DequeIterator &operator--() {
      if (m_element == (*m_block)->begin()) {
        --m_block;
        m_element = (*m_block)->end() - 1;
      } else {
        --m_element;
      }
      return *this;
    }
    /// Post decrement operator.
    DequeIterator &operator--(int) {
      DequeIterator copy = *this;
      --(*this);
      return copy;
    }
    /// Sum operator
    friend DequeIterator operator+(const DequeIterator &it,
                                   difference_type increment) {
      DequeIterator result = it;
      if (increment > 0) {
        difference_type element_index = it.m_element - (*it.m_block)->begin();
        difference_type blocks_to_advance =
            std::floor(element_index + increment) / (float)BlockSize;
        element_index = (element_index + (increment % BlockSize)) % BlockSize;
        result.m_block = it.m_block + blocks_to_advance;
        result.m_element = (*result.m_block)->begin() + element_index;
      }
      for (difference_type counter{0}; counter > increment; --counter) {
        --result;
      }
      return result;
    }
    /// Sum operator
    friend DequeIterator operator+(difference_type increment,
                                   const DequeIterator &it) {
      return it + increment;
    }
    /// Subtract operator
    friend DequeIterator operator-(const DequeIterator &it,
                                   difference_type decrement) {
      return it + (-decrement);
    }
    /// Subtract operator
    friend DequeIterator operator-(difference_type decrement,
                                   const DequeIterator &it) {
      return it + (-decrement);
    }
    /// Difference operator. Returns the distance between "rhs" to "lhs".
    friend difference_type operator-(const DequeIterator &lhs,
                                     const DequeIterator &rhs) {
      if (lhs.m_block == rhs.m_block) {
        return lhs.m_element - rhs.m_element;
      }
      difference_type lhs_index = lhs.m_element - (*lhs.m_block)->begin();
      difference_type rhs_index = rhs.m_element - (*rhs.m_block)->begin();
      return (lhs.m_block - rhs.m_block) * BlockSize + lhs_index - rhs_index;
    }
    /// Checks whether "lhs" is equal to "rhs".
    friend bool operator==(const DequeIterator &lhs, const DequeIterator &rhs) {
      return lhs.m_element == rhs.m_element and lhs.m_block == rhs.m_block;
    }
    /// Checks whether "lhs" is difference than "rhs".
    friend bool operator!=(const DequeIterator &lhs, const DequeIterator &rhs) {
      return !(lhs == rhs);
    }

  private:
    BlockItr m_block;     //!< Iterator to the block of the element.
    ElementItr m_element; //!< Iterator to the element.
  };

private:
  enum position_e {
    FRONT, //!< Refers to the begin.
    BACK   //!< Refers to the end.
  };

  //=== Auxiliary Functions.
  /*!
   * Consults the number of vacant spaces in the "position" far side of the
   * block.
   * \param position target side of the block.
   * \return number of spaces vacant in the "position" far side of the block.
   */
  size_type block_vacancy(position_e position) {
    if (position == position_e::FRONT) {
      return std::distance((*m_head.get_block_it())->begin(),
                           m_head.get_element_it());
    } else {
      return std::distance(m_tail.get_element_it(),
                           (*m_tail.get_block_it())->end()) -
             1;
    }
  }
  /*!
   * Consults the number of vacant blocks in the "position" far side of the mob.
   * \param position target side of the mob.
   * \return number of blocks vacant in the "position" far side of the mob.
   */
  size_type mob_vacancy(position_e position) {
    if (position == position_e::FRONT) {
      return std::distance(m_mob.begin(), m_head.get_block_it());
    } else {
      return std::distance(m_tail.get_block_it(), m_mob.end()) - 1;
    }
  }
  /*!
   * Assures that at least "count" vacancy spaces exists in the "position" far
   * side of the container.
   * \param position target side of the container.
   * \param pos iterator to an element in the container.
   * \param count number of spaces that needs to be vacant in the target
   *              position in the container.
   * \return iterator pointing to the same element "pos" was pointing to.
   */
  iterator space_vacancy(position_e position, iterator pos, size_type count) {
    size_type vacancy = block_vacancy(position);
    difference_type require_blocks =
        std::ceil(((float)count - vacancy) / BlockSize);
    if (mob_vacancy(position) < require_blocks) {
      difference_type index = pos - m_head;
      reallocate_mob(2 * m_size + count);
      pos = m_head + index;
    }
    allocate_blocks(position, require_blocks);
    return pos;
  }
  /*!
   * Guarantees that "count" blocks are allocated in the "position" far side of
   * the mob.
   * \param position target side of the mob.
   * \param count number of blocks guarantee to be allocate.
   */
  void allocate_blocks(position_e position, size_type count) {
    if (position == position_e::FRONT) {
      auto runner = m_head.get_block_it() - 1;
      for (size_type counter{0}; counter < count; ++counter, --runner) {
        if (*runner == nullptr) {
          *runner = std::make_unique<block_t>();
        }
      }
    } else if (*(++m_tail.get_block_it()) == nullptr) {
      auto runner = m_tail.get_block_it() + 1;
      for (size_type counter{0}; counter < count; ++counter, ++runner) {
        if (*runner == nullptr) {
          *runner = std::make_unique<block_t>();
        }
      }
    }
  }
  /*!
   * Reallocates mob to have the capacity to hold at least "new_size" elements.
   * If mob already has capacity for that, does nothing.
   * \param new_size minimum capacity that the mob should have.
   */
  void reallocate_mob(size_type new_size) {
    new_size = std::ceil((float)new_size / BlockSize);
    if (new_size > m_mob_capacity) {
      size_type size_difference = new_size - m_mob_capacity;
      block_list_t new_mob(new_size);
      typename block_list_t::iterator new_mob_runner =
          new_mob.begin() + size_difference / 2;
      for (typename block_list_t::iterator mob_runner = m_mob.begin();
           mob_runner != m_mob.end(); ++new_mob_runner, ++mob_runner) {
        if (mob_runner == m_head.get_block_it()) {
          m_head = iterator(new_mob_runner, m_head.get_element_it());
        }
        if (mob_runner == m_tail.get_block_it()) {
          m_tail = iterator(new_mob_runner, m_tail.get_element_it());
        }
        swap(*new_mob_runner, *mob_runner);
      }
      m_mob = std::move(new_mob);
      m_mob_capacity = new_size;
    }
  }
  /*!
   * Checks if iterator "pos", is closer to the end or to the begin of the
   * container.
   * \param pos iterator in question.
   * \return position_e that represents whether the iterator is closer to the
   *         begin "FRONT" or closer to the end "BACK".
   */
  position_e closer_to(iterator pos) {
    difference_type to_end = m_tail - pos;
    difference_type to_start = pos - m_head;
    return to_start < to_end ? position_e::FRONT : position_e::BACK;
  }
  template <class InputIt, class OutputIt>
  OutputIt copy(InputIt first, InputIt last, OutputIt d_first) {
    while (first != last) {
      *(first++) = *(d_first++);
    }
    return d_first;
  }
  template <class InputIt, class OutputIt>
  OutputIt copy_backward(InputIt first, InputIt last, OutputIt d_last) {
    while (first != last) {
      *(--d_last) = *(--last);
    }
    return d_last;
  }
  /// Initializes container variables.
  void initialize_control_variables() {
    m_size = 0;
    m_mob_capacity = DefaultMobCapacity;
    m_mob.resize(m_mob_capacity);
    typename block_list_t::iterator mob_mid_it =
        m_mob.begin() + DefaultMobCapacity / 2;
    *mob_mid_it = std::make_unique<block_t>();
    typename block_t::iterator block_mid_it =
        (*mob_mid_it)->begin() + BlockSize / 2;
    m_head = iterator(mob_mid_it, block_mid_it);
    m_tail = iterator(mob_mid_it, block_mid_it);
  }

  block_list_t m_mob;       //!< The dynamic map of blocks.
  size_type m_mob_capacity; //!< Capacity of the mob.
  size_type m_size;         //!< Number of elements in the container.
  iterator m_head;          //!< Iterator to the head block.
  iterator m_tail;          //!< Iterator to the tail block.
};
} // namespace sc

#endif // DEQUE_H
