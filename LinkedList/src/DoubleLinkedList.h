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
  list(size_type count = 0, const_reference value = value_type()) {
    initialize();
    assign(count, value);
  }
  template <typename InputIt> list(InputIt first, InputIt last) {
    initialize();
    assign(first, last);
  }
  list(list &other) {
    initialize();
    assign(other);
  }
  list(list &&other) {
    initialize();
    assign(std::move(other));
  }
  list(std::initializer_list<value_type> ilist) {
    initialize();
    for (auto runner = ilist.begin(); runner != ilist.end(); ++runner) {
      push_back(*runner);
    }
  }
  ~list() {
    clear();
    delete m_head;
    delete m_tail;
  }
  list_reference operator=(list &other) {
    assign(other);
    return other;
  }
  list_reference operator=(list &&other) {
    assign(other);
    return *this;
  }
  list_reference operator=(std::initializer_list<value_type> ilist) {
    assign(ilist);
    return *this;
  }
  void assign(list &other) {
    clear();
    for (auto v : other) {
      push_back(v);
    }
  }
  void assign(list &&other) {
    clear();
    m_head->next = other.m_head->next;
    m_tail->previous = other.m_tail->previous;
    m_head->next->previous = m_head;
    m_tail->previous->next = m_tail;
    m_size = other.m_size;
    other.reset_controls();
  }
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
  reference front() { return m_head->next->data; };
  const_reference front() const { return m_head->next->data; };
  reference back() { return m_tail->previous->data; };
  const_reference back() const { return m_tail->previous->data; };

  ///=== [III] Iterators.
  iterator begin() { return iterator(m_head->next); }
  iterator end() { return iterator(m_tail); }

  ///=== [IV] Capacity.
  [[nodiscard]] bool empty() const { return m_size == 0; }
  [[nodiscard]] size_type size() const { return m_size; }

  ///=== [V] Modifiers.
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
  iterator insert(iterator pos, size_type count, const_reference value) {
    iterator first_inserted = insert(pos, value);
    while (--count) {
      insert(pos, value);
    }
    return first_inserted;
  }
  // TODO be carefully with ambiguity with copy insert.
  template <class InputIt>
  iterator insert(iterator pos, InputIt first, InputIt last) {
    iterator first_inserted = insert(pos, *first++);
    for (; first != last; ++first) {
      insert(pos, *first);
    }
    return first_inserted;
  }
  iterator insert(iterator pos, std::initializer_list<value_type> ilist) {
    iterator first_inserted = insert(pos, *ilist.begin());
    for (auto runner = ilist.begin() + 1; runner != ilist.end(); ++runner) {
      insert(pos, *runner);
    }
    return first_inserted;
  }
  iterator erase(iterator pos) {
    --m_size;
    auto past_removed = pos + 1;
    (&pos)->previous->next = (&pos)->next;
    (&pos)->next->previous = (&pos)->previous;
    delete &pos;
    return past_removed;
  }
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
  void push_back(const_reference value) { insert(end(), value); }
  void pop_back() { erase(--end()); }
  void push_front(const_reference value) { insert(begin(), value); }
  void pop_front() { erase(begin()); }
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

    iterator() = default;
    iterator(node_pointer pointer) : m_pointer{pointer} {}
    ~iterator() = default;
    node_pointer operator&() { return m_pointer; }
    const_node_pointer operator&() const { return m_pointer; }
    reference operator*() { return m_pointer->data; }
    const_reference operator*() const { return m_pointer->data; }
    pointer operator->() { return &m_pointer->data; }
    const_pointer operator->() const { return &m_pointer->data; }
    iterator operator++() {
      m_pointer = m_pointer->next;
      return *this;
    }
    iterator operator++(int) {
      iterator copy = *this;
      ++(*this);
      return copy;
    }
    friend iterator operator+(iterator it, difference_type increment) {
      for (difference_type counter{0}; counter < increment; ++counter) {
        ++it;
      }
      for (difference_type counter{0}; counter > increment; --counter) {
        --it;
      }
      return it;
    }
    friend iterator operator+(difference_type increment, iterator it) {
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
    friend iterator operator-(iterator it, difference_type decrement) {
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
