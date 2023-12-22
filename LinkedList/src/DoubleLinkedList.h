#ifndef DOUBLE_LINKED_LIST_H
#define DOUBLE_LINKED_LIST_H

#include <climits>    // INT_MAX
#include <cstddef>    // size_t
#include <functional> // function<>, equal_to<>, greater<>
#include <initializer_list>

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
  /*!
   * Default constructor. Creates a list with "count" copies of "value".
   * In case no parameters are provided, the vector will be created empty.
   * \param count number of elements to insert.
   * \param value data to store in the vector.
   */
  list(size_type count = 0, const_reference value = value_type()) {
    initialize();
    assign(count, value);
  }
  /*!
   * Creates a list with the data in the range [begin, end).
   * \param begin beginning of the range.
   * \param end ending of the range (not included).
   */
  template <typename InputIt> list(InputIt first, InputIt last) {
    initialize();
    assign(first, last);
  }
  /*!
   * Copy constructor. Creates a list equivalent to "copy".
   * \param copy list to be copied.
   */
  list(list &other) {
    initialize();
    assign(other);
  }
  /*!
   * Move constructor. Creates a list with the nodes of "other" list.
   * After operation is done, "other" will be empty.
   * \param other list to take memory from.
   */
  list(list &&other) {
    initialize();
    assign(std::move(other));
  }
  /*!
   * Creates a list with the same size and the same elements as "ilist".
   * \param ilist initializer_list with the elements to insert.
   */
  list(std::initializer_list<value_type> ilist) {
    initialize();
    for (auto runner = ilist.begin(); runner != ilist.end(); ++runner) {
      push_back(*runner);
    }
  }
  /// Deallocates memory.
  ~list() {
    clear();
    delete m_head;
    delete m_tail;
  }
  /*!
   * Makes the list equivalent to "copy".
   * \param copy list to be copied.
   */
  list_reference operator=(list &other) {
    assign(other);
    return other;
  }
  /*!
   * Makes this be equal to "other" by taking ownership of its nodes.
   * After operation is done, "other" will be empty.
   * \param other list to take memory from.
   */
  list_reference operator=(list &&other) {
    assign(other);
    return *this;
  }
  /*!
   * Makes the list equal to the ilist.
   * \param ilist initializer_list with the elements to insert.
   */
  list_reference operator=(std::initializer_list<value_type> ilist) {
    assign(ilist);
    return *this;
  }
  /*!
   * Makes the list equivalent to "copy".
   * \param copy list to be copied.
   */
  void assign(list &other) {
    clear();
    for (auto v : other) {
      push_back(v);
    }
  }
  /*!
   * Makes this be equal to "other" by taking ownership of its nodes.
   * After operation is done, "other" will be empty.
   * \param other list to take memory from.
   */
  void assign(list &&other) {
    clear();
    m_head->next = other.m_head->next;
    m_tail->previous = other.m_tail->previous;
    m_head->next->previous = m_head;
    m_tail->previous->next = m_tail;
    m_size = other.m_size;
    other.reset_controls();
  }
  /*!
   * Make this have "count" copies of the data "value".
   * \param count number of elements to insert.
   * \param value data to store in the vector. If not provided, default
   *        constructor of the value_type will be used.
   */
  void assign(size_type count, const_reference value = value_type()) {
    // TODO optimize this. Make use of existing nodes.
    clear();
    while (count--) {
      push_back(value);
    }
    // iterator list_runner = begin();
    // size_type counter{0};
    // for (; m_size > 0 and counter < count; --m_size, ++list_runner,
    // ++counter) {
    //   *list_runner = value;
    // }
    // erase(list_runner, end());
    // for (; counter < count; ++counter) {
    //   push_back(value);
    // }
    // m_size = count;
  }
  /*!
   * Makes the list contain the same data stored in the range [begin, end).
   * \param begin beginning of the range.
   * \param end ending of the range (not included).
   */
  template <typename InputIt> void assign(InputIt first, InputIt last) {
    // TODO optimize this. Make use of existing nodes.
    clear();
    for (; first != last; ++first) {
      push_back(*first);
    }
    // auto list_runner = begin();
    // size_type new_size = std::distance(first, last);
    // for (; m_size > 0 and first != last; --m_size, ++first) {
    //   *list_runner = *first;
    // }
    // erase(list_runner, end());
    // for (; first != last; ++first) {
    //   push_back(*first);
    // }
    // m_size = new_size;
  }
  /*!
   * Makes the list equal to the ilist.
   * \param ilist initializer_list with the elements to insert.
   */
  void assign(std::initializer_list<value_type> ilist) {
    // TODO optimize this. Make use of existing nodes.
    clear();
    for (auto v : ilist) {
      push_back(v);
    }
    // iterator runner = begin();
    // auto ilist_runner = ilist.begin();
    // for (; m_size-- and ilist_runner != ilist.end(); ++runner,
    // ++ilist_runner) {
    //   (*runner) = *ilist_runner;
    // }
    // erase(runner, end());
    // for (; ilist_runner != ilist.end(); ++ilist_runner) {
    //   push_back(*ilist_runner);
    // }
    // m_size = ilist.size();
  }

  ///=== [II] Element Access.
  /// Access the element in the front of the container.
  reference front() { return m_head->next->data; };
  /// Access the element in the front of the container.
  const_reference front() const { return m_head->next->data; };
  /// Access the element in the end of the container.
  reference back() { return m_tail->previous->data; };
  /// Access the element in the end of the container.
  const_reference back() const { return m_tail->previous->data; };

  ///=== [III] Iterators.
  /*!
   * Gets a iterator to the beginning of the container.
   * \return iterator pointing to the beginning of the container.
   */
  iterator begin() { return iterator(m_head->next); }
  /*!
   * Gets a iterator to the element past the last element of  the container.
   * \return iterator to the element past the last element of the container.
   */
  iterator end() { return iterator(m_tail); }

  ///=== [IV] Capacity.
  /// Consults whether the container is empty.
  [[nodiscard]] bool empty() const { return m_size == 0; }
  /// Consults the number of elements in the container.
  [[nodiscard]] size_type size() const { return m_size; }

  ///=== [V] Modifiers.
  /// Removes all elements of the container, i.e. empties it.
  void clear() {
    if (m_size > 0) {
      iterator runner = begin();
      iterator it_end = end();
      while (runner != it_end) {
        node_pointer aux = &runner;
        ++runner;
        delete aux;
      }
      reset_controls();
    }
  }
  /*!
   * Inserts "value" data before node being pointer by "pos".
   * \param pos iterator pointing to the node past the position to insert.
   * \param value data to insert.
   * \return iterator pointing to the inserted node.
   */
  iterator insert(iterator pos, const_reference value) {
    ++m_size;
    node_pointer new_node = new Node(value, &pos, (&pos)->previous);
    (&pos)->previous->next = new_node;
    (&pos)->previous = new_node;
    return iterator(new_node);
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
    if (count == 0) {
      return pos;
    }
    iterator first_inserted = insert(pos, value);
    while (--count) {
      insert(pos, value);
    }
    return first_inserted;
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
    iterator first_inserted = insert(pos, *first++);
    for (; first != last; ++first) {
      insert(pos, *first);
    }
    return first_inserted;
  }

  /*!
   * Inserts elements from "ilist" before position pointed by "pos".
   * \param pos iterator pointing to the element past the position to insert.
   * \param ilist initializer_list with elements to insert.
   * \return iterator pointing to first element inserted, or "pos" if "ilist" is
   *         empty.
   */
  iterator insert(iterator pos, std::initializer_list<value_type> ilist) {
    iterator first_inserted = insert(pos, *ilist.begin());
    for (auto runner = ilist.begin() + 1; runner != ilist.end(); ++runner) {
      insert(pos, *runner);
    }
    return first_inserted;
  }
  /*!
   * Removes the element pointed by "pos".
   * \param pos iterator pointing to the element to remove.
   * \return iterator pointing to the element past the removed one. Note, that
   *         it can return end().
   */
  iterator erase(iterator pos) {
    --m_size;
    auto past_removed = pos + 1;
    (&pos)->previous->next = (&pos)->next;
    (&pos)->next->previous = (&pos)->previous;
    delete &pos;
    return past_removed;
  }
  /*!
   * Removes the range of elements ["first", "last").
   * \param first iterator pointing to the beginning of the range.
   * \param last iterator pointing to the end of the range (not included).
   * \return iterator pointing to the element past the removed one. Note, that
   *         it can return end().
   */
  iterator erase(iterator first, iterator last) {
    if (first == last) {
      return first;
    }
    (&first)->previous->next = &last;
    (&last)->previous = (&first)->previous;
    while (first != last) {
      auto aux = first + 1;
      delete &first;
      first = aux;
      --m_size;
    }
    return last;
  }
  /*!
   * Inserts a element in the end of the container.
   * \param value data to store.
   */
  void push_back(const_reference value) { insert(end(), value); }
  /// Removes the last element in the container.
  void pop_back() { erase(--end()); }
  /*!
   * Inserts a element in the beginning of the container.
   * \param value data to store.
   */
  void push_front(const_reference value) { insert(begin(), value); }
  /// Removes the first element in the container.
  void pop_front() { erase(begin()); }
  /*!
   * Changes the size of the container. If new_size > size, increases size
   * of the container inserting copies of "value". If new_size < size, reduces
   * the size of the container. Otherwise, do nothing.
   * \param new_size new size of the container.
   * \param value data to store in case on need. If not provided, default
   *        constructor will be used.
   */
  void resize(size_type count, const_reference value = value_type()) {
    if (m_size > count) {
      erase(begin() + count, end());
    }
    while (m_size < count) {
      push_back(value);
    }
  }

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

    /// Default constructor.
    iterator() = default;
    /// Makes a iterator pointing to the node at the address "pointer".
    iterator(node_pointer pointer) : m_pointer{pointer} {}
    /// Copy constructor.
    iterator(const iterator &) = default;
    /// Destructor.
    ~iterator() = default;
    /// Consults the underlying pointer of the node being pointed by iterator.
    node_pointer operator&() { return m_pointer; }
    /// Consults the underlying pointer of the node being pointed by iterator.
    const_node_pointer operator&() const { return m_pointer; }
    /// Gets a reference to the data being pointed by this iterator.
    reference operator*() { return m_pointer->data; }
    /// Gets a constant reference to the data being pointed by this iterator.
    const_reference operator*() const { return m_pointer->data; }
    /// Arrow operator.
    pointer operator->() { return &m_pointer->data; }
    /// Arrow operator.
    const_pointer operator->() const { return &m_pointer->data; }
    /// Pre-increment.
    iterator operator++() {
      m_pointer = m_pointer->next;
      return *this;
    }
    /// Pos-increment
    iterator operator++(int) {
      iterator copy = *this;
      ++(*this);
      return copy;
    }
    /*!
     * Increment operator.
     * \param it iterator to begin the traversal.
     * \param increment number of nodes to skip.
     * \return iterator pointing to "increment" nodes after "it".
     */
    friend iterator operator+(iterator it, difference_type increment) {
      for (difference_type counter{0}; counter < increment; ++counter) {
        ++it;
      }
      for (difference_type counter{0}; counter > increment; --counter) {
        --it;
      }
      return it;
    }
    /*!
     * Increment operator.
     * \param it iterator to begin the traversal.
     * \param increment number of nodes to skip.
     * \return iterator pointing to "increment" nodes after "it".
     */
    friend iterator operator+(difference_type increment, iterator it) {
      return it + increment;
    }
    /// Pre-decrement.
    iterator operator--() {
      m_pointer = m_pointer->previous;
      return *this;
    }
    /// Pos-decrement.
    iterator operator--(int) {
      iterator copy = *this;
      --(*this);
      return copy;
    }
    /*!
     * Difference operator.
     * \param it iterator to begin the traversal.
     * \param decrement number of nodes to go back.
     * \return iterator pointing to "decrement" nodes before "it".
     */
    friend iterator operator-(iterator it, difference_type decrement) {
      return it + (-decrement);
    }
    /// Difference operator. Returns the distance of this to "right".
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
    /*!
     * Makes this be equal to the iterator "copy".
     * \param copy iterator to copy the data from.
     */
    iterator operator=(const iterator &copy) {
      m_pointer = copy.m_pointer;
      return *this;
    }
    /// Checks whether two iterator are different.
    bool operator!=(const iterator &right) {
      return this->m_pointer != right.m_pointer;
    }
    /// Checks whether two iterator are equal.
    bool operator==(const iterator &right) {
      return this->m_pointer == right.m_pointer;
    }

  protected:
    node_pointer m_pointer{nullptr}; //!< Pointer to a node in the container.
  };

private:
  /// Initializes data members.
  void initialize() {
    m_head = new Node;
    m_tail = new Node(value_type(), nullptr, m_head);
    m_head->next = m_tail;
  }
  /// Reset control data members.
  void reset_controls() {
    m_size = 0;
    m_head->next = m_tail;
    m_tail->previous = m_head;
  }
  /*!
   * Inserts "node" before position pointed by "pos".
   * \param pos iterator pointing to the node past the position to insert.
   * \param node node to insert.
   * \return iterator to inserted node.
   */
  iterator insert_not_creating(iterator pos, Node *node) {
    ++m_size;
    node->next = &pos;
    node->previous = (&pos)->previous;
    (&pos)->previous->next = node;
    (&pos)->previous = node;
    return iterator(node);
  }

  node_pointer m_head; //!< Pointer to the head node.
  node_pointer m_tail; //!< Pointer to the tail node.
  size_type m_size{0}; //!< Number of elements in the list.
};
} // namespace sc

#endif
